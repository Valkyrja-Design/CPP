#include <iostream>

class A{
    public:
        A(){ std::cout << "Constructor called!\n"; }
        A(const A& a){ 
            std::cout << "Copy constructor called!\n";
        }
        A(A&& a){
            std::cout << "Move constructor called!\n";
        }
        A& operator=(A&& a){
            std::cout << "Move assignment operator called!\n";
            return *this;
        }
        A& operator=(const A& a){
            std::cout << "Copy assignment operator called!\n";
            return *this;
        }
        ~A(){
            std::cout << "Destructor called!\n";
        }
};

A foo(){
    A a;
    return a;   // copy constructor elided
}

A goo(){
    return A{};  // copy constructor elided
}

void hoo(A a){}

A loo(){
    return A{};
}

int main(){ 
    A a = foo();    // move constructor elided
    A b = goo();    // move constructor elided 
    A c = A{};       // move elided
    A d = A{A{}};   // copy and move both elided 
    hoo({});    // constructor is called
    hoo(A{});   // copy elided
    loo();
    A a = {};
    a = A{};    // no elision here    
}