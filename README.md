# Initialization

- [Default initialization](https://en.cppreference.com/w/cpp/language/default_initialization)
- [Aggregate initialization](https://en.cppreference.com/w/cpp/language/aggregate_initialization)
- [Value initialization](https://en.cppreference.com/w/cpp/language/value_initialization)
- [List initialization](https://en.cppreference.com/w/cpp/language/list_initialization)
- [Zero initialization](https://en.cppreference.com/w/cpp/language/zero_initialization)
- [Copy initialization](https://en.cppreference.com/w/cpp/language/copy_initialization)

# Rule of five/three/zero

- [cppreference](https://en.cppreference.com/w/cpp/language/rule_of_three)

# Constant folding

Optimization performed by the compiler where it works out the result of an expression at compile time and directly inserts that into the generated code. For example:
```
#define ONE 1
#define TWO 2 

int main(){
    int x = (TWO ? 10 : 5) + ONE + TWO;  // will directly result in 13
}
```
- [constant-folding](https://devblogs.microsoft.com/cppblog/optimizing-c-code-constant-folding/)

# As-if Rule

Basically the compiler can perform any optimization as long as the *observable behavior* of the program doesn't change. For example:
```
int& preinc(int& n) { return ++n; }
int add(int n, int m) { return n+m; }
 
// volatile input to prevent constant folding 
volatile int input = 7;
 
// volatile output to make the result a visible side-effect
volatile int result;
 
int main()
{
    int n = input;
// using built-in operators would invoke undefined behavior
//    int m = ++n + ++n;
// but using functions makes sure the code executes as-if 
// the functions were not overlapped
    int m = add(preinc(n), preinc(n));
    result = m;
}
```
Here, the compiler can eliminate all calls to ```preinc``` and ```add``` to just a single expression  
```
result = 2*input + 3;
```

The only optimization which elides this rule is [*Copy Elision*](#copy-elision)

- [cppreference](https://en.cppreference.com/w/cpp/language/as_if)
- [stackoverflow](https://stackoverflow.com/questions/15718262/what-exactly-is-the-as-if-rule)

# Copy Elision

It is a type of optimization performed by the compiler involving the elision (or omission) of copy/move constructors to avoid extra copies. **An important point is that the constructor which is being elided must still exist and must be accessible to ensure that copy elision does not happen in cases where the object is non-copyable (if the constructor is private or protected or deleted)**. Common forms of copy elision:

## Named Return Value Optimization

It happens when an (named) object returned by value from a function has its copy elided
```
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
};

A foo(){
    A a;
    return a;   // elided
}

int main(){
    A a = foo();    // copy constructor elided here as well
    /* 
      foo::a is effectively constructed into the memory for main::a and it is destructed after main::a's lifetime
    */
}
```
the output of this program is just
```
Constructor called!
Destructor called!
```
If we had defined a move constructor then that would have been elided in ```A a = foo();``` instead of copy constructor.

## Return Value Optimization 

This happens when a temporary object is returned from a function.
```
#include <iostream>

class A{
    public:
        A(){ std::cout << "Constructor called!\n"; }
        A(const A& a){ std::cout << "Copy constructor called!\n"; }
        ~A(){ std::cout << "Destructor called!\n"; }
};

A foo(){
    return A{}; // copy elided 
}

int main(){
    A a = foo(); // copying of temporary elided
}
```
Output
```
Constructor called!
Destructor called!
```

## Constructing from temporaries 

```
#include <iostream>

class A{
    public:
        A(){ std::cout << "Constructor called!\n"; }
        A(const A& a){ std::cout << "Copy constructor called!\n": }
        ~A(){ std::cout << "Destructor called!\n"; }
};

void foo(A a){}

int main(){
    A a = A{};  // copy elided
    A a = A{A{}};   // elision 2 times
    foo(A{});       // copy elided for parameter construction
    foo({});        // regular constructor called
}
```

- [stackoverflow](https://stackoverflow.com/questions/12953127/what-are-copy-elision-and-return-value-optimization/12953150#12953150)
- [cppreference](https://en.cppreference.com/w/cpp/language/copy_elision)

# Aggregates

- [stackoverflow](https://stackoverflow.com/questions/4178175/what-are-aggregates-and-pods-and-how-why-are-they-special/7189821#7189821)
