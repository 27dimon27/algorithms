#include <iostream>

// 6_2. Дано множество целых чисел из [0...10^9] размера n.
// Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
// 1) 10% перцентиль
// 2) медиана
// 3) 90% перцентиль
// Реализуйте стратегию выбора опорного элемента “медиана трёх”. Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.

// функция сравнения
int compareNums(const int& a, const int& b) {
    if (a < b) return -1; // если первое число меньше второго, то возвращаем -1
    if (a > b) return 1; // если первое число больше второго, то возвращаем 1
    return 0; // если числа равны, то возвращаем 0
}

// выбор медианы трех элементов
int medianOfThree(int* arr, int left, int right, int (*compare)(const int&, const int&)) {
    int mid = left + (right - left) / 2; // считаем середину
    
    if (compare(arr[left], arr[mid]) > 0) // если левый элемент больше центрального, то меняем их местами
        std::swap(arr[left], arr[mid]);
    if (compare(arr[left], arr[right]) > 0) // если левый элемент больше правого, то меняем их местами
        std::swap(arr[left], arr[right]);
    if (compare(arr[mid], arr[right]) > 0) // если центральный элемент больше правого, то меняем их местами
        std::swap(arr[mid], arr[right]);
    
    return mid; // возвращаем индекс медианы
}

// функция partition с проходом от конца к началу
int partition(int* arr, int left, int right, int (*compare)(const int&, const int&)) {
    int pivotIndex = medianOfThree(arr, left, right, compare); // выбираем опорный элемент как медиану трёх
    std::swap(arr[pivotIndex], arr[right]); // перемещаем опорный элемент в конец
    int pivot = arr[right]; // определяем значение опорного элемента
    
    int i = right; // итератор i - конец группы элементов, бОльших опорного
    int j = right - 1; // итератор j - первый нерассмотренный элемент
    
    while (j >= left) {
        if (compare(arr[j], pivot) <= 0) { // если элемент не больше опорного, то сдвигаем j
            j--;
        } else { // если элемент больше опорного, то меняем местами и сдвигаем оба итератора
            std::swap(arr[i - 1], arr[j]);
            i--;
            j--;
        }
    }
    
    std::swap(arr[i], arr[right]); // помещаем опорный элемент на правильную позицию
    return i; // возвращаем нужный индекс
}

// поиск k-ой порядковой статистики
int findKthOrderStatistic(int* arr, int left, int right, int k, int (*compare)(const int&, const int&)) {
    while (left <= right) {
        int pivotIndex = partition(arr, left, right, compare); // считаем опорный индекс
        
        if (pivotIndex == k) { // если опорный индекс равен порядковой статистики, то возвращаем значение
            return arr[k];
        } else if (pivotIndex < k) { // если опорный индекс меньше порядковой статистики, то двигаем левую границу
            left = pivotIndex + 1;
        } else { // если опорный индекс больше порядковой статистики, то двигаем правую границу
            right = pivotIndex - 1;
        }
    }
    return arr[k];
}

int main() {
    int n;
    std::cin >> n; // считаем кол-во элементов
    
    // выделяем память под массив
    int* arr = new int[n];
    
    for (int i = 0; i < n; i++) {
        std::cin >> arr[i]; // получаем элементы массива
    }
    
    // вычисляем индексы для перцентилей
    int index10 = n / 10; // 10% перцентиль
    int index50 = n / 2; // медиана (50% перцентиль)
    int index90 = n * 9 / 10; // 90% перцентиль
    
    // находим статистики
    int percentile10 = findKthOrderStatistic(arr, 0, n - 1, index10, compareNums);
    int percentile50 = findKthOrderStatistic(arr, 0, n - 1, index50, compareNums);
    int percentile90 = findKthOrderStatistic(arr, 0, n - 1, index90, compareNums);
    
    // выводим результаты
    std::cout << percentile10 << std::endl;
    std::cout << percentile50 << std::endl;
    std::cout << percentile90 << std::endl;
    
    // чистим память выделенного под числа массива
    delete[] arr;
    
    return 0;
}