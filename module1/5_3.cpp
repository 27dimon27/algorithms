#include <iostream>

// 5_3. На числовой прямой окрасили N отрезков. Известны координаты левого и правого концов каждого отрезка (Li и Ri).
// Найти длину окрашенной части числовой прямой.

// структура отрезка
struct Segment {
    int left;
    int right;
};

// функция сравнения отрезков по левой координате
int compareSegments(const Segment& a, const Segment& b) {
    if (a.left < b.left) return -1; // если левая граница первого отрезка меньше, то возвращаем -1
    if (a.left > b.left) return 1; // если левая граница первого отрезка больше, то возвращаем 1
    return 0; // если левые границы отрезков равны, то возвращаем 0
}

// сортировка слиянием
template <typename T>
void mergeSort(T* arr, int left, int right, int (*compare)(const T&, const T&)) {
    // если левая граница больше или равна правой, то выходим
    if (left >= right) return;
    
    // считаем середину
    int mid = left + (right - left) / 2;
    
    // рекурсивно сортируем левую и правую половины
    mergeSort(arr, left, mid, compare);
    mergeSort(arr, mid + 1, right, compare);
    
    // считаем размеры обеих частей
    int leftSize = mid - left + 1;
    int rightSize = right - mid;
    
    // создаем временные массивы
    T* leftArr = new T[leftSize];
    T* rightArr = new T[rightSize];
    
    // копируем данные во временные массивы
    for (int i = 0; i < leftSize; i++) {
        leftArr[i] = arr[left + i];
    }
    for (int j = 0; j < rightSize; j++) {
        rightArr[j] = arr[mid + 1 + j];
    }
    
    // сливаем временные массивы в основной массив
    int i = 0, j = 0, k = left;
    while (i < leftSize && j < rightSize) {
        if (compare(leftArr[i], rightArr[j]) <= 0) { // если левая граница отрезка из первого массива меньше, то добавляем его
            arr[k] = leftArr[i];
            i++;
        } else { // если левая граница отрезка из первого массива больше, то добавляем его
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }
    
    // если в левом массиве остались элементы, то переносим их
    while (i < leftSize) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }
    
    // если в правом массиве остались элементы, то переносим их
    while (j < rightSize) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
    
    // чистим память временных массивов
    delete[] leftArr;
    delete[] rightArr;
}

// функция вычисления длины окрашенной части
int calculateLength(Segment* segments, int n) {
    if (n == 0) return 0; // если нет отрезков, то ничего не окрашено
    
    // сортируем отрезки по левой координате
    mergeSort(segments, 0, n - 1, compareSegments);
    
    int totalLength = 0;
    int currentLeft = segments[0].left; // берём левую границу первого отрезка
    int currentRight = segments[0].right; // берём правую границу первого отрезка
    
    // объединяем пересекающиеся отрезки
    for (int i = 1; i < n; i++) {
        if (segments[i].left <= currentRight) { // если отрезки пересекаются или соприкасаются
            if (segments[i].right > currentRight) { // если правая граница нового отрезка больше правой границы текущего отрезка
                currentRight = segments[i].right; // обновляем правую границу
            }
        } else { // если отрезки не пересекаются и не соприкасаются
            totalLength += (currentRight - currentLeft); // добавляем длину текущего отрезка
            currentLeft = segments[i].left; // меняем текущую левую границу на левую границу нового отрезка
            currentRight = segments[i].right; // меняем текущую правую границу на правую границу нового отрезка
        }
    }
    
    // добавляем длину последнего отрезка
    totalLength += (currentRight - currentLeft);
    
    return totalLength;
}

int main() {
    int n;
    std::cin >> n; // считываем кол-во отрезков
    
    // выделяем память для массива отрезков
    Segment* segments = new Segment[n];
    
    for (int i = 0; i < n; i++) {
        std::cin >> segments[i].left >> segments[i].right; // считываем координаты отрезков
    }
    
    // вычисляем длину окрашенной части
    int result = calculateLength(segments, n);
    
    // выводим результат
    std::cout << result << std::endl;
    
    // чистим память выделенного под отрезки массива
    delete[] segments;
    
    return 0;
}