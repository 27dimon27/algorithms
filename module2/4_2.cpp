#include <iostream>

// Дано число N и N строк.
// Каждая строка содержащит команду добавления или удаления натуральных чисел, а также запрос на получение k-ой порядковой статистики.
// Команда добавления числа A задается положительным числом A, команда удаления числа A задается отрицательным числом “-A”.
// Запрос на получение k-ой порядковой статистики задается числом k.
// Требуемая скорость выполнения запроса - O(log n).

class AVLTree {
private:
    struct Node {
        int key; // значение узла
        int height; // высота поддерева с корнем в данном узле
        int size; // кол-во узлов в поддереве
        Node* left; // левый ребёнок
        Node* right; // правый ребёнок
        
        Node(int k) : key(k), height(1), size(1), left(nullptr), right(nullptr) {}
    };
    
    Node* root;
    
    // функция получения высоты узла
    int getHeight(Node* node) {
        return node ? node->height : 0;
    }
    
    // функция получения размера поддерева
    int getSize(Node* node) {
        return node ? node->size : 0;
    }
    
    // функция обновления узла
    void updateNode(Node* node) {
        if (node) {
            node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
            node->size = getSize(node->left) + getSize(node->right) + 1;
        }
    }
    
    // функция получения баланса узла
    int getBalance(Node* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }
    
    // правый поворот
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        
        x->right = y;
        y->left = T2;
        
        updateNode(y);
        updateNode(x);
        
        return x;
    }
    
    // левый поворот
    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        
        y->left = x;
        x->right = T2;
        
        updateNode(x);
        updateNode(y);
        
        return y;
    }
    
    // функция балансировки узла
    Node* balanceNode(Node* root) {
        updateNode(root);
        
        int balance = getBalance(root);
        
        // случай Left Left
        if (balance > 1 && getBalance(root->left) >= 0) {
            return rotateRight(root);
        }
        
        // случай Left Right
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }
        
        // случай Right Right
        if (balance < -1 && getBalance(root->right) <= 0) {
            return rotateLeft(root);
        }
        
        // случай Right Left
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }
        
        return root;
    }
    
    // функция вставки узла
    Node* insert(Node* root, int key) {
        if (!root) { // если нет корня,
            return new Node(key); // то создаём корень
        }
        
        if (key < root->key) { // если значение меньше, чем текущее,
            root->left = insert(root->left, key); // то рекурсивно записываем в левое поддерево
        } else if (key > root->key) { // если значение больше, чем текущее,
            root->right = insert(root->right, key); // то рекурсивно записываем в правое поддерево
        } else {
            return root; // если ключ уже существует, просто возвращаем узел
        }
        
        return balanceNode(root); // балансируем дерево на обратном пути рекурсии
    }

    // функция для нахождения минимального значения и удаления узла
    std::pair<Node*, int> findAndRemoveMin(Node* root) {
        if (!root->left) {
            int minValue = root->key;
            Node* rightChild = root->right;
            delete root;
            return {rightChild, minValue};
        }
        
        auto [newLeft, minValue] = findAndRemoveMin(root->left);
        root->left = newLeft;
        
        return {balanceNode(root), minValue};
    }
    
    // функция удаления узла
    Node* remove(Node* root, int key) {
        if (!root) {
            return nullptr;
        }
        
        if (key < root->key) { // если меньше,
            root->left = remove(root->left, key); // то идём влево
        } else if (key > root->key) { // если больше,
            root->right = remove(root->right, key); // то идём вправо
        } else { // если нашли
            if (!root->left || !root->right) { // есть один ребёнок или нет детей
                Node* temp = root->left ? root->left : root->right;
                delete root;
                return temp;
            } else { // есть оба ребёнка
                if (!root->right->left) { // специальный случай: правый ребенок сам минимальный
                    Node* temp = root->right;
                    root->key = temp->key;
                    root->right = temp->right;
                    delete temp;
                } else {
                    auto [newRight, minValue] = findAndRemoveMin(root->right); // находим минимум и удаляем узел
                    root->key = minValue;
                    root->right = newRight;
                }
            }
        }
        
        return balanceNode(root); // балансируем дерево
    }
    
    // функция поиска k-го элемента
    int findKth(Node* root, int k) {
        if (!root) { // если ошибки,
            return -1; // возвращаем недопустимое значение
        }
        
        int leftSize = getSize(root->left); // кол-во узлов в левом поддереве
        
        if (k < leftSize) {
            return findKth(root->left, k); // k-ый элемент находится в левом поддереве
        } else if (k == leftSize) {
            return root->key; // текущий узел == k-ый элемент
        } else {
            return findKth(root->right, k - leftSize - 1); // k-ый узел в правом поддереве
        }
    }
    
    // функция очистки дерева
    void deleteTree(Node* root) {
        if (root) {
            deleteTree(root->left);
            deleteTree(root->right);
            delete root;
        }
    }
    
public:
    AVLTree() : root(nullptr) {}
    
    ~AVLTree() {
        deleteTree(root);
    }
    
    // добавление элемента
    void add(int key) {
        root = insert(root, key);
    }
    
    // удаление элемента
    void remove(int key) {
        root = remove(root, key);
    }
    
    // поиск k-й порядковой статистики
    int findKth(int k) {
        if (k < 0 || k >= getSize(root)) {
            return -1; // Неверный индекс
        }
        return findKth(root, k);
    }
};

int main() {
    int N;
    std::cin >> N;
    
    AVLTree tree;
    
    for (int i = 0; i < N; i++) {
        int value, k;
        std::cin >> value >> k;
        
        if (value > 0) {
            tree.add(value);
        } else if (value < 0) {
            tree.remove(-value);
        }

        std::cout << tree.findKth(k) << "\n";
    }
    
    return 0;
}