#include <iostream>

// 4_2. Имеется лог-файл, в котором хранятся пары для N пользователей (Идентификатор пользователя, посещаемость сайта).
// Напишите программу, которая выбирает K пользователей, которые чаще других заходили на сайт, и выводит их в порядке возрастания посещаемости. Количество заходов и идентификаторы пользователей не повторяются.
// Требования: время работы O(N * logK), где N - кол-во пользователей. Ограничение на размер кучи O(K).

// класс кучи
template <typename T>
class Heap {
private:
    T* data; // буфер для хранения информации в куче
    int capacity; // текущая вместимость кучи
    int size; // текущий размер кучи
    bool (*comparator)(const T&, const T&); // функция-компаратор

    void resize() { // функция изменения размера буфера кучи
        capacity *= 2;
        T* new_data = new T[capacity];
        for (int i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
    }

    void sift_up(int index) { // функция просеивания вверх
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (comparator(data[index], data[parent])) {
                std::swap(data[index], data[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }

    void sift_down(int index) { // функция просеивания вниз
        while (2 * index + 1 < size) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < size && comparator(data[left], data[smallest])) {
                smallest = left;
            }
            if (right < size && comparator(data[right], data[smallest])) {
                smallest = right;
            }

            if (smallest != index) {
                std::swap(data[index], data[smallest]);
                index = smallest;
            } else {
                break;
            }
        }
    }

public:
    Heap(int initial_capacity, bool (*comp)(const T&, const T&)) // конструктор
        : capacity(initial_capacity), size(0), comparator(comp) {
        data = new T[capacity];
    }

    ~Heap() { // деструктор
        delete[] data;
    }

    Heap(const Heap&) = delete; // Запрет на копирование
    Heap& operator=(const Heap&) = delete; // Запрет на присваивание

    void push(const T& value) { // добавить значение в кучу
        if (size >= capacity) {
            resize();
        }
        data[size] = value;
        sift_up(size);
        size++;
    }

    T pop() { // извлечь значение из корня кучи
        T result = data[0];
        data[0] = data[size - 1];
        size--;
        sift_down(0);
        return result;
    }

    const T& top() const { // получить значение в корне кучи
        return data[0];
    }

    int get_size() const { // получить размер буфера кучи
        return size;
    }
};

// структура данных пользователя
struct User {
    int id;
    int visits;
};

// функция-компаратор для минимальной кучи
bool min_heap_compare(const User& a, const User& b) {
    return a.visits < b.visits;
}

int main() {
    int N, K;
    std::cin >> N >> K; // считываем числа

    Heap<User> heap(K, min_heap_compare); // cоздаем min-heap для K пользователей с наибольшей посещаемостью

    for (int i = 0; i < N; ++i) {
        User user;
        std::cin >> user.id >> user.visits; // считываем данные о каждом пользователе

        if (heap.get_size() < K) { // если в куче меньше K пользователей - просто добавляем в кучу
            heap.push(user);
        } else if (user.visits > heap.top().visits) { // иначе, если новый пользователь активнее - извлекаем корень и записываем в него новые данные
            heap.pop();
            heap.push(user);
        }
    }

    // извлекаем пользователей из кучи и сохраняем в массив
    User* result = new User[K];
    int count = heap.get_size();
    
    for (int i = 0; i < count; ++i) {
        result[i] = heap.pop();
    }

    // выводим результат
    for (int i = count - K; i < count; ++i) {
        std::cout << result[i].id;
        if (i < count - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;

    // чистим память выделенного под результат массива
    delete[] result;
    return 0;
}