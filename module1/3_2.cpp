#include <iostream>

// 3_2. Реализовать дек с динамическим зацикленным буфером (на основе динамического массива).
// Требования: Дек должен быть реализован в виде класса.

class Deque {
private:
    int* buffer; // Динамический массив для хранения элементов
    int capacity; // Ёмкость буфера
    int size; // Количество элементов
    int front; // Индекс первого элемента
    int back; // Индекс последнего элемента

    // Увеличение ёмкости буфера при необходимости
    void resize() {
        int newCapacity = capacity * 2;
        int* newBuffer = new int[newCapacity]; // Выделяем память для нового буфера
        
        // Копируем элементы в новый буфер, начиная с нулевого индекса
        for (int i = 0; i < size; i++) {
            newBuffer[i] = buffer[(front + i) % capacity];
        }
        
        delete[] buffer; // Удаляем старый буфер

        // Копируем новый буфер и его параметры
        buffer = newBuffer;
        capacity = newCapacity;
        front = 0;
        back = size - 1;
    }

public:
    // Конструктор
    Deque(int initialCapacity = 5) {
        capacity = (initialCapacity > 0) ? initialCapacity : 5;
        buffer = new int[capacity];
        size = 0;
        front = 0;
        back = -1;
    }

    // Деструктор
    ~Deque() {
        delete[] buffer;
    }

    Deque(const Deque&) = delete; // Запрет на копирование
    Deque& operator=(const Deque&) = delete; // Запрет на присваивание

    // Добавление элемента в начало (1)
    void pushFront(int val) {
        if (size == capacity) {
            resize();
        }
        
        front = (front - 1 + capacity) % capacity; // Обновление индекса для вставки
        buffer[front] = val;
        size++;

        if (size == 1) {
            back = front; // Если единичный размер, то последний элемент = первый элемент
        }
    }

    // Удаление элемента из начала (2)
    int popFront() {
        if (isEmpty()) {
            return -1; // Если дек пуст, то возвращаем -1
        }
        
        int val = buffer[front];
        front = (front + 1) % capacity; // Обновление индекса первого элемента
        size--;
        
        return val;
    }

    // Добавление элемента в конец (3)
    void pushBack(int val) {
        if (size == capacity) {
            resize();
        }
        
        back = (back + 1) % capacity; // Обновление индекса для вставки
        buffer[back] = val;
        size++;

        if (size == 1) {
            front = back; // Если единичный размер, то первый элемент = последний элемент
        }
    }

    // Удаление элемента из конца (4)
    int popBack() {
        if (isEmpty()) {
            return -1; // Если дек пуст, то возвращаем -1
        }
        
        int val = buffer[back];
        back = (back - 1 + capacity) % capacity; // Обновление индекса последнего элемента
        size--;
        
        return val;
    }

    // Проверка дека на пустоту
    bool isEmpty() const {
        return size == 0;
    }
};

int main() {
    int n, a, b, val;
    bool flag = true; // Флаг совпадения ожидаемых значений
    Deque deque;

    std::cin >> n; // Чтение кол-ва команд
    for (int i = 0; i < n; i++) {
        std::cin >> a >> b; // Чтение номера метода и числа
        if (flag) { // Если ещё не нашли несовпадение
            switch (a)
            {
            case 1:
                deque.pushFront(b); // Вставка в начало
                break;
            
            case 2:
                val = deque.popFront(); // Извлечение из начала
                if (val != b) {
                    flag = false; // Если не совпадает с ожидаемым значением, то меняем флаг
                }
                break;
            
            case 3:
                deque.pushBack(b); // Вставка в конец
                break;
            
            case 4:
                val = deque.popBack(); // Извлечение из конца
                if (val != b) {
                    flag = false; // Если не совпадает с ожидаемым значением, то меняем флаг
                }
                break;
            }
        }
    }

    std::cout << (flag ? "YES" : "NO"); // Вывод результата

    return 0;
}