# User-provided Functions

A function is user-provided if it is user-declared and not explicitly defaulted or deleted on its declaration. A user-provided explicitly defaulted (defaulted after its first declaration) is defined at the point it is explicitly defaulted.

- [cppreference](https://en.cppreference.com/w/cpp/language/function#User-provided_functions)

# Default Constructors
This is a constructor which can be called with no args (either defined with empty parameter list or with default values for all the arguments).  

## Implicitly-declared default constructor
If there are no *user-declared* constructors of any type provided in a class then the compiler always declares a default constructor as ```inline public``` member function of its class. You can still force the generation of a default constructor in the presence of other user-declared constructors using the ```default``` keyword.

## Implicitly-defined default constructor
If the implicitly-declared is not defined as deleted, it is defined by the compiler if it is **[odr-used](https://en.cppreference.com/w/cpp/language/definition#ODR-use)**

```
struct A
{
    int x;
    A(int x = 1): x(x) {} // user-defined default constructor
};
 
struct B: A
{
    // B::B() is implicitly-defined, calls A::A()
};
 
struct C
{
    A a;
    // C::C() is implicitly-defined, calls A::A()
};
 
struct D: A
{
    D(int y): A(y) {}
    // D::D() is not declared because another constructor exists
};
 
struct E: A
{
    E(int y): A(y) {}
    E() = default; // explicitly defaulted, calls A::A()
};
 
struct F
{
    int& ref; // reference member
    const int c; // const member
    // F::F() is implicitly defined as deleted
};
 
// user declared copy constructor (either user-provided, deleted or defaulted)
// prevents the implicit generation of a default constructor
 
struct G
{
    G(const G&) {}
    // G::G() is implicitly defined as deleted
};
 
struct H
{
    H(const H&) = delete;
    // H::H() is implicitly defined as deleted
};
 
struct I
{
    I(const I&) = default;
    // I::I() is implicitly defined as deleted
};
 
int main()
{
    A a;
    B b;
    C c;
//  D d; // compile error
    E e;
//  F f; // compile error
//  G g; // compile error
//  H h; // compile error
//  I i; // compile error
}
```

- [cppreference](https://en.cppreference.com/w/cpp/language/default_constructor)

# Constructors 

We must provide member initializers for bases and non-static data members, such as refs and const, which cannot be default-initialized. 

### Delegating Constructor 

If the name of the class itself appears in the member initializer list, then the list must consist of only that one member initializer and such a constructor is called *delegating constructor*, and the constructor selected by the initializer is called the *target constructor*. The target constructor is executed after which the control reaches the delegating constructor and its body is executed.  
**Delegating constructors cannot be recursive**

```
class Foo
{
public: 
    Foo(char x, int y) {}
    Foo(int y) : Foo('a', y) {} // Foo(int) delegates to Foo(char, int)
};
```

- [cppreference](https://en.cppreference.com/w/cpp/language/constructor)

# Initialization

### Default Initialization 

This is performed when an object is constructed with no initializer. The effects of default initialization are :

- if the object is a **non-POD** class type, then its default constructor is called to provide the initial value to the object.
- if the object is an array, every element is default-initialized.
- no initialization is performed otherwise.

**Note**: Default initialization of *non-class* variables with automatic or dynamic storage duration results in indeterminate values (**static and thread-local objects get zero-initialized**).

- [cppreference](https://en.cppreference.com/w/cpp/language/default_initialization)

### Value Initialization 

This is performed when an object is constructed with an empty initializer.  
- If ```{}``` is used and the type T is an aggregate type, then [*aggregate-initialization*](#aggregate-initialization) is performed instead of value-initialization.  
- If the type T is a class type with no default constructors but has a constructor taking ```std::initializer_list```, [*list-initialization*](#list-initialization) is performed.

The effects of value-initialization are:

- If the type T is class type with no [*default constructor*](#default-constructors) or with a user-declared, user-provided or deleted default constructor, the object is [*default-initialized*](#default-initialization). If no default constructor exists, this leads to a compilation error.
- If the type T is class type with a default constructor that is not user-declared neither user-provided nor deleted (i.e., a class with an implicitly-defined or defaulted default constructor), the object is [*zero-initialized*](#zero-initialization) and and the semantic constraints for default-initialization are checked, and if T has a *non-trivial default constructor*, the object is [*default-initialized*](#default-initialization).
- If T is an array type, each element of the array is value-initialized.
- otherwise, the object is [*zero-initialized*](#zero-initialization).

**Notes**: 
1. The syntax ```T object();``` *does not* initialize an object; it declares a function instead!  
2. **References cannot be value-initialized**  

- [cppreference](https://en.cppreference.com/w/cpp/language/value_initialization)

### Aggregate Initialization

- [Aggregate initialization](https://en.cppreference.com/w/cpp/language/aggregate_initialization)

### List Initialization

- [List initialization](https://en.cppreference.com/w/cpp/language/list_initialization)

### Zero Initialization

- [Zero initialization](https://en.cppreference.com/w/cpp/language/zero_initialization)
- [Default initialization](https://en.cppreference.com/w/cpp/language/default_initialization)
- [Value initialization](https://en.cppreference.com/w/cpp/language/value_initialization)
- [Copy initialization](https://en.cppreference.com/w/cpp/language/copy_initialization)


# Rule of five/three/zero

### Rule of zero
It says that a class should rely on its members (smart pointers) to manage memory resources and avoid implementing custom copy/move constructors or copy/move assignment operators. 

### Rule of three 
If a class requires a user-defined constructor, a user-defined destructor or a user-defined copy assignment operator, it almost certainly requires all three of them.  

### Rule of five 
**The presence of a user-defined (or ```= default``` or ```= delete``` declared) destructor, copy constructor, or copy assignment operator prevents implicit definition of move constructor and move assignment operator.** So if a class requires move semantics then we must provide all five of the special member functions.

- [cppreference](https://en.cppreference.com/w/cpp/language/rule_of_three)
- [stackoverflow](https://stackoverflow.com/questions/44997955/rule-of-zero-confusion)

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
