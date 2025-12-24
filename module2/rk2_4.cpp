#include <iostream>
#include <string>

class AVLTree {
private:
    struct Node {
        int key;
        int height;
        int size;
        Node* left;
        Node* right;
        
        Node(int k) : key(k), height(1), size(1), left(nullptr), right(nullptr) {}
    };
    
    Node* root;
    
    int getHeight(Node* node) {
        return node ? node->height : 0;
    }
    
    int getSize(Node* node) {
        return node ? node->size : 0;
    }
    
    void updateNode(Node* node) {
        if (node) {
            node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
            node->size = getSize(node->left) + getSize(node->right) + 1;
        }
    }
    
    int getBalance(Node* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }
    
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        
        x->right = y;
        y->left = T2;
        
        updateNode(y);
        updateNode(x);
        
        return x;
    }
    
    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        
        y->left = x;
        x->right = T2;
        
        updateNode(x);
        updateNode(y);
        
        return y;
    }
    
    Node* balanceNode(Node* root) {
        updateNode(root);
        
        int balance = getBalance(root);
        
        if (balance > 1 && getBalance(root->left) >= 0) {
            return rotateRight(root);
        }
        
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }
        
        if (balance < -1 && getBalance(root->right) <= 0) {
            return rotateLeft(root);
        }
        
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }
        
        return root;
    }
    
    Node* insert(Node* root, int key) {
        if (!root) {
            return new Node(key);
        }
        
        if (key < root->key) {
            root->left = insert(root->left, key);
        } else if (key > root->key) {
            root->right = insert(root->right, key);
        } else {
            return root;
        }
        
        return balanceNode(root);
    }
    
    Node* removeMin(Node* root) {
        if (!root->left) {
            Node* rightChild = root->right;
            delete root;
            return rightChild;
        }
        root->left = removeMin(root->left);
        return balanceNode(root);
    }
    
    Node* remove(Node* root, int key) {
        if (!root) {
            return nullptr;
        }
        
        if (key < root->key) {
            root->left = remove(root->left, key);
        } else if (key > root->key) {
            root->right = remove(root->right, key);
        } else {
            if (!root->left || !root->right) {
                Node* temp = root->left ? root->left : root->right;
                delete root;
                return temp;
            } else {
                Node* minRight = root->right;
                while (minRight->left) {
                    minRight = minRight->left;
                }

                root->key = minRight->key;
                root->right = removeMin(root->right);
            }
        }
        
        return balanceNode(root);
    }
    
    bool exists(Node* root, int key) {
        if (!root) {
            return false;
        }
        
        if (key < root->key) {
            return exists(root->left, key);
        } else if (key > root->key) {
            return exists(root->right, key);
        } else {
            return true;
        }
    }
    
    bool findNext(Node* root, int key, int& result) {
        if (!root) {
            return false;
        }
        
        if (root->key <= key) {
            return findNext(root->right, key, result);
        } else {
            bool foundInLeft = findNext(root->left, key, result);
            if (foundInLeft) {
                return true;
            }
            result = root->key;
            return true;
        }
    }
    
    bool findPrev(Node* root, int key, int& result) {
        if (!root) {
            return false;
        }
        
        if (root->key >= key) {
            return findPrev(root->left, key, result);
        } else {
            bool foundInRight = findPrev(root->right, key, result);
            if (foundInRight) {
                return true;
            }
            result = root->key;
            return true;
        }
    }
    
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
    
    void insert(int key) {
        root = insert(root, key);
    }
    
    void remove(int key) {
        root = remove(root, key);
    }
    
    bool exists(int key) {
        return exists(root, key);
    }

    bool next(int key, int& result) {
        return findNext(root, key, result);
    }
    
    bool prev(int key, int& result) {
        return findPrev(root, key, result);
    }
};

int main() {
    AVLTree tree;
    std::string command;
    int x;
    
    while (std::cin >> command >> x) {
        if (command == "insert") {
            tree.insert(x);
        } else if (command == "delete") {
            tree.remove(x);
        } else if (command == "exists") {
            if (tree.exists(x)) {
                std::cout << "true\n";
            } else {
                std::cout << "false\n";
            }
        } else if (command == "next") {
            int result;
            if (tree.next(x, result)) {
                std::cout << result << "\n";
            } else {
                std::cout << "none\n";
            }
        } else if (command == "prev") {
            int result;
            if (tree.prev(x, result)) {
                std::cout << result << "\n";
            } else {
                std::cout << "none\n";
            }
        }
    }
    
    return 0;
}