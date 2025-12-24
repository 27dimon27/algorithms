#include "Huffman.h"
#include <vector>
#include <queue>
#include <memory>
#include <map>
#include <cstddef>

namespace {
    // структура узла дерева хаффмана
    struct HuffmanNode {
        byte symbol;
        size_t frequency;
        std::shared_ptr<HuffmanNode> left;
        std::shared_ptr<HuffmanNode> right;
        
        HuffmanNode(byte sym, size_t freq) 
            : symbol(sym), frequency(freq), left(nullptr), right(nullptr) {}
            
        // конструктор для внутренних узлов
        HuffmanNode(std::shared_ptr<HuffmanNode> l, std::shared_ptr<HuffmanNode> r)
            : symbol(0), frequency(l->frequency + r->frequency), left(l), right(r) {}
            
        bool isLeaf() const {
            return !left && !right;
        }
    };
    
    // компаратор для приоритетной очереди
    struct NodeCompare {
        bool operator()(const std::shared_ptr<HuffmanNode>& a, 
                       const std::shared_ptr<HuffmanNode>& b) {
            return a->frequency > b->frequency;
        }
    };
    
    // класс для побитовой записи в поток байтов
    class BitOutputStream {
    private:
        IOutputStream& output;
        byte buffer; // буфер для накопления битов
        int bitCount; // количество битов в буфере
        
    public:
        explicit BitOutputStream(IOutputStream& out) 
            : output(out), buffer(0), bitCount(0) {}
            
        ~BitOutputStream() {
            flush();
        }
        
        // запись одного бита
        void writeBit(bool bit) {
            buffer = (buffer << 1) | (bit ? 1 : 0);
            bitCount++;
            
            // когда накопили 8 бит, записываем байт в выходной поток
            if (bitCount == 8) {
                output.Write(buffer);
                buffer = 0;
                bitCount = 0;
            }
        }
        
        // запись целого байта
        void writeByte(byte value) {
            if (bitCount == 0) {
                // если буфер пуст, пишем байт напрямую
                output.Write(value);
                return;
            }
            
            // иначе побитово добавляем в буфер
            for (int i = 7; i >= 0; i--) {
                writeBit((value >> i) & 1);
            }
        }
        
        // сброс буфера
        void flush() {
            if (bitCount > 0) {
                buffer <<= (8 - bitCount);
                output.Write(buffer);
                buffer = 0;
                bitCount = 0;
            }
        }
        
        int getBitCount() const {
            return bitCount;
        }
    };
    
    // класс для побитового чтения из потока байтов
    class BitInputStream {
    private:
        IInputStream& input;
        byte buffer; // буфер для хранения текущего байта
        int bitCount; // количество оставшихся битов в буфере
        bool eof; // флаг конца потока
        
    public:
        explicit BitInputStream(IInputStream& in) 
            : input(in), buffer(0), bitCount(0), eof(false) {}
            
        // чтение одного бита
        bool readBit(bool& bit) {
            if (bitCount == 0) {
                // если буфер пуст, читаем новый байт
                if (!input.Read(buffer)) {
                    eof = true;
                    return false;
                }
                bitCount = 8;
            }
            
            // извлекаем старший бит из буфера
            bit = (buffer >> (bitCount - 1)) & 1;
            bitCount--;
            return true;
        }
        
        // чтение целого байта
        bool readByte(byte& value) {
            value = 0;
            bool bit;
            
            // читаем 8 бит и собираем их в байт
            for (int i = 7; i >= 0; i--) {
                if (!readBit(bit)) return false;
                if (bit) value |= (static_cast<byte>(1) << i);
            }
            
            return true;
        }
        
        bool isEof() const {
            return eof && bitCount == 0;
        }
    };
    
    // построение дерева хаффмана
    std::shared_ptr<HuffmanNode> buildHuffmanTree(const size_t frequencies[256]) {
        std::priority_queue<std::shared_ptr<HuffmanNode>, 
                          std::vector<std::shared_ptr<HuffmanNode>>, 
                          NodeCompare> pq;
        
        // добавляем в очередь все символы с ненулевой частотой
        for (int i = 0; i < 256; i++) {
            if (frequencies[i] > 0) {
                pq.push(std::make_shared<HuffmanNode>(static_cast<byte>(i), frequencies[i]));
            }
        }
        
        if (pq.empty()) return nullptr;
        
        // обработка случая с одним уникальным символом
        if (pq.size() == 1) {
            auto node = pq.top();
            pq.pop();
            // создаем фиктивный узел для корректного построения кодов
            auto dummy = std::make_shared<HuffmanNode>(0, 0);
            return std::make_shared<HuffmanNode>(node, dummy);
        }
        
        // объединяем узлы с наименьшими частотами
        while (pq.size() > 1) {
            auto left = pq.top(); pq.pop();
            auto right = pq.top(); pq.pop();
            auto parent = std::make_shared<HuffmanNode>(left, right);
            pq.push(parent);
        }
        
        // возвращаем корень дерева
        return pq.top();
    }
    
    // рекурсивная генерация кодов хаффмана для каждого символа
    void generateCodes(const std::shared_ptr<HuffmanNode>& node, 
                      std::vector<bool>& code, 
                      std::map<byte, std::vector<bool>>& codes) {
        if (!node) return;
        
        // если достигли листа, сохраняем код
        if (node->isLeaf()) {
            if (node->frequency > 0) {
                codes[node->symbol] = code;
            }
            return;
        }
        
        // левый ребенок - добавляем 0 к коду
        code.push_back(false);
        generateCodes(node->left, code, codes);
        code.pop_back();
        
        // правый ребенок - добавляем 1 к коду
        code.push_back(true);
        generateCodes(node->right, code, codes);
        code.pop_back();
    }
    
    // сериализация дерева в компактный битовый формат
    void serializeTree(const std::shared_ptr<HuffmanNode>& node, BitOutputStream& out) {
        if (!node) return;
        
        if (node->isLeaf()) {
            // маркер листа (1 бит) + символ (8 бит)
            out.writeBit(true);
            out.writeByte(node->symbol);
        } else {
            // маркер внутреннего узла (1 бит)
            out.writeBit(false);
            serializeTree(node->left, out);
            serializeTree(node->right, out);
        }
    }
    
    // десериализация дерева из компактного битового формата
    std::shared_ptr<HuffmanNode> deserializeTree(BitInputStream& in) {
        bool bit;
        if (!in.readBit(bit)) return nullptr;
        
        if (bit) {
            // читаем символ
            byte symbol;
            if (!in.readByte(symbol)) return nullptr;
            return std::make_shared<HuffmanNode>(symbol, 1);
        } else {
            // рекурсивно читаем детей
            auto left = deserializeTree(in);
            auto right = deserializeTree(in);
            if (!left || !right) return nullptr;
            return std::make_shared<HuffmanNode>(left, right);
        }
    }
}

// основная функция кодирования
void Encode(IInputStream& original, IOutputStream& compressed) {
    // чтение исходных данных и подсчет частот символов
    std::vector<byte> data;
    byte value;
    size_t frequencies[256] = {0};
    
    while (original.Read(value)) {
        data.push_back(value);
        frequencies[static_cast<unsigned char>(value)]++;
    }
    
    size_t totalSize = data.size();
    if (totalSize == 0) { // пустой файл
        return;
    }
    
    // построение дерева хаффмана
    auto root = buildHuffmanTree(frequencies);
    if (!root) return;
    
    // генерация кодов для каждого символа
    std::map<byte, std::vector<bool>> codes;
    std::vector<bool> code;
    generateCodes(root, code, codes);
    
    // запись сжатых данных
    BitOutputStream bitOut(compressed);
    
    // запись размера исходных данных
    for (int i = 24; i >= 0; i -= 8) {
        bitOut.writeByte(static_cast<byte>((totalSize >> i) & 0xFF));
    }
    
    // запись дерева хаффмана
    serializeTree(root, bitOut);
    
    // запись закодированных данных
    for (byte sym : data) {
        const auto& code = codes[sym];
        for (bool bit : code) {
            bitOut.writeBit(bit);
        }
    }
    
    // сброс буфера
    bitOut.flush();
}

// основная функция декодирования
void Decode(IInputStream& compressed, IOutputStream& original) {
    BitInputStream bitIn(compressed);
    
    // чтение размера исходных данных
    size_t totalSize = 0;
    byte sizeByte;
    
    for (int i = 0; i < 4; i++) {
        if (!bitIn.readByte(sizeByte)) { // не удалось прочитать размер
            return;
        }
        totalSize = (totalSize << 8) | static_cast<unsigned char>(sizeByte);
    }
    
    if (totalSize == 0) { // пустой файл
        return;
    }
    
    // восстановление дерева хаффмана
    auto root = deserializeTree(bitIn);
    if (!root) return;
    
    // подготовка к декодированию
    size_t decodedSize = 0;
    auto currentNode = root;
    bool bit;
    
    // специальная обработка случая с одним символом
    if (root->isLeaf() || (root->left && root->left->isLeaf() && root->right && root->right->frequency == 0)) {
        byte symbol = root->left ? root->left->symbol : root->symbol;
        for (size_t i = 0; i < totalSize; i++) {
            original.Write(symbol);
        }
        return;
    }
    
    // нормальное декодирование: проходим по дереву в зависимости от битов
    while (decodedSize < totalSize) {
        if (!bitIn.readBit(bit)) { // конец данных
            break;
        }
        
        // 0 - влево, 1 - вправо
        currentNode = bit ? currentNode->right : currentNode->left;
        
        // если достигли листа, записываем символ и возвращаемся к корню
        if (currentNode && currentNode->isLeaf()) {
            original.Write(currentNode->symbol);
            decodedSize++;
            currentNode = root;
        }
    }
}