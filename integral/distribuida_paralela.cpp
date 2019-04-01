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

    if( world_rank == 0 ){
        
        int a = atoi(argv[1]); // Límite inferior
        int b = atoi(argv[2]); // Límite superior
        // Función ∫(ln(x)x²)dx
        int partitions = atoi(argv[3]); 
        double rectangle_base = (b-a)/(double)partitions;

        int num_iterations_by_node = partitions / (world_size - 1);
        int last_node_iterations = num_iterations_by_node + ( partitions - ( num_iterations_by_node * (world_size - 1 ) ) );
        
        //Envio
        for( int i = 1; i < world_size; i++ ){
            double data_to_send[3];
            data_to_send[0] =  a + (((num_iterations_by_node * i) - num_iterations_by_node) * rectangle_base); // Partida
            data_to_send[2] = rectangle_base;
            if( i != ( world_size - 1 ) ){
                data_to_send[1] = num_iterations_by_node;
                MPI_Send(&data_to_send, 3, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            }else{
                data_to_send[1] = last_node_iterations;
                MPI_Send(&data_to_send, 3, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            }
        }

        //Recepción
        double total_area = 0;
        for( int i = 1; i < world_size; i++ ){
            double p_area;
            MPI_Recv(&p_area, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_area += p_area;
        }

        cout << "Integral: distribuida/paralela( a=" << a << "; b=" << b <<"; n=" << partitions << "; ) => " <<  total_area << endl;

    }else{
        double data[3];
        MPI_Recv(&data, 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        double area = 0.0;
        for(int i = 0; i < data[1]; i++){
            double x_i = (data[0] + (i * data[2]));
            area += data[2] * (log( x_i ) * pow( x_i , 2.0));
        }

        MPI_Send(&area, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

    }
    MPI_Finalize();
    return 0;
}
