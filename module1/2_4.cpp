#include <iostream>

// 2_4. Дан отсортированный массив различных целых чисел A[0..n-1] и массив целых чисел B[0..m-1].
// Для каждого элемента массива B[i] найдите минимальный индекс элемента массива A[k], ближайшего по значению к B[i].
// Требования: Время работы поиска для каждого элемента B[i]: O(log(k)).
// Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон для бинарного поиска размером порядка k с помощью экспоненциального поиска, а потом уже в нем делать бинарный поиск.

// Экспоненциальный поиск для определения границ бинарного поиска
void findBorders(const int A[], int n, int target, int& left, int& right) {    
    // Если target меньше первого элемента
    if (target <= A[0]) {
        left = right = 0;
        return;
    }
    
    // Если target больше последнего элемента
    if (target >= A[n - 1]) {
        left = right = n - 1;
        return;
    }
    
    // Экспоненциальный поиск для нахождения верхней границы
    int bound = 1;
    while (bound < n && A[bound] < target) {
        bound *= 2;
    }
    
    left = bound / 2; // Записываем нижнюю границу
    right = (bound < n) ? bound : n - 1; // Записываем верхнюю границу
}

// Бинарный поиск в найденном диапазоне
int binarySearch(const int A[], int left, int right, int target) {
    while (left < right - 1) { // Пока правая граница не наползла на левую или границы не оказались соседями
        int mid = (left + right) / 2; // Считаем центральный элемент
        
        if (A[mid] == target) { // Если нашли точное совпадение
            return mid;
        } else if (A[mid] < target) { // Если цель больше текущего элемента
            left = mid; // Двигаем левую границу
        } else { // Если цель меньше текущего элемента
            right = mid; // Двигаем правую границу
        }
    }
    
    // Если точного совпадения нет, ищем ближайший элемент
    // Проверяем граничные случаи
    if (right <= 0) return right;
    if (left >= right) return left;
        
    // Считаем расстояния до левого и правого элементов
    int left_dist = std::abs(target - A[right]);
    int right_dist = std::abs(target - A[left]);

    if (left_dist < right_dist) { // Выбираем ближайший элемент
        return right;
    } else {
        return left;
    }
}

// Функция поиска ближайшего элемента
int findClosestElement(const int A[], int n, int target) {
    if (n == 0) return -1; // Если массив A пустой, то возвращаем -1
    if (n == 1) return 0; // Если в массиве A только 1 элемент, то возвращаем его (нулевой) индекс
    
    int left, right; // Инициализация границ поиска
    findBorders(A, n, target, left, right); // Запускаем экспоненциальный поиск для поиска границ бинарного поиска
    
    return binarySearch(A, left, right, target); // Запускаем бинарный поиск с найденными границами
}

int main() {
    int n, m;
    
    std::cin >> n; // Чтение размера массива A
    int* A = new int[n]; // Инициализация массива A
    for (int i = 0; i < n; i++) {
        std::cin >> A[i]; // Поочерёдное чтение элементов массива A
    }
    
    std::cin >> m; // Чтение размера массива B
    int* B = new int[m]; // Инициализация массива B
    for (int i = 0; i < m; i++) {
        std::cin >> B[i]; // Поочерёдное чтение элементов массива B
    }
    
    // Поиск ближайших элементов для каждого B[i]
    for (int i = 0; i < m; i++) {
        int closest = findClosestElement(A, n, B[i]);
        std::cout << closest << " ";
    }
    std::cout << std::endl;
    
    // Очистка памяти
    delete[] A;
    delete[] B;
    
    return 0;
}