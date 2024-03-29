# Table of Contents

- [Trivial Types](#trivial-types)
- [Scoped and Unscoped Enums](#scoped-and-unscoped-enums)
- [Narrowing Conversions](#narrowing-conversions)
- [User-provided Functions](#user-provided-functions)
- [Nested Classes](#nested-classes)
- [Default Constructors](#default-constructors)
    - [Implicitly-declared default constructor](#implicitly-declared-default-constructor)
    - [Implicitly-defined default constructor](#implicitly-defined-default-constructor)
- [Constructors](#constructors)
    - [Delegating Constructor](#delegating-constructor)
- [Rule of five/three/zero](#rule-of-fivethreezero)
    - [Rule of zero](#rule-of-zero)
    - [Rule of three](#rule-of-three)
    - [Rule of five](#rule-of-five)
- [`= default`](#default)
- [`= delete`](#delete)
- [`explicit`](#explicit)
- [Auto Generation of Special Member Functions](#auto-generation-of-special-member-functions)
    - [Default Constructor](#default-constructor)
    - [Copy Constructor](#copy-constructor)
    - [Copy-assignment Operator](#copy-assignment-operator)
    - [Move Constructor](#move-constructor)
    - [Move-assignment Operator](#move-assignment-operator)
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
- [`using`](#using)
    - [Type alias](#type-alias)
    - [using-declaration](#using-declaration)
    - [using-directive](#using-directive)
- [`using` vs `typedef`](#using-vs-typedef)
- [Lambda Expressions](#lambda-expressions-since-c11)
- [`inline`](#inline)
    - [`inline` vs `static`](#inline-vs-static)
- [Template Type Deduction Rules](#template-type-deduction-rules)
- [`auto`](#auto)
- [Abbreviated function templates](#abbreviated-function-templates-since-c20)
- [Value Categories](#value-categories)
- [`decltype`](#decltype)
- [Limitations](#tidbits)

# Trivial Types

Trivial types have trivial special member functions. Trivial means that the constructor/operator/destructor is not user-provided and belongs to a class that has 

- no virtual functions or virtual base class,
- no non-trivial base class,
- no non-trivial data members 

Trivial types occupy a contiguous memory region and can be reliably copied with `memcpy`. If it has different access specifiers then the compiler is free to order members with different access specifiers.

- [Microsoft C++ Documentation](https://learn.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-170)


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

# Nested Classes

We can declare a class within another class. Such a class is called a nested class. 

The name of the nested class exists in the scope of the enclosing class, and name lookup from a member function of a nested class visits the scope of the enclosing class after examining the scope of the nested class.  
Like any member of its enclosing class, **the nested class has access to all names (private, protected, etc) to which the enclosing class has access**, but it is otherwise **independent and has no special access to the this pointer of the enclosing class**.  
Declarations in a nested class can use any members of the enclosing class, following the usual usage rules for the non-static members.

**Friend functions defined within a nested class have no special access to the members of the enclosing class** even if lookup from the body of a member function that is defined within a nested class can find the private members of the enclosing class.

```cpp
int x, y; // globals
class enclose // enclosing class
{
    // note: private members
    int x;
    static int s;
public:
    struct inner // nested class
    {
        void f(int i)
        {
            x = i; // Error: can't write to non-static enclose::x without instance
            int a = sizeof x; // Error until C++11,
                              // OK in C++11: operand of sizeof is unevaluated,
                              // this use of the non-static enclose::x is allowed.
            s = i;   // OK: can assign to the static enclose::s
            ::x = i; // OK: can assign to global x
            y = i;   // OK: can assign to global y
        }
 
        void g(enclose p, int i)
        {
            p.x = i; // OK: assign to enclose::x
        }

        friend void h()
        {
            // int a = sizeof x;    // Error: can't access private or protected members from a friend function 
        }
    };
};
```

Out-of-class definitions of the members of a nested class appear in the namespace of the enclosing class.

```cpp
struct enclose
{
    struct inner
    {
        static int x;
        void f(int i);
    };
};
 
int enclose::inner::x = 1;       // definition
void enclose::inner::f(int i) {} // definition
```

Nested classes can be forward-declared and later defined, either within the same enclosing class body, or outside of it.

```cpp
class enclose
{
    class nested1;    // forward declaration
    class nested2;    // forward declaration
    class nested1 {}; // definition of nested class
};
 
class enclose::nested2 {}; // definition of nested class
```

Nested class declarations obey member access specifiers, a private member class cannot be named outside the scope of the enclosing class, **although objects of that class may be manipulated**.

```cpp
class enclose
{
    struct nested // private member
    {
        void g() {}
    };

public:
    static nested f() { return nested{}; }
};
 
int main()
{
    // enclose::nested n1 = enclose::f(); // error: 'nested' is private
 
    enclose::f().g();       // OK: does not name 'nested'
    auto n2 = enclose::f(); // OK: does not name 'nested'
    n2.g();
}
```

- [cppreference](https://en.cppreference.com/w/cpp/language/nested_types)

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

# `= default`

You can default any special member function to instruct the compiler to generate the default implementation. This is also useful to reinstate a special member function whose automatic generation was prevented by its corresponding rules. Default special member functions perform better so we should prefer them over explicitly declaration if we want the default behavior. You can also default a special member function outside the body of the class.

# `= delete`

You can delete special member functions as well as normal member functions and non-member functions to prevent them from being defined or called. It must be deleted as it is declared; you can't delete it later after declaring as you could do with default.  

Deleting of normal member function or non-member functions prevents problematic type promotions from causing an unintended function to be called. This works because deleted functions still participate in overload resolution and provide a better match than the function that could be called after the types are promoted. The function call resolves to the more-specific—but deleted—function and causes a compiler error.

```cpp
// deleted overload prevents call through type promotion of float to double from succeeding.
void call_with_true_double_only(float) =delete;
void call_with_true_double_only(double param) { return; }

template < typename T >
void call_with_true_double_only(T) =delete; //prevent call through type promotion of any T to double from succeeding.

void call_with_true_double_only(double param) { return; } // also define for const double, double&, etc. as needed.
```

# `explicit`

A constructor with a single required parameter is considered an implicit conversion function, meaning it converts the parameter type to the class type. We can use `explicit` keyword to prevent this. A constructor declared `explicit` converts to class type only when [direct-initialization](#direct-initialization) or explicit casting is done. You can also use it for conversion functions.  

For example:

```cpp
struct Z { };

struct X { 
    explicit X(int a){} // X can be constructed from int explicitly
    explicit operator Z (){ // X can be converted to Z explicitly
        return Z{};
    }
};

struct Y{
    Y(int a){} // int can be implicitly converted to Y
    operator Z (){ // Y can be implicitly converted to Z
        return Z{};
    }
};


void foo(X x) { }
void bar(Y y) { }
void baz(Z z) { }

int main(){
    // function calls
    // foo(2);                     // error: no implicit conversion int to X possible
    foo(X(2));                  // OK: direct initialization: explicit conversion
    foo(static_cast<X>(2));     // OK: explicit conversion

    bar(2);                     // OK: implicit conversion via Y(int) 
    bar(Y(2));                  // OK: direct initialization
    bar(static_cast<Y>(2));     // OK: explicit conversion

    // object initialization
    // X x2 = 2;                   // error: no implicit conversion int to X possible
    X x3(2);                    // OK: direct initialization
    X x4 = X(2);                // OK: direct initialization
    X x5 = static_cast<X>(2);   // OK: explicit conversion 

    Y y2 = 2;                   // OK: implicit conversion via Y(int)
    Y y3(2);                    // OK: direct initialization
    Y y4 = Y(2);                // OK: direct initialization
    Y y5 = static_cast<Y>(2);   // OK: explicit conversion

    // conversion function
    X x1{ 0 };
    Y y1{ 0 };

    // baz(x1);                    // error: X not implicitly convertible to Z
    baz(Z(x1));                 // OK: explicit initialization
    baz(static_cast<Z>(x1));    // OK: explicit conversion

    baz(y1);                    // OK: implicit conversion via Y::operator Z()
    baz(Z(y1));                 // OK: direct initialization
    baz(static_cast<Z>(y1));    // OK: explicit conversion
}
```

- [stackoverflow](https://stackoverflow.com/questions/121162/what-does-the-explicit-keyword-mean)

# Auto Generation of Special Member Functions

### Default Constructor

The default constructor is generated if there's no user-declared constructor.

### Copy Constructor 

If there's no user-declared copy constructor, one is declared implicitly. If the class declares a move constructor or a move-assignment operator, the implicitly declared copy constructor is defined as deleted otherwise it is defined as defaulted. If a copy-assignment operator or destructor is explicitly declared, then automatic generation of copy constructor is deprecated.

### Copy-assignment Operator

Similar to copy constructor. Deprecated if the class has an user-declared copy constructor or an user-declared destructor.

### Move Constructor

If the class does not explicitly declare a move constructor, it'll be automatically generated if and only if the class does not have an user-declared copy constructor, copy-assignment operator, move-assignment operator and destructor.


### Move-assignment Operator 

Similar to move constructor.

### Destructor

The destructor is auto-generated if there's no user-declared destructor. They're `noexcept` by default and `virtual` only if base class destructor is virtual.

**Note**: The default move member functions will resort to copy member functions of the data member if the data member being moved is not move enabled.

**Note**: Default generation of special member functions, except default constructor, is not prevented by templated versions of the special member functions .

- [stackoverflow](https://stackoverflow.com/questions/4943958/conditions-for-automatic-generation-of-default-copy-move-ctor-and-copy-move-assi)
- [Microsoft C++ documentation](https://learn.microsoft.com/en-us/cpp/cpp/explicitly-defaulted-and-deleted-functions?view=msvc-170)

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

- If the type T is class type with no [*default constructor*](#default-constructors) or with a **user-declared (until C++11)**, **user-provided or deleted default constructor (since C++11)**, the object is [*default-initialized*](#default-initialization). If no default constructor exists, this leads to a compilation error.
- If the type T is class type with a default constructor that is not **user-declared (until C++11)** **neither user-provided nor deleted (since C++11)** (i.e., a class with an implicitly-defined or defaulted default constructor), the object is [*zero-initialized*](#zero-initialization) and and the semantic constraints for default-initialization are checked, and if T has a *non-trivial default constructor*, the object is [*default-initialized*](#default-initialization).
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
    - no virtual, private or protected base classes (**since C++17**)
    - no virtual member functions
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

# `using`

## Type alias

- `using identifier = type-id;`
- `template < template-parameter-list >`  
`using identifier = type-id ;`

## *using-declaration*

- `using typename(optional) nested-name-specifier unqualified-id ;		(until C++17)`
- `using declarator-list ;		                                        (since C++17)`

These can be used to introduce namespace members into other namespaces and blocks, or to introduce base class members into derived classes.

### In namespace scope

Using-declaration introduces a member of another namespace into current namespace or block scope.

```cpp
#include <iostream>
#include <string>
 
using std::string;
 
int main()
{
    string str = "Example";
    using std::cout;
    cout << str;
}
```

### In class definition

Using-declaration *introduces a member* of a base class into the derived class definition, such as to expose a protected member of base as public member of derived. In this case, nested-name-specifier must name a base class of the one being defined. If the name is the name of an overloaded member function of the base class, all base class member functions with that name are introduced. If the derived class already has a member with the same name, parameter list, and qualifications, the derived class member hides or overrides (doesn't conflict with) the member that is introduced from the base class. 

```cpp
#include <iostream>
 
struct B
{
    virtual void f(int) { std::cout << "B::f\n"; }
    void g(char)        { std::cout << "B::g\n"; }
    void h(int)         { std::cout << "B::h\n"; }
protected:
    int m; // B::m is protected
    typedef int value_type;
};
 
struct D : B
{
    using B::m;          // D::m is public
    using B::value_type; // D::value_type is public
 
    using B::f;
    void f(int) { std::cout << "D::f\n"; } // D::f(int) overrides B::f(int)
 
    using B::g;
    void g(int) { std::cout << "D::g\n"; } // both g(int) and g(char) are visible
 
    using B::h;
    void h(int) { std::cout << "D::h\n"; } // D::h(int) hides B::h(int)
};
 
int main()
{
    D d;
    B& b = d;
 
//  b.m = 2;  // Error: B::m is protected
    d.m = 1;  // protected B::m is accessible as public D::m
 
    b.f(1);   // calls derived f()
    d.f(1);   // calls derived f()
    std::cout << "----------\n";
 
    d.g(1);   // calls derived g(int)
    d.g('a'); // calls base g(char), exposed via using B::g;
    std::cout << "----------\n";
 
    b.h(1);   // calls base h()
    d.h(1);   // calls derived h()
}
```

## *using-directive*

`using namespace namespace-name;`

Using-directives can only be used in namespace and block scope (not class scope). From the point of view of name lookup of any name after a using-directive and until the end of the scope in which it appears, every name from namespace-name is visible *as if* it were *declared in the nearest enclosing namespace which contains both the using-directive and namespace-name*.

It does not add any names to the region where it appears unlike *using-declaration*, and thus does not prevent identical names from being declared.

Using directives are transitive in the sense that if a inside the namespace used in the using-directive there is another using-directive, the effect as if the second directive was declared besides the first.

```cpp
namespace A
{
    int i;
}
 
namespace B
{
    int i;
    int j;
 
    namespace C
    {
        namespace D
        {
            using namespace A; // all names from A injected into global namespace
 
            int j;
            int k;
            int a = i;         // i is B::i, because A::i is hidden by B::i
        }
 
        using namespace D; // names from D are injected into C
                           // names from A are injected into global namespace
 
        int k = 89; // OK to declare name identical to one introduced by a using
        int l = k;  // ambiguous: C::k or D::k since they're in the same scope now
        int m = i;  // ok: B::i hides A::i
        int n = j;  // ok: D::j hides B::j
    }
}
```

If, after using a using-directive for a namespace, that namespace is extended to add additional members/using-directives, those additional members are *also* visible through this using-directive.

```cpp
namespace D
{
    int d1;
    void f(char);
}
using namespace D; // introduces D::d1, D::f, D::d2, D::f,
                   // E::e, and E::f into global namespace!
 
int d1;            // OK: no conflict with D::d1 when declaring
 
namespace E
{
    int e;
    void f(int);
}
 
namespace D            // namespace extension
{
    int d2;
    using namespace E; // transitive using-directive
    void f(int);
}
 
void f()
{
    d1++;    // error: ambiguous ::d1 or D::d1?
    ::d1++;  // OK
    D::d1++; // OK
    d2++;    // OK, d2 is D::d2
 
    e++;     // OK: e is E::e due to transitive using
 
    f(1);    // error: ambiguous: D::f(int) or E::f(int)?
    f('a');  // OK: the only f(char) is D::f(char)
}
```

- [cppreference-using-declaration](https://en.cppreference.com/w/cpp/language/using_declaration)
- [cppreference-using-directives](https://en.cppreference.com/w/cpp/language/namespace#Using-directives)

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

whereas an *alias-declaration* is **not** an *init-statement* and thus was not allowed in contexts which allow *init-statement* **until C++23**. **This inconsistency between *typedef-declaration* and *alias-declaration* has been removed in C++23.**

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
*The implicit capture of `*this` when the capture default is `=` is deprecated. (since C++20)*.  

A lambda expression can *use* a variable without capturing it if the variable
- is a non-local variable or has static storage duration

A lambda expression can *read* a variable without capturing it if the variable 
- is a const non-volatile integral or enumeration type that has been initialized with a constant expression, or
- is `constexpr` and has no mutable members

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
# `inline`

`inline` makes a function an *inline function* which allows it to be defined in multiple translation units. `inline` doesn't have any affect on the ability of the compiler to *inline* a function call, it is upto the compiler to inline the function call or ignore it and inline another call which you didn't even specify.  

`inline`ing a function also *enables* the compiler the ability to *inline* a function call since the compiler doesn't have access to function definitions of object files other than the one it is currently producing and it must have access to the function definition in order to *inline* the call.  

Every translation unit that uses that function must provide its own definition of it

## `inline` vs `static`

Both are very different in what they instruct the compiler to do.  

`inline` allows multiple definitions of the function in different translation units. The compiler will then *inline* the function calls or merge the function definitions from different translation units so that the result function exists only once in the final executable i.e., the functions will have the same address across translation units (and will share their static variables).  

On the other hand, `static` instructs the compiler to generate the function in every translation unit where it is defined and not just share it. This means that the multiple copies of the function exist in the executable (the same function will have different address in different translation units)


- [stackoverflow-inline-functions](https://stackoverflow.com/questions/29796264/is-there-still-a-use-for-inline)
- [stackoverflow-inline-vs-static](https://stackoverflow.com/questions/22102919/static-vs-inline-for-functions-implemented-in-header-files)
- [stackoverflow-inline-variables](https://stackoverflow.com/questions/38043442/how-do-inline-variables-work)
- [cppreference](https://en.cppreference.com/w/cpp/language/inline)

# Template Type Deduction Rules

For a function template of the form 

```cpp
template<typename T>
void f(ParamType param);

f(expr);
```
there are three cases which affect the deduced type for T,

### ParamType is a *reference* or *pointer*, but not a *Universal Reference*

In this case type deduction goes like this,

- If *expr*'s type is a reference, ignore the reference part
- Then pattern-match *expr*'s type against *ParamType* to determine T

For example, for the template

```cpp
template<typename T>
void f(T& param);
```

and we've the following variables 

```cpp
int x = 25;
const int cx = x;
const int& rx = cx;
```

the deduced types for T and param are 

```cpp
f(x);           // T is int, param's type is int& 
f(cx);          // T is const int, param's type is const int& 
f(rx);          // T is const int, param's type is const int&
```

Type deduction works exactly the same way for *rvalue* reference parameters. If we were to change `T&` to `const T&`,

```cpp
template<typename T>
void f(const T& param); // param is now a ref-to-const

int x = 27;             // as before
const int cx = x;       // as before
const int& rx = x;      // as before

f(x);                   // T is int, param's type is const int&
f(cx);                  // T is int, param's type is const int&
f(rx);                  // T is int, param's type is const int&
```

If param were a pointer, things behave similarly,

```cpp
template<typename T>
void f(T* param);       // param is now a pointer

int x = 27;             // as before
const int *px = &x;     // px is a ptr to x as a const int

f(&x);  // T is int, param's type is int*
f(px);  // T is const int,
        // param's type is const int*
```

### ParamType is a *Universal Reference*

These are declared similar to rvalue references, for a function template with type param T, a universal reference has type `T&&`. They behave differently when *lvalues* are passed in,

- If *expr* is lvalue, both T and *ParamType* are deduced to be lvalue references
- Otherwise, if *expr* is rvalue, normal rvalue reference rules apply  

```cpp
template<typename T>
void f(T&& param);      // param is now a universal reference

int x = 27;             // as before
const int cx = x;       // as before
const int& rx = x;      // as before

f(x);                   // x is lvalue, so T is int&,
                        // param's type is also int&

f(cx);                  // cx is lvalue, so T is const int&,
                        // param's type is also const int&

f(rx);                  // rx is lvalue, so T is const int&,
                        // param's type is also const int&

f(27);                  // 27 is rvalue, so T is int,
                        // param's type is therefore int&&
```

### ParamType is neither a pointer nor a reference 

```cpp
template<typename T>
void f(T param);
```

Since param will be a copy of whatever is passed, the following rules govern the type deduced for T,

- If *expr*'s type is reference, ignore the reference part
- If *expr* is *const* or *volatile*, ignore that too 

```cpp
int x = 27;         // as before
const int cx = x;   // as before
const int& rx = x;  // as before

f(x);   // T's and param's types are both int
f(cx);  // T's and param's types are again both int
f(rx);  // T's and param's types are still both int
```

Even here the only the *const*-ness of the passed argument is ignored, the *const*-ness of whatever param points (or refers) to is preserved.

```cpp
template<typename T>
void f(T param);        // param is still passed by value

const char* const ptr = "const pointer to const"

f(ptr);                 // T's type will be const char*
```

### Array Arguments

```cpp
template<typename T>
void f(T param);

const char[] name = "Some name";

f(name);            // T's type is const char* 
```

We can't declare parameters of array type but we can declare parameters of type *reference* to arrays! In which case

```cpp
template<typename T>
void f(T& param);

f(name);        // T's type is const char[size] and param's type  
                // is const char (&)[size]
```

This is useful in, for example, getting size of an array at compile-time,

```cpp
// return size of an array as a compile-time constant. (The
// array parameter has no name, because we care only about
// the number of elements it contains.)
template<typename T, std::size_t N> // see info
constexpr std::size_t arraySize(T (&)[N]) noexcept {
    return N;
}
```

### Function Arguments

Similar to arrays, function types also decay to function pointers and things similar to arrays happen with function types

```cpp
void someFunc(int, double); // someFunc is a function;
                            // type is void(int, double)
template<typename T>
void f1(T param);           // in f1, param passed by value

template<typename T>
void f2(T& param);          // in f2, param passed by ref

f1(someFunc);               // param deduced as ptr-to-func;
                            // type is void (*)(int, double)

f2(someFunc);               // param deduced as ref-to-func;
                            // type is void (&)(int, double)
```

- [Effective Modern C++](https://github.com/vpreethamkashyap/Library/blob/master/Scott%20Meyers-Effective%20Modern%20C%2B%2B_%2042%20Specific%20Ways%20to%20Improve%20Your%20Use%20of%20C%2B%2B11%20and%20C%2B%2B14-O'Reilly%20Media%20(2014).pdf)

# `auto`

`auto` type deduction follows exactly the same rules as template type deduction except for one case. For a variable declared with `auto`, `auto` plays the role of T in the template and the type specifier acts as the ParamType.

```cpp
auto x = 25;            // type specifier is auto
const auto y = 25;      // type specifier is const auto
const auto& z = 25;     // type specifier is const auto& 

auto&& uref1 = x;       // x is int and lvalue,
                        // so uref1's type is int&
auto&& uref2 = cx;      // cx is const int and lvalue,
                        // so uref2's type is const int&
auto&& uref3 = 27;      // 27 is int and rvalue,
                        // so uref3's type is int&&
```

The case where `auto` and template type deduction differ is when you declare a variable 
with *braced-initializer*

```cpp
auto x = {2, 4, 5};     // x's type is std::initializer_list<int>
auto y = {2.0, 5, 5};   // two type deductions, one for y -> std::initializer_list<T>
                        // and second for T, which fails in this case

template<typename T>
void f(T param);

f({2, 5, 5});           // Error! can't deduce type for T
```

So the only real difference between `auto` and template type deduction is that `auto`
assumes that a braced initializer represents a `std::initializer_list`, but template
type deduction doesn’t.

`auto` as return or parameter type follows template type deduction.

# Abbreviated function templates (since C++20)

When `auto` appears in the parameter list of a function declaration or of a function template declaration, the declaration declares a function template, and one 'invented' template parameter for each `auto` is appended to the template parameter list. 

```cpp
void f1(auto); // same as template<class T> void f1(T)
```

Abbreviated function templates can be specialized like all function templates.

# Value Categories

Too much stuff...

- [cppreference](https://en.cppreference.com/w/cpp/language/value_category)

# `decltype`

`decltype` is used to get the type of an expression or a name, it usually tells you the exact type of the expression or name you pass it to like so 

`decltype`'s primary use is when the return type in function templates depends on the parameter types 

```cpp
template<typename T, typename U>
decltype(auto) add(T t, U u){
    return t + u;
};
```

The following rules govern the type returned by `decltype(E)`:

- If `E` is an unparenthesized id-expression (essentially a variable name) or a class member access then `decltype(E)` returns the exact type with which `E` was declared
- Otherwise if `E` is any other expression of type `T` and  
    - if the value category of `E` is *xvalue*, then `decltype` yields `T&&`,
    - if the value category of `E` is *lvalue*, then `decltype` yields `T&`,
    - if the value category of `E` is *prvalue*, then `decltype` yields `T`
    - the above resultant types are subject to **reference collapsing**

```cpp
constexpr int x;
decltype(x) y;          // type of y is const int 
decltype((x)) z;        // type of z is const int&, (x) is lvalue expression

struct A{
    double x;
};

const A* ptr;
decltype((ptr)) p;          // const A* &, reference to pointer to const
decltype(ptr->x) x;         // double
decltype((ptr->x)) x;       // const double&, type of (ptr->x) is const double

decltype(5) x;              // int, 5 is prvalue
decltype("string") str;     // const char (&)[7], string literal is an lvalue stored 
                            // as char[] in memory
decltype(("string")) str;   // ditto

void foo(int);
decltype(foo) f;            // void (int), since name
decltype((foo)) f;          // void (&)(int), since lvalue expression

struct B{
    static int x;
    int y;
    int& lvalref = x;
    int&& rvalref = std::move(x);
    int& otherlvalref = y;
    int&& otherrvalref = 5;
};

B foo();

decltype(B::x) x;           // int
decltype((B::x)) x;         // int&
decltype(B::lvalref) x;     // int&
decltype((B::lvalref)) x;   // int&, lvalue expression
decltype(B::rvalref) x;     // int&&
decltype((B::rvalref)) x;   // int&, rvalref is lvalue
                            // ditto for otherrefs
decltype(B{}.lvalref) x;    // int& unparenthesized member access,  
decltype((B{}.lvalref)) x;  // int&, B{}.lvalref is lvalue expression
                            // because lvalref is not of object type
decltype(B{}.rvalref) x;    // int&&, member access
decltype((B{}.rvalref)) x;  // int&, lvalue expression and reference collapsing
decltype((B{}.x)) x;        // int&, static data member so B{}.x is lvalue
decltype((B{}.y)) x;        // int&&, B{}.y is xvalue since y is non-static

/* same for foo().stuff */
``` 

`decltype` is also useful when you want to use the type of a lambda like so

```cpp
auto comp = [](const auto& left, const auto& right){
        return left < right;
};

// creating a set with custom comparator
std::set<int, decltype(comp)> s{comp};
```

- [cppreference](https://en.cppreference.com/w/cpp/language/decltype)
- [blog post by David Mazieres](https://www.scs.stanford.edu/~dm/blog/decltype.pdf)

# Tidbits

- You can use the non-static data members of a class in unevaluated operands but can't do that for non-static member functions like so

    ```cpp
    struct A{
        int x = 2;
        
        // static const int m = x;          // Error: no this pointer when initializing the static var
        static const int m = sizeof(x);     // OK: x in unevaluated operand

        int foo(){}
    };

    decltype(A::x) x;       // OK: even though x doesn't belong to a class but an object of it

    // decltype(A::foo()) x;   // Error: because even though the call isn't actually made foo()
    //                         // needs the *this* pointer i.e., it must be called on an object 
    ```

    - [cppreference](https://en.cppreference.com/w/cpp/language/data_members#Usage)

- Member function templates cannot be declared virtual. This constraint is imposed because the usual implementation of the virtual function call mechanism uses a fixed-size table with one entry per virtual function. However, the number of instantiations of a member function template is not fixed until the entire program has been translated. Hence, supporting virtual member function templates would require support for a whole new kind of mechanism in C++ compilers and linkers. In contrast, the ordinary members of class templates can be virtual because their number is fixed when a class is instantiated
    - [stackoverflow](https://stackoverflow.com/questions/2354210/can-a-class-member-function-template-be-virtual)

