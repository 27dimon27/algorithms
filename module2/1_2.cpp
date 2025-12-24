#include <iostream>
#include <vector>
#include <cmath>

// 1_2. Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв.
// Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера. Начальный размер таблицы должен быть равным 8-ми.
// Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
// Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.
// Для разрешения коллизий используйте двойное хеширование.

class HashTable {
private:
    // структура для хранения элемента таблицы
    struct HashNode {
        std::string value; // хранимая строка
        bool is_occupied; // была ли ячейка когда-то занята
        bool is_deleted; // является ли ячейка удалённой
        
        HashNode() : is_occupied(false), is_deleted(false) {} // конструктор структуры для хранения
        // деструктор, конструктор копирования и оператор присваивания не требуется прописывать явно,
        // т.к. структура не управляет ресурсами напрямую
    };
    
    std::vector<HashNode> table; // хеш-таблица
    size_t size; // размер хеш-таблицы
    size_t capacity; // вместимость хеш-таблицы
    size_t deleted_count; // кол-во удалённых элементов
    const double fill_factor = 0.75; // константный коэффициент, при котором происходит перехеширование таблицы
    
    // первая хеш-функция (метод Горнера)
    size_t hash1(const std::string& key) const {
        const int p = 31; // простое число
        size_t hash = 0;
        for (const char& c : key) { // посимвольно высчитываем хэш
            hash = hash * p + c;
        }
        return hash % capacity;
    }
    
    // вторая хеш-функция для двойного хеширования
    size_t hash2(const std::string& key) const {
        const int p = 37; // другое простое число
        size_t hash = 0;
        for (const char& c : key) { // посимвольно высчитываем хэш
            hash = hash * p + c;
        }
        return (hash * 2 + 1) % capacity;
    }
    
    // поиск индекса для вставки, поиска и удаления
    size_t find_index(const std::string& key, bool for_insert = false) const {
        size_t h1 = hash1(key); // считаем первый хеш
        size_t h2 = hash2(key); // считаем второй хеш
        
        size_t index = h1; // дефолтный индекс
        size_t first_deleted = capacity; // индекс первой удаленной ячейки (невалидный индекс по умолчанию)
        
        for (size_t i = 0; i < capacity; ++i) {
            index = (h1 + i * h2) % capacity; // считаем индекс

            if (!table[index].is_occupied) { // если нашли ещё никогда не занятую ячейку
                if (for_insert && first_deleted < capacity) { // если операция вставки и уже находили удалённую ячейку
                    return first_deleted; // возвращаем первую удаленную ячейку
                }
                return index; // иначе возвращаем текущую пустую ячейку
            }
            
            if (table[index].is_occupied && !table[index].is_deleted && 
                table[index].value == key) { // если ячейка хотя бы раз была занята, не удалена сейчас и значение совпадает,
                return index; // то возвращаем найденный элемент
            }
            
            // Запоминаем первую удаленную ячейку
            if (for_insert && table[index].is_deleted && first_deleted == capacity) { // если операция вставки, ячейка удалена и ещё не находили валидный индекс,
                first_deleted = index; // то записываем индекс первой удалённой ячейки
            }
        }

        if (for_insert && first_deleted < capacity) {
            return first_deleted;
        }
        
        return capacity; // прошли все ячейки и не нашли подходящей
    }
    
    // перехеширование таблицы
    void rehash() {
        // создаём копии таблицы и вместимости
        std::vector<HashNode> old_table = table;
        size_t old_capacity = capacity;
        
        if (size > deleted_count) { // если кол-во активных элементов больше, чем кол-во удалённых,
            capacity *= 2; // то увеличиваем вместимость в 2 раза
        }
        table.clear(); // очищаем таблицу
        table.resize(capacity); // делаем ресайз
        size = 0; // сбрасываем размер
        deleted_count = 0; // сбрасываем кол-во удалённых элементов
        
        // перемещаем все активные элементы в новую таблицу
        for (size_t i = 0; i < old_capacity; ++i) {
            if (old_table[i].is_occupied && !old_table[i].is_deleted) { // если ячейка хоть раз была занята и сейчас не удалена,
                insert(old_table[i].value); // то вставляем ячейку в новую таблицу
            }
        }
    }
    
public:
    HashTable() : size(0), deleted_count(0), capacity(8) { // конструктор, вместимость по умолчанию - 8
        table.resize(capacity);
    }
    
    // операция вставки элемента
    bool insert(const std::string& key) {
        if (static_cast<double>(size) / static_cast<double>(capacity) >= fill_factor) { // если коэффициент заполнения >= установленной константы,
            rehash(); // то делаем перехеширование
        }
        
        size_t index = find_index(key, true); // считаем индекс для вставки
        if (table[index].is_occupied && 
            !table[index].is_deleted && table[index].value == key) { // если ячейка уже содержит этот ключ и не удалена,
            return false; // то элемент уже существует -> ничего не делаем и выходим
        }

        if (table[index].is_occupied && table[index].is_deleted) { // если вставляем в удалённую ячейку,
            deleted_count--; // то уменьшаем счётчик удалённых
        }
        
        // если всё ок
        table[index].value = key; // вставляем значение в найденную ячейку
        table[index].is_occupied = true; // обозначаем ячейку как однажды занятую (по факту меняет флаг только при первом использовании ячейки)
        table[index].is_deleted = false; // обозначаем ячейку как неудалённую
        size++; // увеличиваем размер таблицы
        return true; // всё прошло успешно
    }
    
    // операция поиска элемента
    bool contains(const std::string& key) const {
        size_t index = find_index(key); // считаем индекс для поиска
        return (index < capacity && table[index].is_occupied && // проверяем, что индекс валидный, ячейка была хоть раз занята, ячейка сейчас не удалена и значение совпадает
                !table[index].is_deleted && table[index].value == key); // возвращаем bool-значение проверки
    }
    
    // операция удаления элемента
    bool remove(const std::string& key) {
        size_t index = find_index(key); // считаем индекс для удаления
        if (index < capacity && table[index].is_occupied && // если индекс валидный, ячейка была хоть раз занята, ячейка сейчас не удалена и значение совпадает,
            !table[index].is_deleted && table[index].value == key) {
            table[index].is_deleted = true; // то помечаем ячейку как удалённую
            size--; // уменьшаем размер таблицы
            deleted_count++; // увеличиваем вол-во удалённых элементов
            return true; // всё прошло успешно
        }
        return false; // если что-то не так
    }
};

int main() {
    HashTable ht; // инициализируем хеш-таблицу
    std::string operation, value; // инициализируем строки под операции и значения
    
    while (std::cin >> operation >> value) { // пока идёт ввод команд
        if (operation == "+") { // если операция вставки
            std::cout << (ht.insert(value) ? "OK" : "FAIL") << std::endl; // выполняем вставку
        } else if (operation == "-") { // если операция удаления
            std::cout << (ht.remove(value) ? "OK" : "FAIL") << std::endl; // выполняем удаление
        } else if (operation == "?") { // если операция поиска
            std::cout << (ht.contains(value) ? "OK" : "FAIL") << std::endl; // выполняем поиск
        }
    }
    
    return 0;
}