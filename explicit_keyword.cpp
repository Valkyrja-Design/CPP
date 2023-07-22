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