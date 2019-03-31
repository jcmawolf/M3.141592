#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <climits>
#include <cstdlib>

using std::cout;
using std::endl;

double randNumber(int seed){
    srand(time(NULL) + seed);
    return (double) rand() / ( (double) RAND_MAX + 1);
}

int main( int argc, char *argv[] ){

    int num_iterations = atoi(argv[1]);
    int circle_points = 0;
    int square_points = 0;
    double pi = -1.0;

    double x, y;
    int seed = time(NULL); //Cantidad de segundos desde 1970-01-01
    /** 
     * Se evita que la primer semilla sea igual para ambos
     * puntos. Se espera que el número de iteraciones sea
     * menor al valor máximo de un entero.
     **/
    x = randNumber(seed * INT_MAX); //Desborde de los enteros
    y = randNumber(seed * -INT_MAX); //Desborde de los enteros
    for (int k = 1; k <= num_iterations; k++) {
        double d = (x*x + y*y);
        if( d <= 1.0 ){
            circle_points++;
        }
        square_points++;
        seed = time(NULL) * d; // Calculo de nueva semilla, multiplicada por d para adicionar algo de sal. 
        x = randNumber(seed - k); // Desechados en la última iteración, permiten el ahorro de la 
        y = randNumber(seed + k); // comprobación por iteración de si k == 0
    }

    pi =  4.0 * ( circle_points / (double) square_points );

    cout << "Pi: Monte Carlo, secuencial( k=" << num_iterations << " ) => " <<  pi << endl;

    return 0;
}
