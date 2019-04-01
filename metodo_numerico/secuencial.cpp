#include <iostream>
#include <cstdlib>
#include <cmath>
#include <iomanip>

using std::cout;
using std::endl;
using std::setprecision;

double norma(double *y){
    double y1=fabs(y[0]);
	double y2=fabs(y[1]);
	double y3=fabs(y[2]);
	double norma_y=fmax(fabs(y[0]),fmax(fabs(y[1]),fabs(y[2])));
	return norma_y;
};

void factorLU(double matrix[3][3],double **matrix_L,double **matrix_U,int n){
    double suma = 0;
    for(int k=0;k<n;k++){
        for(int s=0;s<k;s++){
            suma = suma + matrix_L[k][s]*matrix_U[s][k];
        }
        matrix_U[k][k] = matrix[k][k]-suma;
        for(int j=k;j<n;j++){
            suma = 0;
            for(int s=0; s<k;s++){
                suma = suma + matrix_L[k][s]*matrix_U[s][j];
            }
            matrix_U[k][j] = (matrix[k][j]-suma)/matrix_L[k][k]; 
        }
        for(int i=k;i<n;i++){
            suma = 0;
            for(int s=0;s<k;s++){
                suma = suma + matrix_L[i][s]*matrix_U[s][k];
            }
            matrix_L[i][k] = (matrix[i][k]-suma)/matrix_U[k][k];
        }
    }
};

double* solveTriangularMatrix(double **matrix, double *b){
    //si el elemento en la esquina superior derecha es cero es triangular inferior (tomando un elemento en la parte triangular superior)
    double *x= new double[3];
    double suma;
    int n=3;
    if(matrix[0][2] == 0){
        for(int i=0;i<3;i++){
            suma=0;
            for(int j=0;j<i;j++){
                suma = suma + (matrix[i][j]*x[j]);
            }
            x[i] = (b[i] - suma)/matrix[i][i];
        }

        return x;
    }
    else{
        //si el elemento en la esquina inferior izq es cera es triangular superior
        if(matrix[2][0] == 0){
            for(int i=2;i>-1;i--){
                suma=0;
                for(int j=i;j<3;j++){
                    suma = suma + (matrix[i][j]*x[j]);
                }
                x[i] = (b[i] - suma)/matrix[i][i];
            }

            return x;
        }
    }
    
};

double* factorSolveLU( double matrix[3][3], double x[3] ){
    
    //LU factorization
    int n=3;
    double *z;
    double *y;
    double *x_=x;
    double **matrix_L = new double*[n];
    double **matrix_U = new double*[n];

    for(int i=0;i<n;i++){
        matrix_L[i]=new double[n];
        matrix_U[i]=new double[n];            
    }
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            matrix_L[i][j] = 0.0;
            matrix_U[i][j] = 0.0;
        }
    }
    for(int i=0;i<n;i++){
        matrix_U[i][i]=1.0;
        matrix_L[i][i]=1.0; 
    }

    factorLU(matrix,matrix_L,matrix_U,3);
    //  Lz = x
    z = solveTriangularMatrix(matrix_L,x_);
    // Uy = z
    y = solveTriangularMatrix(matrix_U, z);

    return y;
};


int main( int argc, char *argv[] ){

    /**
     * Código tomado y adaptado de un taller previo que presenté.
     * https://github.com/vvbv/Taller-5-AMN
     **/

    double matrix[3][3] = {{6,5,-5},{2,6,-2},{2,5,-1}};
    int iterations = atoi( argv[1] );
    double *x_= new double[3];
    x_[0]=3;
    x_[1]=7;
    x_[2]=-13;
          
	double *y;
    double last_residue = 0;
	for( int i = 0; i < iterations; i++ ){
        // Resolution between Lz = x
        y = factorSolveLU(matrix,x_);
        double r = y[0] / (double) x_[0];
        x_[0] = y[0] / (double) norma( y );
        x_[1] = y[1] / (double) norma( y );
        x_[2] = y[2] / (double) norma( y );
        last_residue = r;
    }

    cout << "Ite: " << iterations << " => [" 
             << setprecision(5) << x_[0] << ", " 
             << setprecision(5) << x_[1] << ", " 
             << setprecision(5) << x_[2] << "]" << " R: " 
             << setprecision(5) << last_residue << endl;

    return 0;
}
