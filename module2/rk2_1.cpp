#include <iostream>
#include <sstream>

class Tree {
private:
    struct Node {
        int value;
        Node* left;
        Node* right;
        
        Node(int val) : value(val), left(nullptr), right(nullptr) {}
    };
    
    Node* root;
    
    Node* insert(Node* node, int value) {
        if (!node) {
            return new Node(value);
        }
        
        if (value < node->value) {
            node->left = insert(node->left, value);
        } else if (value > node->value) {
            node->right = insert(node->right, value);
        }
        
        return node;
    }
    
    bool isEqual(Node* node, int firstValue) const {
        if (!node) {
            return true;
        }
        
        if (node->value != firstValue) {
            return false;
        }
        
        return isEqual(node->left, firstValue) && 
               isEqual(node->right, firstValue);
    }
    
public:
    Tree() : root(nullptr) {}
    
    void insert(int value) {
        root = insert(root, value);
    }
    
    bool isEqual() const {
        if (!root) {
            return true;
        }
        
        int firstValue = root->value;
        return isEqual(root, firstValue);
    }
};

int main() {
    Tree tree;
    
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    
    int value;
    while (iss >> value) {
        tree.insert(value);
    }
    
    bool allEqual = tree.isEqual();
    std::cout << (allEqual ? 1 : 0) << std::endl;
    
    return 0;
}