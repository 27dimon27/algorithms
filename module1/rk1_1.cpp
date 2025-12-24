#include <iostream>

class Vector {
private:
    int* data;
    size_t size;
    size_t capacity;
    
    Vector(const Vector&) = delete;
    Vector& operator=(const Vector&) = delete;
    
public:
    Vector() : data(nullptr), size(0), capacity(0) {}
    
    ~Vector() {
        delete[] data;
    }
    
    void push_back(int value) {
        if (size >= capacity) {
            resize();
        }
        data[size++] = value;
    }
    
    int& operator[](size_t index) {
        return data[index];
    }
    
    const int& operator[](size_t index) const {
        return data[index];
    }
    
    size_t getSize() const {
        return size;
    }
    
private:
    void resize() {
        size_t new_capacity = (capacity == 0) ? 1 : capacity * 2;
        int* new_data = new int[new_capacity];
        
        for (size_t i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }
        
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }
};

int searchPosition(const Vector& A, int k) {
    int left = 0;
    int right = A.getSize() - 1;
    
    while (left <= right) {
        int mid = (left + right) / 2;
        
        if (A[mid] == k) {
            return mid;
        } else if (A[mid] < k) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return left;
}

int main() {
    int n;
    std::cin >> n;
    
    Vector A;
    for (int i = 0; i < n; ++i) {
        int value;
        std::cin >> value;
        A.push_back(value);
    }
    
    int k;
    std::cin >> k;
    
    int result = searchPosition(A, k);
    std::cout << result << std::endl;
    
    return 0;
}