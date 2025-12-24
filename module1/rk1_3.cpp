#include <iostream>
#include <algorithm>

class Box3D {
private:
    int data[3];
    
public:
    Box3D() : data{0, 0, 0} {}
    Box3D(int x, int y, int z) : data{x, y, z} {}
    
    Box3D(const Box3D& other) {
        for (int i = 0; i < 3; i++) {
            data[i] = other.data[i];
        }
    }
    
    Box3D& operator=(const Box3D& other) {
        if (this != &other) {
            for (int i = 0; i < 3; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    
    int& operator[](int index) { return data[index]; }
    const int& operator[](int index) const { return data[index]; }
};

class Box {
public:
    int id;
    Box3D dimensions;
    
    Box() : id(0) {}
    Box(int id, const Box3D& dim) : id(id), dimensions(dim) {}
    
    Box(const Box& other) : id(other.id), dimensions(other.dimensions) {}
    
    Box& operator=(const Box& other) {
        if (this != &other) {
            id = other.id;
            dimensions = other.dimensions;
        }
        return *this;
    }
};

class BoxVector {
private:
    Box* data;
    int capacity;
    int size;
    
    void resize(int new_capacity) {
        Box* new_data = new Box[new_capacity];
        for (int i = 0; i < size; i++) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }
    
public:
    BoxVector() : data(new Box[10]), capacity(10), size(0) {}
    
    BoxVector(const BoxVector& other) : data(new Box[other.capacity]), capacity(other.capacity), size(other.size) {
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    BoxVector& operator=(const BoxVector& other) = delete;
    
    ~BoxVector() {
        delete[] data;
    }
    
    void push_back(const Box& box) {
        if (size >= capacity) {
            resize(capacity * 2);
        }
        data[size++] = box;
    }
    
    Box& operator[](int index) { return data[index]; }
    const Box& operator[](int index) const { return data[index]; }
    
    int getSize() const { return size; }
};

bool compareBoxes(const Box& a, const Box& b) {
    int a_dims[3] = {a.dimensions[0], a.dimensions[1], a.dimensions[2]};
    int b_dims[3] = {b.dimensions[0], b.dimensions[1], b.dimensions[2]};
    
    std::sort(a_dims, a_dims + 3);
    std::sort(b_dims, b_dims + 3);
    
    for (int i = 0; i < 3; i++) {
        if (a_dims[i] != b_dims[i]) {
            return a_dims[i] < b_dims[i];
        }
    }
    return false;
}

void insertionSort(BoxVector& boxes) {
    int n = boxes.getSize();
    for (int i = 1; i < n; i++) {
        Box key = boxes[i];
        int j = i - 1;
        
        while (j >= 0 && compareBoxes(key, boxes[j])) {
            boxes[j + 1] = boxes[j];
            j--;
        }
        boxes[j + 1] = key;
    }
}

int main() {
    int n;
    std::cin >> n;
    
    BoxVector boxes;
    
    for (int i = 0; i < n; i++) {
        int a, b, c;
        std::cin >> a >> b >> c;
        boxes.push_back(Box(i, Box3D(a, b, c)));
    }
    
    insertionSort(boxes);
    
    for (int i = 0; i < boxes.getSize(); i++) {
        std::cout << boxes[i].id;
        if (i < boxes.getSize() - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
    
    return 0;
}