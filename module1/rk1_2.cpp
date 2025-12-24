#include <iostream>
#include <string>

class Stack {
private:
    char* data;
    int capacity;
    int top_index;

    Stack(const Stack& other) = delete;
    Stack& operator=(const Stack& other) = delete;

public:
    Stack(int size = 10000) {
        capacity = size;
        data = new char[capacity];
        top_index = -1;
    }

    ~Stack() {
        delete[] data;
    }

    void push(char c) {
        if (top_index < capacity - 1) {
            data[++top_index] = c;
        }
    }

    char pop() {
        if (top_index >= 0) {
            return data[top_index--];
        }
        return ' ';
    }

    char top() const {
        if (top_index >= 0) {
            return data[top_index];
        }
        return ' ';
    }

    bool empty() const {
        return top_index == -1;
    }
};

bool is_stack_anagram(const std::string& word1, const std::string& word2) {
    if (word1.length() != word2.length()) {
        return false;
    }

    Stack stack;
    int i = 0;
    int j = 0;

    while (i < word1.length() || !stack.empty()) {
        if (!stack.empty() && stack.top() == word2[j]) {
            stack.pop();
            j++;
        } else if (i < word1.length()) {
            stack.push(word1[i]);
            i++;
        } else {
            return false;
        }
    }

    return j == word2.length();
}

int main() {
    std::string word1, word2;
    std::cin >> word1 >> word2;

    if (is_stack_anagram(word1, word2)) {
        std::cout << "YES" << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }

    return 0;
}