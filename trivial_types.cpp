#include <iostream>

// We need the class to have compiler-provided 
// or explicitly defaulted special member functions
// for it to be trivial type

class A{
    public:
        A(){}   // not trivial
};  

class B{
    public:
        B() = default;  // trivial
        B(int x = 3) {}
};

class C{
    public:
        C(int x = 3){}  // not trivial
};

class D{
    public:
        int x;
        int y;
        int z;
    private:
        int n;
        double m;
};

int main(){
    std::cout << std::is_trivial<A>() << '\n';
    std::cout << std::is_trivial<B>() << '\n';
    std::cout << std::is_trivial<C>() << '\n';
}