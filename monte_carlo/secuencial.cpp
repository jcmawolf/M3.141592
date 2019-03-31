#include <iostream>
#include <stdlib.h> 

using std::cout;
using std::endl;

double randNumber(){
    return (double) rand() / ( (double) RAND_MAX + 1);
}

int main(){
    double c, n;
    
    for (c = 1; c <= 10; c++) {
        n = randNumber();
        cout << n << endl;
    }

    return 0;
}
