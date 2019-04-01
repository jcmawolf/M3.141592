#include <iostream>
#include <cstdlib>
#include <cmath> 
#include <mpi.h> 

using std::cout;
using std::endl;

int main( int argc, char *argv[] ){

    // Initialize the MPI environment
	MPI_Init(NULL, NULL);
	// Find out rank, size
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int a; // Límite inferior
    int b; // Límite superior
    // Función ∫(ln(x)x²)dx
    int partitions; 
    double rectangle_base;
    int num_iterations_by_node;
        
    double data_to_send[5];
    if( world_rank == 0 ){
        a = atoi(argv[1]); 
        data_to_send[0] = (double) a; // Límite inferior

        b = atoi(argv[2]); 
        data_to_send[1] = (double) b; // Límite superior

        partitions = atoi(argv[3]); 
        data_to_send[2] = (double) partitions;

        num_iterations_by_node = partitions / (world_size - 1); 
        data_to_send[3] = (double) num_iterations_by_node;

        rectangle_base = (b-a)/(double)partitions; 
        data_to_send[4] = rectangle_base;
    }

    // Envio
    MPI_Bcast(&data_to_send, 5, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    double area = 0.0;
    if( world_rank != 0 ){
        double partida =  data_to_send[0] + (((data_to_send[3] * world_rank) - data_to_send[3]) * data_to_send[4]);
        for(int i = 0; i < data_to_send[3]; i++){
            double x_i =  (partida + (i * data_to_send[4]));
            area += data_to_send[4] * (log( x_i ) * pow( x_i , 2.0));
        }
    }

    double a_p[world_size];
    MPI_Gather(&area, 1, MPI_DOUBLE, a_p, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    if( world_rank == 0 ){
        
        for( int i = 0; i <= (world_size - 1); i++ ){
            area += a_p[ i ];
        }

        cout << "Integral: distribuida/paralela mod( a=" << a << "; b=" << b <<"; n=" << partitions << " => " << num_iterations_by_node * (world_size - 1) << "; ) => " <<  area << endl;

    }
    MPI_Finalize();

    return 0;
}
