#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

constexpr int f(int x){
    x = 3;
    x = 4;
    return x;
}

int main()
{
    int z = 4;
    const int y = z;
    constexpr int x = std::sqrt(4);
    std::cout << x << '\n';
}