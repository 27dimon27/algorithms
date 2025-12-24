#include <iostream>
#include <stack>
#include <vector>

// 2_2. Дано число N < 10^6 и последовательность целых чисел из [−2^31..2^31] длиной N.
// Требуется построить бинарное дерево, заданное наивным порядком вставки.
// Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
// Выведите элементы в порядке pre-order (сверху вниз).
// Рекурсия запрещена.

// дефолтная функция сравнения
bool defaultCompare(const int& a, const int& b) {
    return a <= b;
};

// узел бинарного дерева
struct TreeNode {
    int key; // хранимое значение
    TreeNode* left; // указатель на левый дочерний узел
    TreeNode* right; // указатель на правый дочерний узел
    
    TreeNode(int value) : key(value), left(nullptr), right(nullptr) {} // конструктор
};

// бинарное дерево
class BinaryTree {
private:
    TreeNode* root; // указатель на корень
    bool (*comparator)(const int&, const int&); // функция-компаратор для сравнения

public:
    BinaryTree(bool (*comp)(const int&, const int&)) // конструктор
        : root(nullptr), comparator(comp) {}

    ~BinaryTree() { // деструктор
        if (!root) return; // если нет корня, то чистить нечего
        
        std::stack<TreeNode*> stack; // создаём стек для очистки
        stack.push(root); // пушим корень
        
        while (!stack.empty()) { // пока стек не пустой
            TreeNode* current = stack.top(); // получаем верхний элемент стека
            stack.pop(); // удаляем верхний элемент стека
            
            if (current->left) { // если есть левый дочерний узел,
                stack.push(current->left); // то пушим его в стек для удаления
            }
            if (current->right) { // если есть правый дочерний узел,
                stack.push(current->right); // то пушим его в стек для удаления
            }
            
            delete current; // очищаем память текущего узла
        }
        
        root = nullptr; // сбрасываем корневой указатель
    }
    
    // вставка элемента
    void insert(int value) {
        TreeNode* new_node = new TreeNode(value); // создаём новый узел
        
        if (!root) { // если нет корня,
            root = new_node; // то новый узел становится корнем
            return;
        }
        
        TreeNode* current = root; // запоминаем текущий узел
        TreeNode* parent = nullptr; // запоминаем родительский узел
        
        while (current) { // пока текущий узел не пустой
            parent = current; // переносим указатель на родительский узел
            if (comparator(current->key, value)) { // если значение нового узла больше или равно значению текущего узла,
                current = current->right; // то перемещаем указатель на текущий узел в правый дочерний узел
            } else { // если значение нового узла меньше значения текущего узла,
                current = current->left; // то перемещаем указатель на текущий узел в левый дочерний узел
            }
        }
        
        // нашли место для вставки
        if (comparator(parent->key, value)) { // если значение нового узла больше или равно значению родительского узла,
            parent->right = new_node; // то вставляем новый узел в правый дочерний узел
        } else { // если значение нового узла меньше значения родительского узла,
            parent->left = new_node; // то вставляем новый узел в левый дочерний узел
        }
    }
    
    // pre-order обход
    std::vector<int> preOrder() const {
        std::vector<int> result; // создаём вектор под результат
        if (!root) return result; // если нет корня, то возвращаем пустой результат
        
        std::stack<TreeNode*> stack; // создаём стек для обхода сверху вниз
        stack.push(root); // пушим корень
        
        while (!stack.empty()) { // пока стек не пустой
            TreeNode* current = stack.top(); // получаем верхний элемент стека
            stack.pop(); // удаляем верхний элемент стека
            
            result.push_back(current->key); // пушим в вектор значение текущего узла
            
            // сначала обрабатываем правый дочерний узел, потом обрабатываем левый дочерний узел
            if (current->right) { // если правый дочерний узел есть,
                stack.push(current->right); // то пушим в стек
            }
            if (current->left) { // если левый дочерний узел есть,
                stack.push(current->left); // то пугим в стек
            }
        }
        
        return result; // возвращаем результат
    }
};

int main() {
    int N;
    std::cin >> N; // считываем кол-во чисел
    
    BinaryTree tree(defaultCompare); // инициализируем дерево
    
    int num;
    for (int i = 0; i < N; ++i) {
        std::cin >> num; // считываем число
        tree.insert(num); // вставляем число в дерево
    }
    
    std::vector<int> pre_order = tree.preOrder(); // получаем результат в порядке pre-order
    for (size_t i = 0; i < pre_order.size(); ++i) {
        std::cout << pre_order[i] << " "; // выводим результат
    }
    
    return 0;
}