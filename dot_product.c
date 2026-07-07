#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>


int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int *vectorA, *vectorB, *subVectorA, *subVectorB = NULL;
    int numProcess, myRank, registerByVector;
    int subVectorProduct = 0;
    int totalProduct = 0;
    int totalPosition = 10;
    int root = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &numProcess);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Status status;

    if (totalPosition % numProcess != 0) {
      printf("cannot start with invalid number of process : %d to length of "
             "vector (1xN) : %d\n",
             numProcess, totalPosition);
      MPI_Finalize();
      return 0;
    }

    registerByVector = totalPosition/numProcess;
    subVectorA = (int *)(malloc(registerByVector * sizeof(int)));
    subVectorB = (int *)(malloc(registerByVector * sizeof(int)));


    if (myRank == root) {
     vectorB = (int *)(malloc(totalPosition * sizeof(int)));
     vectorA = (int *)(malloc(totalPosition * sizeof(int)));
     for (int i = 0; i < totalPosition; i++) {
         vectorA[i] = 1;
         vectorB[i] = 3;
         subVectorProduct += vectorA[i] * vectorB[i];
     }

     for (int dest = 1; dest < numProcess; dest++) {
        int offset = dest * registerByVector;
        MPI_Send(&vectorA[offset], registerByVector, MPI_INT, dest, 0,MPI_COMM_WORLD);
        MPI_Send(&vectorB[offset], registerByVector, MPI_INT, dest, 1,MPI_COMM_WORLD);
     }

     totalProduct = subVectorProduct;
     for (int source = 1; source < numProcess; source++) {
        MPI_Recv(&subVectorProduct, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
        totalProduct += subVectorProduct;
     }
     printf("\n==============================================\n");
     printf("Final Dot Product: %d\n", totalProduct);
     printf("==============================================\n");
     free(vectorA);
     free(vectorB);

    } else {
        MPI_Recv(subVectorA, registerByVector, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(subVectorB, registerByVector, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        for (int i = 0; i < registerByVector; i++) {
            subVectorProduct += subVectorA[i] * subVectorB[i];
        }
        printf("[Process %d] with product: %d\n", myRank, subVectorProduct);
        MPI_Send(&subVectorProduct, 1, MPI_INT, 0, 2,MPI_COMM_WORLD);

   }

     free(subVectorA);
     free(subVectorB);

     MPI_Finalize();


}
