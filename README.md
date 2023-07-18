# Table of Contents

- [Trivial Types](#trivial-types)
- [Scoped and Unscoped Enums](#scoped-and-unscoped-enums)
- [User-provided Functions](#user-provided-functions)
- [Default Constructors](#default-constructors)
    - [Implicitly-declared default constructor](#implicitly-declared-default-constructor)
    - [Implicitly-defined default constructor](#implicitly-defined-default-constructor)
- [Constructors](#constructors)
    - [Delegating Constructor](#delegating-constructor)
- [Rule of five/three/zero](#rule-of-fivethreezero)
    - [Rule of zero](#rule-of-zero)
    - [Rule of three](#rule-of-three)
    - [Rule of five](#rule-of-five)
- [Narrowing Conversions](#narrowing-conversions)
- [Initialization](#initialization)
    - [Default Initialization](#default-initialization)
    - [Non-local Initialization](#non-local-initialization)
    - [Value Initialization](#value-initialization)
    - [List Initialization](#list-initialization)
    - [Aggregate Initialization](#aggregate-initialization)
    - [Designated Initializers](#designated-initializers)
    - [Constant Initialization](#constant-initialization)
    - [Zero Initialization](#zero-initialization)
    - [Copy Initialization](#copy-initialization)
    - [Direct Initialization](#direct-initialization)
- [Aggregates](#aggregates)
- [Constant Folding](#constant-folding)
- [As-if Rule](#as-if-rule)
- [Copy Elision](#copy-elision)
    - [Named Return Value Optimization](#named-return-value-optimization)
    - [Return Value Optimization](#return-value-optimization)
    - [Constructing from temporaries](#constructing-from-temporaries)
- [`using` vs `typedef`](#using-vs-typedef)
- [Abbreviated function templates (since C++20)](#abbreviated-function-templates-since-c20)
- [Lambda Expressions](#lambda-expressions)
- [Limitations](#limitations)
    - [Member function templates cannot be virtual](#member-function-templates-cannot-be-virtual)


# Trivial Types

A class or struct is trivial type if it has compiler-provided or explicitly defaulted special member functions. It occupies a contiguous region of memory and can have different access specifiers.

# Scoped and Unscoped Enums

```cpp
// unscoped enum:
// enum [identifier] [: type] {enum-list};

// scoped enum:
// enum [class|struct] [identifier] [: type] {enum-list};

// Forward declaration of enumerations  (C++11):
enum A : int;          // non-scoped enum must have type specified
enum class B;          // scoped enum defaults to int but ...
enum class C : short;  // ... may have any integral underlying type
```

We specify an enum is scoped by using the ```class or struct``` keyword, in which case an identifier must be provided. In an unscoped enum, the scope is the surrounding scope; in a scoped enum, the scope is the enum-list itself. In a scoped enum, the list may be empty, which in effect defines a new integral type.

```cpp
namespace CardGame_Scoped
{
    enum class Suit { Diamonds, Hearts, Clubs, Spades };

    void PlayCard(Suit suit)
    {
        if (suit == Suit::Clubs) // Enumerator must be qualified by enum type
        { /*...*/}
    }
}

namespace CardGame_NonScoped
{
    enum Suit { Diamonds, Hearts, Clubs, Spades };

    void PlayCard(Suit suit)
    {
        if (suit == Clubs) // Enumerator is visible without qualification
        { /*...*/
        }
    }
}
```

- [Microsoft C++ documentation](https://learn.microsoft.com/en-us/cpp/cpp/enumerations-cpp?view=msvc-170)

# User-provided Functions

A function is user-provided if it is user-declared and not explicitly defaulted or deleted on its declaration. A user-provided explicitly defaulted (defaulted after its first declaration) is defined at the point it is explicitly defaulted.

- [cppreference](https://en.cppreference.com/w/cpp/language/function#User-provided_functions)

# Default Constructors
This is a constructor which can be called with no args (either defined with empty parameter list or with default values for all the arguments).  

## Implicitly-declared default constructor
If there are no *user-declared* constructors of any type provided in a class then the compiler always declares a default constructor as ```inline public``` member function of its class. You can still force the generation of a default constructor in the presence of other user-declared constructors using the ```default``` keyword.

## Implicitly-defined default constructor
If the implicitly-declared is not defined as deleted, it is defined by the compiler if it is **[odr-used](https://en.cppreference.com/w/cpp/language/definition#ODR-use)**

```cpp
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

```cpp
class Foo
{
public: 
    Foo(char x, int y) {}
    Foo(int y) : Foo('a', y) {} // Foo(int) delegates to Foo(char, int)
};
```

- [cppreference](https://en.cppreference.com/w/cpp/language/constructor)

# Rule of five/three/zero

### Rule of zero
It says that a class should rely on its members (smart pointers) to manage memory resources and avoid implementing custom copy/move constructors or copy/move assignment operators. 

### Rule of three 
If a class requires a user-defined constructor, a user-defined destructor or a user-defined copy assignment operator, it almost certainly requires all three of them.  

### Rule of five 
**The presence of a user-defined (or ```= default``` or ```= delete``` declared) destructor, copy constructor, or copy assignment operator prevents implicit definition of move constructor and move assignment operator.** So if a class requires move semantics then we must provide all five of the special member functions.

- [cppreference](https://en.cppreference.com/w/cpp/language/rule_of_three)
- [stackoverflow](https://stackoverflow.com/questions/44997955/rule-of-zero-confusion)

# Narrowing Conversions

- Conversion of a floating-point type to an integer type.
- Conversion from a ```long double``` to ```double``` or to ```float``` and conversion from ```double``` to ```float```, except where the source is a constant expression and overflow does not occur.
- Conversion from an integer type to a floating-point type, except where the source is a constant expression whose value can be stored exactly in the target type.
- Conversion from an integer type or [unscoped enumeration type] to an integer type that cannot store all the values of the original, except where the source is a constant expression whose value can be stored exactly in the target type.
- Conversion from a pointer type or pointer-to-member type to bool.
- [cppreference](https://en.cppreference.com/w/cpp/language/list_initialization)

# Initialization

### Default Initialization 

### Syntax 

- ```T object ;```
- ```new T```

This is performed when an object is constructed with no initializer. The effects of default initialization are :

- if the object is a **non-POD** class type, then its default constructor is called to provide the initial value to the object.
- if the object is an array, every element is default-initialized.
- no initialization is performed otherwise.

**Note**: Default initialization of *non-class* variables with automatic or dynamic storage duration results in indeterminate values (**static and thread-local objects get zero-initialized**).

- [cppreference](https://en.cppreference.com/w/cpp/language/default_initialization)

### Non-local Initialization 

All non-local variables with static storage duration are initialized as part of program startup, before the execution of ```main()``` begins. The initialization occurs in two distinct stages:

#### Static Initialization

There are two forms of static initialization:

1. If possible, constant initialization is applied.
2. Otherwise, non-local static and thread-local variables are zero-initialized.
In practice:

Constant initialization is usually applied at compile time. Pre-calculated object representations are stored as part of the program image. If the compiler doesn't do that, it must still guarantee that the initialization happens before any dynamic initialization.  
Variables to be zero-initialized are placed in the .bss segment of the program image, which occupies no space on disk and is zeroed out by the OS when loading the program.

#### Dynamic Initialization

- [stackoverflow](https://stackoverflow.com/questions/17783210/when-are-static-and-global-variables-initialized)
- [stackoverflow](https://stackoverflow.com/questions/22117310/c-global-variable-initialization-order)
- [cppreference](https://en.cppreference.com/w/cpp/language/initialization#Non-local_variables)

### Value Initialization 

#### Syntax

- ```T()```
- ```new T()```
- ```Class::Class(...) : member () { ... }```
- ```T object{};```
- ```T{}```
- ```new T{}```
- ```Class::Class(...) : member {} { ... }```

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
3. The standard specifies that zero-initialization is not performed when the class has a user-provided or deleted default constructor, which implies that whether said default constructor is selected by overload resolution is not considered. All known compilers performs additional zero-initialization if a non-deleted defaulted default constructor is selected.

```cpp
struct A
{
    A() = default;
 
    template<class = void>
    A(int = 0) {} // A has a user-provided default constructor, which is not selected
 
    int x;
};
 
constexpr int test(A a)
{
    return a.x; // the behavior is undefined if a's value is indeterminate
}
 
constexpr int zero = test(A());
// ill-formed: the parameter is not zero-initialized according to the standard,
// which results in undefined behavior that makes the program ill-formed in contexts 
// where constant evaluation is required.
// However, such code is accepted by all known compilers.
 
void f()
{
    A a = A(); // not zero-initialized according to the standard
               // but implementations generate code for zero-initialization nonetheless
}
```

- [cppreference](https://en.cppreference.com/w/cpp/language/value_initialization)

### List Initialization

#### Direct-list-initialization

- ```T object{ arg1, arg2, ... }```
- ```T object{.des1 = arg1 , .des2 { arg2 } ... };``` (**since C++20**)
- ```T { arg1, arg2, ... }```
- ```T {.des1 = arg1 , .des2 { arg2 } ... }``` (**since C++20**)
- ```new T{ arg1, arg2, ... }```
- ```new T {.des1 = arg1 , .des2 { arg2 } ... }``` (**since C++20**)
- ```Class { member { arg1, arg2, ... }; };```
- ```Class { T member {.des1 = arg1 , .des2 { arg2 } ... }; };``` (**since C++20**)
- ```Class::Class() : member { arg1, arg2, ... } {...```
- ```Class::Class() : member {.des1 = arg1 , .des2 { arg2 } ...} {...``` (**since C++20**)

#### Copy-list-initialization

- ```T object = { arg1, arg2, ... };```
- ```T object = {.des1 = arg1 , .des2 { arg2 } ... };``` (**since C++20**)
- ```function ({ arg1, arg2, ... })```
- ```function ({.des1 = arg1 , .des2 { arg2 } ... })``` (**since C++20**)
- ```return { arg1, arg2, ... };```
- ```return {.des1 = arg1 , .des2 { arg2 } ... };``` (**since C++20**)
- ```object [{ arg1, arg2, ... }]```
- ```object [{.des1 = arg1 , .des2 { arg2 } ... }]``` (**since C++20**)
- ```object = { arg1, arg2, ... }```
- ```object = {.des1 = arg1 , .des2 { arg2 } ... }``` (**since C++20**)
- ```Class { T member = { arg1, arg2, ... }; };```
- ```Class { T member = {.des1 = arg1 , .des2 { arg2 } ... }; };``` (**since C++20**)

Initialization of an object from *braced-init-list*. The effects of list-initialization of an object of type T are:

- If the *braced-init-list* contains a [designated-initializer-list](#designated-initializers), T must be an [aggregate](#aggregates) class and [aggregate-initialization](#aggregate-initialization) is performed. (**since C++20**)
- If T is an [aggregate class](#aggregates) and the *braced-init-list* contains a single element of the same or derived type, the object is initialized from that element (by [copy-initialization](#copy-initialization) for copy-list-initialization, or by [direct-initialization](#direct-initialization) for direct-list-initialization).
- Otherwise, if T is a char array and the *braced-init-list* has a single string literal, the array is initialized from the string literal as usual.
- Otherwise, if T is an [aggregate type](#aggregates), [aggregate initialization](#aggregate-initialization) is performed.
- Otherwise, if the *braced-init-list* is empty and T is a class type with a default constructor, [value-initialization](#value-initialization) is performed.
- Otherwise, the constructors of T are considered, first all constructors taking ```std::initializer_list``` as the only argument, or as the first argument if all the remaining arguments have default values against a single argument of type ```std::initializer_list``` (our *braced-init-list*) and if this doesn't produce a match, all constructors of T are matched against the set of arguments that consists of the elements of the *braced-init-list*.
- Otherwise, if T is not a class type, if the *brace-init-list*  has only one element then either [direct-initialized](#direct-initialization) or [copy-initialized](#copy-initialization) depending on the syntax of the initializer used, except that [narrowing conversions](#narrowing-conversions) are not allowed.
- Otherwise, if the *braced-init-list* has no elements, [value-initialization](#value-initialization) is performed.

#### Notes

Every initializer clause is **sequenced before** any initializer clause that follows it in the *braced-init-list*. This is in contrast with the arguments of a function call expression, which are **unsequenced (until C++17)**, **indeterminately sequenced (since C++17)**.  

A braced-init-list is not an expression and therefore has no type. ```auto``` keyword deduces any *braced-init-list* as ```std::initializer_list``` in copy-list-initialization.

Aggregates copy/move initialize directly from single-element braced-init-list of the same type, but non-aggregates consider initializer_list constructors first:

```cpp
struct X {}; // aggregate
 
struct Q     // non-aggregate
{
    Q() = default;
    Q(Q const&) = default;
    Q(std::initializer_list<Q>) {}
};
 
int main()
{
    X x;
    X x2 = X{x}; // copy-constructor (not aggregate initialization)
 
    Q q;
    Q q2 = Q{q}; // initializer-list constructor (not copy constructor)
}
```

- [List initialization](https://en.cppreference.com/w/cpp/language/list_initialization)

### Aggregate Initialization

#### Syntax 

- ```T object = { arg1, arg2, ... };```
- ```T object { arg1, arg2, ... };```
- ```T object = { .des1 = arg1 , .des2 { arg2 } ... };``` (**since C++20**)
- ```T object { .des1 = arg1 , .des2 { arg2 } ... };``` (**since C++20**)

Initialization of an aggregate from an initializer list. This is a type of [list-initialization](#list-initialization)

The effects of aggregate initialization are:

- For designated initializers (**since C++20**), the explicitly initialized elements are given by the identifiers in the designators. 
- Otherwise, the explicitly initialized elements are the first n elements of the aggregate, where n is the number of elements in the initializer list.
- Initialize each element of the aggregate in element order. 

#### Initializing the elements

For each explicitly initialized element:

- The element is [copy-initialized](#copy-initialization) from the corresponding value in the initializer list. (**until C++20**)
- The element is initialized from the corresponding element of the initializer list. The initialization is [direct-initialization](#direct-initialization) if the initializer list is *designated initializer list* and the initializer begins with = ```(object = )```, otherwise the initialization is [copy-initialization](#copy-initialization). (**since C++20**)
- If the initializer element is a nested braced-init-list, [list-initialize](#list-initialization) the corresponding element with that initializer clause, which will recursively apply the rule if that element is a subaggregate.

```cpp
struct A
{
    int x;
 
    struct B
    {
        int i;
        int j;
    } b;
} a = {1, {2, 3}}; // initializes a.x with 1, a.b.i with 2, a.b.j with 3
 
struct base1 { int b1, b2 = 42; };
 
struct base2
{
    base2()
    {
        b3 = 42;
    }
 
    int b3;
};
 
struct derived : base1, base2
{
    int d;
};
 
derived d1{{1, 2}, {}, 4}; // initializes d1.b1 with 1, d1.b2 with 2,
                           //             d1.b3 with 42, d1.d with 4
derived d2{{}, {}, 4};     // initializes d2.b1 with 0, d2.b2 with 42,
                           //             d2.b3 with 42, d2.d with 4
```

Each element that is not an explicitly initialized element is initialized as follows:

- If the element has a default member initializer, the element is initialized from that initializer.
- Otherwise, if the element is not a reference, the element is [copy-initialized](#copy-initialization) from an empty initializer list.
- Otherwise, the program is ill-formed.

```cpp
struct S
{
    int a;
    const char* b;
    int c;
    int d = b[a];
};
 
// initializes ss.a with 1,
//             ss.b with "asdf",
//             ss.c with the value of an expression of the form int{} (that is, 0),
//         and ss.d with the value of ss.b[ss.a] (that is, 's')
S ss = {1, "asdf"};
```

### Designated Initializers

These are initializers of the form ```{ .des1 = arg1, .des2 = arg2, .. }``` (**since C++20**). Each designator must be the identifier of a non-static data member of class T, and all designators used in the list must appear in the same order as the members of T. Also cannot mix regular and designated initializers so the following is ill-formed:

```cpp
A a{.y = 2, .x = 1}; // error; designator order does not match declaration order
A b{.x = 1, .z = 2}; // ok, b.y initialized to 0

struct C
{
    string str;
    int n = 42;
    int m = -1;
};
 
C{.m = 21} // Initializes str with {}, which calls the default constructor
           // then initializes n with = 42
           // then initializes m with = 21

struct A { int x, y; };
struct B { struct A a; };
 
struct A a = {.y = 1, .x = 2}; // valid C, invalid C++ (out of order)
int arr[3] = {[1] = 5};        // valid C, invalid C++ (array)
struct B b = {.a.x = 0};       // valid C, invalid C++ (nested)
struct A a = {.x = 1, 2};      // valid C, invalid C++ (mixed)
```

- [Aggregate initialization](https://en.cppreference.com/w/cpp/language/aggregate_initialization)

### Constant Initialization

Sets the initial values of static variables to a compile-time constant. This is performed instead of [zero-initialization](#zero-initialization) if the initialization of the object has an initializer or its [default-initialization](#default-initialization) results in some initialization being performed.

```cpp
#include <iostream>
#include <array>
 
struct S
{
    static const int c;
};
 
const int d = 10 * S::c; // not a constant expression: S::c has no preceding
                         // initializer, this initialization happens after const
const int S::c = 5;      // constant initialization, guaranteed to happen first
 
int main()
{
    std::cout << "d = " << d << '\n';
    std::array<int, S::c> a1; // OK: S::c is a constant expression
//  std::array<int, d> a2;    // error: d is not a constant expression
}
```

- [cppreference](https://en.cppreference.com/w/cpp/language/constant_initialization)

### Zero Initialization

Sets the initial value of an object to 0. Zero-initialization is performed in the following cases:

- For every named variable with **static or thread-local** storage duration that is not subject to [constant-initialization](#constant-initialization), **before any other initialization**.
- As part of [value-initialization](#value-initialization) of non-class types and for members of value-initialized class types that have no (user-provided) constructors, including the [value-initialization](#value-initialization) of elements of [aggregates](#aggregates) for which no initializers are provided.
- When an array of any char type is initialized with a string literal that is too short, the remainder of the array is zero-initialized.

The effects of zero-initialization are:

- If T is a class type:
    - all padding bits are initialized to zero bits,
    - each non-static data member is zero-initialized,
    - each non-virtual base class subobject is zero-initialized,
    - if the object is not a base class subobject, each virtual base class subobject is zero-initialized.
- If T is an array type, each element is zero-initialized.
- If T is a reference type, nothing is done.
- [Zero initialization](https://en.cppreference.com/w/cpp/language/zero_initialization)

### Copy Initialization

Initializes an object from another object. 

#### Syntax 

- ```T object = other;```
- ```f(other)```
- ```return other;```
- ```throw object; catch (T object)```
- ```T array[N] = {other-sequence};```

The constructors of T are examined and the best one of them is choosen to initialized the object, for example move constructor if the right object is an rvalue. [Copy elision](#copy-elision) applies.

```cpp
#include <memory>
#include <string>
#include <utility>
 
struct A
{
    operator int() { return 12;}
};
 
struct B
{
    B(int) {}
};
 
int main()
{
    std::string s = "test";        // OK: constructor is non-explicit
    std::string s2 = std::move(s); // this copy-initialization performs a move
 
//  std::unique_ptr<int> p = new int(1); // error: constructor is explicit
    std::unique_ptr<int> p(new int(1));  // OK: direct-initialization
 
    int n = 3.14;    // floating-integral conversion
    const int b = n; // const doesn't matter
    int c = b;       // ...either way
 
    A a;
    B b0 = 12;
//  B b1 = a;       // < error: conversion from 'A' to non-scalar type 'B' requested
    B b2{a};        // < identical, calling A::operator int(), then B::B(int)
    B b3 = {a};     // <
    auto b4 = B{a}; // <
 
//  b0 = a;         // < error, assignment operator overload needed
 
    [](...){}(c, b0, b3, b4); // pretend these variables are used
}
```

- [Copy initialization](https://en.cppreference.com/w/cpp/language/copy_initialization)

### Direct Initialization 

Initializes an object from explicit set of constructor arguments. The effects of direct-initialization are:

- If T is an array type, the program is ill-formed.
- If T is a class type, the constructors of T are examined and the best match is selected by overload resolution. The constructor is then called to initialize the object.
- Otherwise, if T is a non-class type but the source type is a class type, the conversion functions of the source type and its base classes, if any, are examined and the best match is selected by overload resolution. The selected user-defined conversion is then used to convert the initializer expression into the object being initialized.

```cpp
#include <iostream>
#include <memory>
#include <string>
 
struct Foo
{
    int mem;
    explicit Foo(int n) : mem(n) {}
};
 
int main()
{
    std::string s1("test"); // constructor from const char*
    std::string s2(10, 'a');
 
    std::unique_ptr<int> p(new int(1));  // OK: explicit constructors allowed
//  std::unique_ptr<int> p = new int(1); // error: constructor is explicit
 
    Foo f(2); // f is direct-initialized:
              // constructor parameter n is copy-initialized from the rvalue 2
              // f.mem is direct-initialized from the parameter n
//  Foo f2 = 2; // error: constructor is explicit
 
    std::cout << s1 << ' ' << s2 << ' ' << *p << ' ' << f.mem  << '\n';
}
```

- [Direct Initialization](https://en.cppreference.com/w/cpp/language/direct_initialization)

# Aggregates

An aggregate needs to be one of the following types:

- array type
- class type that has 
    - no user-provided, inherited or explicit constructors (**since C++11, until C++20**)
    - no user-declared or inherited constructors (**since C++20**)
    - no private or protected non-static data members
    - no base classes (**until C++17**)
    - no default member initializers (**since C++11, until C++14**)
- [stackoverflow](https://stackoverflow.com/questions/4178175/what-are-aggregates-and-pods-and-how-why-are-they-special/7189821#7189821)


# Constant folding

Optimization performed by the compiler where it works out the result of an expression at compile time and directly inserts that into the generated code. For example:

```cpp
#define ONE 1
#define TWO 2 

int main(){
    int x = (TWO ? 10 : 5) + ONE + TWO;  // will directly result in 13
}
```

- [constant-folding](https://devblogs.microsoft.com/cppblog/optimizing-c-code-constant-folding/)

# As-if Rule

Basically the compiler can perform any optimization as long as the *observable behavior* of the program doesn't change. For example:

```cpp
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

```cpp
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

```cpp
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

```cpp
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

# `using` vs `typedef`

Semantically the same thing but have some difference in allowed contexts and ease of usage in case of template aliasing. A *typedef-declaration* is an *init-statement* and is thus allowed in contexts which allow *init-statement*.

```cpp
// C++11 (C++03) (init. statement in for loop iteration statements).
for (typedef int Foo; Foo{} != 0;)
//   ^^^^^^^^^^^^^^^ init-statement
{
}

// C++17 (if and switch initialization statements).
if (typedef int Foo; true)
//  ^^^^^^^^^^^^^^^ init-statement
{
    (void)Foo{};
}

switch (typedef int Foo; 0)
//      ^^^^^^^^^^^^^^^ init-statement
{
    case 0: (void)Foo{};
}

// C++20 (range-based for loop initialization statements).
std::vector<int> v{1, 2, 3};
for (typedef int Foo; Foo f : v)
//   ^^^^^^^^^^^^^^^ init-statement
{
    (void)f;
}

for (typedef struct { int x; int y;} P; auto [x, y] : {P{1, 1}, {1, 2}, {3, 5}})
//   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ init-statement
{
    (void)x;
    (void)y;
}
```

whereas an *alias-declaration* is **not** an *init-statement* and thus was not allowed in contexts which allow *init-statement* **until C++23**. **This inconsistency has between *typedef-declaration* and *alias-declaration* has been removed in C++23.**

```cpp
// C++ 11.
for (using Foo = int; Foo{} != 0;) {}
//   ^^^^^^^^^^^^^^^ error: expected expression

// C++17 (initialization expressions in switch and if statements).
if (using Foo = int; true) { (void)Foo{}; }
//  ^^^^^^^^^^^^^^^ error: expected expression

switch (using Foo = int; 0) { case 0: (void)Foo{}; }
//      ^^^^^^^^^^^^^^^ error: expected expression

// C++20 (range-based for loop initialization statements).
std::vector<int> v{1, 2, 3};
for (using Foo = int; Foo f : v) { (void)f; }
//   ^^^^^^^^^^^^^^^ error: expected expression
```

- [stackoverflow](https://stackoverflow.com/questions/10747810/what-is-the-difference-between-typedef-and-using-in-c11)


# Abbreviated function templates (since C++20)

When `auto` appears in the parameter list of a function declaration or of a function template declaration, the declaration declares a function template, and one 'invented' template parameter for each `auto` is appended to the template parameter list. 

```cpp
void f1(auto); // same as template<class T> void f1(T)
```

Abbreviated function templates can be specialized like all function templates.

# Lambda Expressions (since C++11)

A lambda expression is a convenient way of defining an anonymous function object (also called a *closure*) right at the location where it is invoked or passed as an argument to a function.

Syntax

- `[ captures ] ( params ) specs requires ﻿(optional) { body }`
- `[ captures ] { body }`

The capture clause specifies which variables from the surrounding scope are captured, and whether the capture is by value or reference. Variables with `&` are accessed by reference and by value otherwise.  
You can also specify a *capture-default* in the beginning of the clause. The only capture defaults are 

- `&` (implicity capture the **used** automatic variables by reference)
- `=` (implicity capture the **used** automatic variables by value)

The current object `*this` can be implicitly captured if either capture default is present. If implicitly captured, it is always captured by reference, even if the capture default is `=`.  
*The implicit capture of `*this` when the capture default is `=` is deprecated. (since C++20)*

If the capture default is `&`, subsequent captures must not begin with `&`:

```cpp
struct S2 { void f(int i); };
void S2::f(int i)
{
    [&] {};          // OK: by-reference capture default
    [&, i] {};       // OK: by-reference capture, except i is captured by copy
    [&, &i] {};      // Error: by-reference capture when by-reference is the default
    [&, this] {};    // OK, equivalent to [&]
    [&, this, i] {}; // OK, equivalent to [&, i]
}
```

If the capture-default is `=`, subsequent simple captures must begin with `&` or be `*this` (*since C++17*) or `this` (*since C++20*):

```cpp
struct S2 { void f(int i); };
void S2::f(int i)
{
    [=] {};        // OK: by-copy capture default
    [=, &i] {};    // OK: by-copy capture, except i is captured by reference
    [=, *this] {}; // until C++17: Error: invalid syntax
                   // since C++17: OK: captures the enclosing S2 by copy
    [=, this] {};  // until C++20: Error: this when = is the default
                   // since C++20: OK, same as [=]
}
```

Any capture may appear only once, and its name must be different from any parameter name.   
Only lambda-expressions defined at block scope or in a default member initializer may have a capture-default or captures without initializers. For such lambda-expression, the reaching scope is defined as the set of enclosing scopes up to and including the innermost enclosing function (and its parameters). This includes nested block scopes and the scopes of enclosing lambdas if this lambda is nested.

### Generalized Capture (since C++14)

In C++14, you can introduce and initialize new variables in the capture clause, without the need to have those variables exist in the lambda function's enclosing scope. The initialization can be expressed as any arbitrary expression; the type of the new variable is deduced from the type produced by the expression. This feature lets you capture move-only variables (such as `std::unique_ptr`) from the surrounding scope and use them in a lambda.

A capture with an initializer acts as if it declares and explicitly captures a variable declared with type `auto`, whose declarative region is the body of the lambda expression (that is, it is not in scope within its initializer), except that:

if the capture is *by-copy*, the non-static data member of the closure object is another way to refer to that auto variable.
if the capture is *by-reference*, the reference variable's lifetime ends when the lifetime of the closure object ends.

```cpp
int x = 4;
 
auto y = [&r = x, x = x + 1]() -> int   // r is a reference to x
{
    r += 2;
    return x * x;
}(); // updates ::x to 6 and initializes y to 25.

pNums = make_unique<vector<int>>(nums);
//...
        auto a = [ptr = move(pNums)]()
        {
            // use ptr
        };
```

Class members cannot be captured explicitly by a capture without initializer (as mentioned above, only variables are permitted in the capture list):

```cpp
class S
{
    int x = 0;
 
    void f()
    {
        int i = 0;
    //  auto l1 = [i, x] { use(i, x); };      // error: x is not a variable
        auto l2 = [i, x = x] { use(i, x); };  // OK, copy capture
        i = 1; x = 1; l2(); // calls use(0,0)
        auto l3 = [i, &x = x] { use(i, x); }; // OK, reference capture
        i = 2; x = 2; l3(); // calls use(1,2)
    }
};
```

When a lambda captures a member using implicit by-copy capture, it does not make a copy of that member variable: the use of a member variable m is treated as an expression `(*this).m`, and `*this` is always implicitly captured by reference:

```cpp
class S
{
    int x = 0;
 
    void f()
    {
        int i = 0;
 
        auto l1 = [=] { use(i, x); }; // captures a copy of i and
                                      // a copy of the this pointer
        i = 1; x = 1; l1();           // calls use(0, 1), as if
                                      // i by copy and x by reference
 
        auto l2 = [i, this] { use(i, x); }; // same as above, made explicit
        i = 2; x = 2; l2();           // calls use(1, 2), as if
                                      // i by copy and x by reference
 
        auto l3 = [&] { use(i, x); }; // captures i by reference and
                                      // a copy of the this pointer
        i = 3; x = 2; l3();           // calls use(3, 2), as if
                                      // i and x are both by reference
 
        auto l4 = [i, *this] { use(i, x); }; // makes a copy of *this,
                                             // including a copy of x
        i = 4; x = 4; l4();           // calls use(3, 2), as if
                                      // i and x are both by copy
    }
};
```


# Limitations

## Member function templates cannot be virtual

Member function templates cannot be declared virtual. This constraint is imposed because the usual implementation of the virtual function call mechanism uses a fixed-size table with one entry per virtual function. However, the number of instantiations of a member function template is not fixed until the entire program has been translated. Hence, supporting virtual member function templates would require support for a whole new kind of mechanism in C++ compilers and linkers. In contrast, the ordinary members of class templates can be virtual because their number is fixed when a class is instantiated

- [stackoverflow](https://stackoverflow.com/questions/2354210/can-a-class-member-function-template-be-virtual)