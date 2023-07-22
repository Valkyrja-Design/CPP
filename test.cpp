#include <iostream>

template < typename T >
void call_with_true_double_only(T) =delete; //prevent call through type promotion of any T to double from succeeding.

void call_with_true_double_only(const double& param) = delete;
void call_with_true_double_only(double& param) { return; } // also define for const double, double&, etc. as needed.

class A{
    public:
        int x;
};

class B : public A{

};

int main(){
    B b;
    std::cout << b.A::x << '\n';
}