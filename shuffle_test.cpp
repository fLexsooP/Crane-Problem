#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

int main(int argc, char const *argv[]) {
    std::mt19937 gen;
    std::vector<int> array = {1, 2, 3, 4, 5, 6};
    std::shuffle(array.begin(), array.end(), gen);
    for (auto &&i : array)
    {
        std::cout << i << " ";
    }

    return 0;
}

// g++ -std=c++17 -Wall shuffle_test.cpp -o shuffle_test