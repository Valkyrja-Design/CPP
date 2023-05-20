#include <iostream>

class A{
    public:
        int x;
    public:
        A() {}  // user-provided constructor, will be 
                // called to default-initialize leaving
                // x to be indeterminate

};

class B{
    public:
        int x;  // zero-initialized
};

class C{
    public:
        int x;
    public:
        C() = delete;
};

class D{
    public:
        int x;
    public:
        D() = default;
};

int main(){
    B b = B{};                 
    std::cout << b.x << "\n";
    A a{};                      // default-initialized
    std::cout << a.x << "\n";
    int x{};                    // zero-initialized
    std::cout << x << "\n";
    C c = C{};                  // aggregate-initialization
                                // allowed since C++11 but 
                                // fails since C++20 (default-initialization
                                // tried and ends in failure)
    std::cout << c.x << "\n"; 
    D* d = new D();
    std::cout << d->x << "\n";

    B e();                      // not an object but a function!
    // std::cout << e.x << "\n";// error
}