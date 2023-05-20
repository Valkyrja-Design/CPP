#include <iostream>

struct S
{
    int n = 42;   // default member initializer
    S() : n(7) {} // will set n to 7, not 42
};

class A
{
    // A() {} // Error must specify member initializer for member which 
              // cannot be default-initialized like refs and consts
    public:
        const int& v = 2;
};

struct C{
    // C() {}  // Error no initializer for x
    const int x;
};

struct D{
    D() {
        std::cout << x << "\n";
    }
    int x;
};

int main(){
    A a;   
    std::cout << a.v << "\n";
    D b;
}