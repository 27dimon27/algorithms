#include <iostream>
#include <sstream>
#include <queue>

struct Node {
    int val;
    Node* left;
    Node* right;
    
    Node(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Tree {
private:
    Node* root;
    
    Node* insert(Node* node, int val) {
        if (node == nullptr) {
            return new Node(val);
        }
        
        if (val < node->val) {
            node->left = insert(node->left, val);
        } else {
            node->right = insert(node->right, val);
        }
        
        return node;
    }
    
    int minDepth(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        
        if (node->left == nullptr) {
            return 1 + minDepth(node->right);
        }
        
        if (node->right == nullptr) {
            return 1 + minDepth(node->left);
        }
        
        return 1 + std::min(minDepth(node->left), minDepth(node->right));
    }
    
    void clear(Node* node) {
        if (node == nullptr) {
            return;
        }
        
        clear(node->left);
        clear(node->right);
        delete node;
    }
    
public:
    Tree() : root(nullptr) {}
    
    ~Tree() {
        clear(root);
    }
    
    void insert(int val) {
        root = insert(root, val);
    }
    
    int getMinDepth() {
        return minDepth(root);
    }
};

int main() {
    Tree tree;
    
    std::string line;
    std::getline(std::cin, line);
    std::stringstream ss(line);
    
    int value;
    while (ss >> value) {
        tree.insert(value);
    }
    
    std::cout << tree.getMinDepth() << std::endl;
    
    return 0;
}