#include <iostream>
#include <unordered_set>
#include <string>

struct Node {
    std::string product_name;
    int color;
    int size;

    bool operator==(const Node& other) const noexcept {
        return product_name == other.product_name && color == other.color && size == other.size;
    }
};

namespace std {
    template<>
    struct hash<Node> {
        std::size_t operator()(const Node& node) const noexcept {
            std::hash<std::string> string_hash;
            std::hash<int> int_hash;
            std::size_t h1 = string_hash(node.product_name);
            std::size_t h2 = int_hash(node.color);
            std::size_t h3 = int_hash(node.size);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}

int main() {
    std::unordered_set<Node> set;
    char operation;
    Node node;

    while (std::cin >> operation >> node.product_name >> node.color >> node.size) {
        switch (operation) {
            case '+': {
                auto [_, inserted] = set.insert(node);
                std::cout << (inserted ? "OK" : "FAIL") << '\n';
                break;
            }
            case '-': {
                size_t erased = set.erase(node);
                std::cout << (erased > 0 ? "OK" : "FAIL") << '\n';
                break;
            }
            case '?': {
                bool found = set.find(node) != set.end();
                std::cout << (found ? "OK" : "FAIL") << '\n';
                break;
            }
            default:
                std::cout << "FAIL\n";
                break;
        }
    }

    return 0;
}