// void foo(int x){            // will generate "multiple definitions" linker error
//     /* do stuff */
// }

inline void goo(int x){     // allows multiple definitions in different translation units
    /* doo stuff */
}

int main(){
    goo(2);
}