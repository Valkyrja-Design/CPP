#include <iostream>

class A{
    public:
        A(int x) : x(x) {}
        // these will also preclude implicit default constructor 
        // A(const A& other) : x(other.x) {}
        // A(A&& other){}
    public:
        int x;
};

class B{
    public:
        B() = default;  // is user-declared, compiler generates
                        // the implicit default constructor even 
                        // if other constructors present
        B(int x) : x(x) {}
    public:
        int x;
};

// B::B(){     // cannot define the compiler declared default constructor
//     x = 2;
// }

class C{
    public:
        int x;
};

// C::C(){       // cannot define the implicitly declared default constructor 
//     x = 2;
// }

int main(){
    // A a;   // error, implicit default constructor not provided 
              // if there is a user-declared constructor
    B b;      // default constructor generated
    std::cout << b.x << "\n";   // default-initialized
 
}