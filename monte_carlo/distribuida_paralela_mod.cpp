#include <iostream>
#include <ctime>
#include <stdlib.h> 
#include <climits>
#include <mpi.h> 
#include <cstdlib>

using std::cout;
using std::endl;

double randNumber(int seed){
    srand(time(NULL) + seed);
    return (double) rand() / ( (double) RAND_MAX + 1);
}

int main( int argc, char *argv[] ){

    // Initialize the MPI environment
	MPI_Init(NULL, NULL);
	// Find out rank, size
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int circle_points = 0;
    int square_points = 0;

    double x, y;
    int seed = time(NULL); //Cantidad de segundos desde 1970-01-01
    int num_iterations_by_node = -1;
    int num_iterations = -1;
    double pi = 0;
    if( world_rank == 0 ){
        num_iterations = atoi(argv[1]);
        num_iterations_by_node = num_iterations / (world_size - 1);
    }

    // Envio
    MPI_Bcast(&num_iterations_by_node, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double ppi;
    if( world_rank != 0 ){
        /** 
         * Se evita que la primer semilla sea igual para ambos
         * puntos. Se espera que el número de iteraciones sea
         * menor al valor máximo de un entero.
         **/
        x = randNumber(seed * INT_MAX + world_rank); //Desborde de variable intencional
        y = randNumber(seed * -INT_MAX  + world_rank); //Desborde de variable intencional
        for (int k = 1; k <= num_iterations_by_node; k++) {
            double d = (x*x + y*y);
            if( d <= 1.0 ){
                circle_points++;
            }
            square_points++;
            seed = time(NULL) * d; // Calculo de nueva semilla, multiplicada por d para adicionar algo de sal. 
            x = randNumber(seed - k + world_rank); // Desechados en la última iteración, permiten el ahorro de la 
            y = randNumber(seed + k + world_rank); // comprobación por iteración de si k == 0
        }
        ppi = circle_points/(double) square_points;
        cout << ppi << endl;
    }

    double reciv_buff[3];
    MPI_Gather(&ppi, 1, MPI_DOUBLE, reciv_buff, 3, MPI_INT, 0, MPI_COMM_WORLD);

    if( world_rank == 0 ){
        cout << reciv_buff[0] << " " << reciv_buff[1] << " " << reciv_buff[2] << endl;
        pi =  4.0 * (  reciv_buff[0] +  reciv_buff[1] +  reciv_buff[2] );
        cout << "Pi: Monte Carlo, distribuida/paralela( k=" << num_iterations << " ) => " <<  pi << endl;
    }
    MPI_Finalize();

    return 0;
}
