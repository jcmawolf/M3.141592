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

    if( world_rank == 0 ){
        double pi = -1.0;
        int num_iterations = atoi(argv[1]);
        int num_iterations_by_node = num_iterations / (world_size - 1);
        int last_node_iterations = num_iterations_by_node + ( num_iterations - ( num_iterations_by_node * (world_size - 1 ) ) );
        
        //Envio
        for( int i = 1; i < world_size; i++ ){
            if( i != ( world_size - 1 ) ){
                MPI_Send(&num_iterations_by_node, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }else{
                MPI_Send(&last_node_iterations, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }

        //Recepción
        for( int i = 1; i < world_size; i++ ){
            int vals[2];
            MPI_Recv(&vals, 2, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            circle_points += vals[0];
            square_points += vals[1];
        }
        
        pi =  4.0 * ( circle_points / (double) square_points );

        cout << "Pi: Monte Carlo, distribuida/paralela( k=" << num_iterations << " ) => " <<  pi << endl;

    }else{
        int num_node_iterations = 0;
        MPI_Recv(&num_node_iterations, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        /** 
         * Se evita que la primer semilla sea igual para ambos
         * puntos. Se espera que el número de iteraciones sea
         * menor al valor máximo de un entero.
         **/
        x = randNumber(seed * INT_MAX); //Desborde de los enteros
        y = randNumber(seed * -INT_MAX); //Desborde de los enteros
        for (int k = 1; k <= num_node_iterations; k++) {
            double d = (x*x + y*y);
            if( d <= 1.0 ){
                circle_points++;
            }
            square_points++;
            seed = time(NULL) * d; // Calculo de nueva semilla, multiplicada por d para adicionar algo de sal. 
            x = randNumber(seed - k); // Desechados en la última iteración, permiten el ahorro de la 
            y = randNumber(seed + k); // comprobación por iteración de si k == 0
        }
        int vals[2];
        vals[0] = circle_points;
        vals[1] = square_points;
        MPI_Send(&vals, 2, MPI_INT, 0, 0, MPI_COMM_WORLD);

    }
    MPI_Finalize();
    return 0;
}
