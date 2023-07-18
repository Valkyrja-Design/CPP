#include <iostream>

class A{
    public:
        A(){ std::cout << "Constructor called!\n"; }
        A(const A& a){ 
            std::cout << "Copy constructor called!\n";
        }
        ~A(){
            std::cout << "Destructor called!\n";
        }
    public:
        int x = 2;
};

A foo(){
    A a;
    a.x = 25;
    return a;   // elided
}

int main(){
    A a = foo();    // copy constructor elided here as well
    /* 
      foo::a is effectively constructed into the memory for main::a and it is destructed after main::a's lifetime
    */
}