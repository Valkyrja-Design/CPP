#include <iostream>

/* Initializes an aggregate from an initializer list. It is a form of list-initialization */

class A{
    public:
        int x;
        A(int x, int y) {}
};
int main(){
    A a = {2, 3};
}