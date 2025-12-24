#include <iostream>
#include <vector>
#include <queue>
#include <sstream>
#include <functional>

// 3_2. Постройте B-дерево минимального порядка t и выведите его по слоям.
// В качестве ключа используются числа, лежащие в диапазоне [0..2^32 − 1]
// Требования:
// B-дерево должно быть реализовано в виде шаблонного класса.
// Решение должно поддерживать передачу функции сравнения снаружи.

// дефолтная функция сравнения
template<typename T>
bool comparator(const T& a, const T& b) {
    return a < b;
}

// шаблонный класс B-дерева
template<typename T>
class BTree {
private:
    struct Node { 
        bool isLeaf; // является ли узел листом
        std::vector<T> keys; // массив ключей
        std::vector<Node*> children; // множество детей

        Node(bool _isLeaf) : isLeaf(_isLeaf) {}
        ~Node() { for (auto* child : children) { delete child; } }
    };

    Node* root; // указатель на корень
    size_t t; // минимальный порядок
    std::function<bool(const T&, const T&)> compare; // функция сравнения

    void splitChild(Node* parent, int index) { // функция разделения переполненного узла
        Node* child = parent->children[index];
        Node* new_child = new Node(child->isLeaf);
        
        // переносим ключи
        for (size_t i = 0; i < t - 1; ++i) {
            new_child->keys.push_back(child->keys[t + i]);
        }
        
        // переносим детей, если не лист
        if (!child->isLeaf) {
            for (size_t i = 0; i < t; ++i) {
                new_child->children.push_back(child->children[t + i]);
            }
            child->children.resize(t);
        }
        
        // поднимаем средний ключ
        T middle_key = child->keys[t - 1];
        parent->keys.insert(parent->keys.begin() + index, middle_key);
        parent->children.insert(parent->children.begin() + index + 1, new_child);
        
        // обрезаем ключи исходного ребенка
        child->keys.resize(t - 1);
    }

    void insertNonFull(Node* node, const T& key) { // вставить в неполный узел
        int i = node->keys.size() - 1;
        
        if (node->isLeaf) {
            // вставляем в лист
            node->keys.push_back(T());
            while (i >= 0 && compare(key, node->keys[i])) {
                node->keys[i + 1] = node->keys[i];
                --i;
            }
            node->keys[i + 1] = key;
        } else {
            // находим ребенка для вставки
            while (i >= 0 && compare(key, node->keys[i])) {
                --i;
            }
            ++i;
            
            // если ребенок полный, разделяем его
            if (node->children[i]->keys.size() == 2 * t - 1) {
                splitChild(node, i);
                if (compare(node->keys[i], key)) {
                    ++i;
                }
            }
            insertNonFull(node->children[i], key);
        }
    }

public:
    BTree(size_t min_order, std::function<bool(const T&, const T&)> comp = comparator<T>)
        : root(nullptr), t(min_order), compare(comp) {}
    ~BTree() { delete root; }
    
    void insert(const T& key) { // функция вставки в дерево
        if (root == nullptr) {
            root = new Node(true);
            root->keys.push_back(key);
            return;
        }
        
        // если корень полный, создаем новый корень
        if (root->keys.size() == 2 * t - 1) {
            Node* new_root = new Node(false);
            new_root->children.push_back(root);
            root = new_root;
            splitChild(root, 0);
        }
        
        insertNonFull(root, key);
    }
    
    void printByLevels() const { // функция вывода по слоям
        if (root == nullptr) {
            return;
        }
        
        std::queue<Node*> current_level, next_level; // создаём две очереди для вывода
        current_level.push(root);
        
        while (!current_level.empty()) {
            Node* node = current_level.front();
            current_level.pop();
            
            // выводим ключи текущего узла
            for (size_t i = 0; i < node->keys.size(); ++i) {
                std::cout << node->keys[i];
                if (i < node->keys.size() - 1) {
                    std::cout << " ";
                }
            }
            
            // добавляем детей в следующую очередь
            if (!node->isLeaf) {
                for (Node* child : node->children) {
                    next_level.push(child);
                }
            }
            
            // если текущий уровень закончился, переходим к следующему
            if (current_level.empty()) {
                std::cout << std::endl;
                std::swap(current_level, next_level);
            } else {
                std::cout << " ";
            }
        }
    }
};

int main() {
    size_t t;
    std::cin >> t; // вводим число минимального порядока
    std::cin.ignore(); // игнорируем перевод строки после числа
    
    BTree<unsigned int> tree(t, comparator<unsigned int>); // инициализируем B-дерево
    
    std::string line;
    std::getline(std::cin, line); // получаем строку элементов дерева
    std::istringstream iss(line);
    
    unsigned int key;
    while (iss >> key) { // перебираем все элементы
        tree.insert(key);
    }
    
    tree.printByLevels(); // выводим по слоям
    
    return 0;
}