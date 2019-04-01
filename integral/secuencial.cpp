#include <iostream>
#include <cstdlib>
#include <cmath> 

using std::cout;
using std::endl;


int main( int argc, char *argv[] ){

    int a = atoi(argv[1]); // Límite inferior
    int b = atoi(argv[2]); // Límite superior
    // Integral ∫(ln(x)x²)dx
    int partitions = atoi(argv[3]); 
    double rectangle_base = (b-a)/(double)partitions;
    
    double area = 0.0;
    for(int i = 0; i < partitions; i++){
        double x_i = (a + (i * rectangle_base));
        area += rectangle_base * (log( x_i ) * pow( x_i , 2.0));
    }
    
    cout << "Integral: secuencial( a=" << a << "; b=" << b <<"; n=" << partitions << "; ) => " <<  area << endl;
    
    return 0;
}
