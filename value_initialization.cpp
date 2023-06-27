#include <iostream>

class A{
    public:
        int x;
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
        int x;  // not zero-initialized according to standard 
                // but implementation still zero-initialize if 
                // defaulted default constructor is selected
    public:
        D() = default;
};

class E{
    public:
        A a;    // will be zero-initialized since E has not user-provided ctor
        int y;
};

int main(){
    B b = B();                 
    std::cout << b.x << "\n";
    A a1{};                      // default-initialized
    std::cout << a1.x << "\n";
    A a2;
    std::cout << a2.x << '\n';
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
    E f{};
    std::cout << f.a.x << ' ' << f.y << '\n';
}