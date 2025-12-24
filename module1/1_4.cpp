#include <iostream>

// 1_4. Инвертируйте значение бита в числе N по его номеру K.
// Формат входных данных. Число N, номер бита K.
// Формат выходных данных. Число с инвертированным битом в десятичном виде.

int invertBit(int N, int K) {
    int mask = 1U << K; // Создаем маску с установленным K-м битом
    return N ^ mask; // Инвертируем K-й бит с помощью XOR
}

int main() {
    unsigned int N, K;
    
    std::cin >> N >> K; // Чтение числа и номера бита
    
    int result = invertBit(N, K); // Инвертирование бита
    std::cout << result << std::endl; // Вывод результата
    
    return 0;
}