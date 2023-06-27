#include <iostream>
#include <vector>
#include <string>

class A{
    int x;
    char c1;
    char c2;
};

class B{
    char c1;
    int x;
    char c2;
};

class C{
    char c1;
    char c2;
    int x;
};

class D{
    double d;
    int x;
};

class E{
    double d;
    int x1;
    int x2;
};

class F{
    int x1;
    double d;
    int x2;
};

class G{
    public:
        int x1;
    protected:
        double d;
        int x2;
};

int main(){
	std::cout << "sizeof A " << sizeof(A) << '\n';
	std::cout << "sizeof B " << sizeof(B) << '\n';
	std::cout << "sizeof C " << sizeof(C) << '\n';
	std::cout << "sizeof D " << sizeof(D) << '\n';
	std::cout << "sizeof E " << sizeof(E) << '\n';
	std::cout << "sizeof F " << sizeof(F) << '\n';
	std::cout << "sizeof G " << sizeof(G) << '\n';
}  	