#define ONE 1
#define TWO 2 

int main(){
    int x = (TWO ? 10 : 5) + ONE + TWO;  // will directly result in 13
}