#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {


    MPI_Init(&argc, &argv);

    int numProcess;
    MPI_Comm_size(MPI_COMM_WORLD, &numProcess);

    int myRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    MPI_Status status;

    int tag = 200;

    int dest;
    int fonte;
    int rootNode = 0;


    dest = (myRank + 1) % numProcess;
    fonte = (myRank + numProcess - 1) % numProcess;

    if (myRank == rootNode) {
        printf("Im the root and my value is %d\n", fonte);
        MPI_Send(&fonte, 1, MPI_INT, dest,tag, MPI_COMM_WORLD);
        fonte--;

        while(1) {
            MPI_Recv(&fonte, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
            if (fonte == 0) {
                printf("Im the root and this is my end with value %d\n", fonte);
                MPI_Send(&fonte, 1, MPI_INT, dest,tag, MPI_COMM_WORLD);

               break;
            }

            fonte--;
            printf("Im the root and send this value %d to my dest %d\n", fonte, dest);
            MPI_Send(&fonte, 1, MPI_INT, dest,tag, MPI_COMM_WORLD);

        }

    } else {

        printf("Im the %d and my value is %d\n", myRank,fonte);
        MPI_Send(&fonte, 1, MPI_INT, dest,tag, MPI_COMM_WORLD);
        while (1) {
            MPI_Recv(&fonte, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);

            if (fonte == 0) {
                printf("Im the %d and my value is %d and its my end\n", myRank,fonte);
                MPI_Send(&fonte, 1, MPI_INT, dest,tag, MPI_COMM_WORLD);
                break;
            }
            printf("Im the %d and my value is %d and send to dest %d\n", myRank,fonte,dest);

            MPI_Send(&fonte, 1, MPI_INT, dest,tag, MPI_COMM_WORLD);
        }
      }
   MPI_Finalize();

   return 0;
}
