#include <mpi.h>
#include <stdio.h>
#include <string.h> /* Para strlen */
/* Definindo o grafo da aplicação antes de executar */
int numeroDeTarefas = 6;
int matrizVizinhanca[6][6] = {{0, 1, 1, 0, 0, 0}, {1, 0, 1, 1, 1, 0},
                              {1, 1, 0, 0, 0, 1}, {0, 1, 0, 0, 0, 0},
                              {0, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0}};
/* Retorna o número de vizinhos da tarefa my_rank */
int contaNumeroDeVizinhos(int my_rank) {
  int i;
  int contador = 0;
  for (i = 0; i < numeroDeTarefas; i++)
    if (matrizVizinhanca[my_rank][i] == 1)
      contador++;
  return contador;
}
/* Programa principal */
int main(int argc, char **argv) {
  int i, numeroDeVizinhos, my_rank, source, tag = 50;
  char message[100] = "Oi!";
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  numeroDeVizinhos = contaNumeroDeVizinhos(my_rank);
  if (my_rank == 0) { /* Enviando para todos os vizinhos de my_rank */
    for (i = 0; i < numeroDeTarefas; i++)
      if (matrizVizinhanca[my_rank][i] == 1) {
        printf("[No: %d]: Enviando mensagem para %d\n", my_rank, i);
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, i, tag,
                 MPI_COMM_WORLD);
      }
    /* Recebendo de todos os vizinhos de my_rank */
    for (i = 0; i < numeroDeVizinhos; i++) {
      MPI_Recv(message, 100, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD,
               &status);
      printf("[No: %d] Recebendo msg de %d\n", my_rank, status.MPI_SOURCE);
    }
  } else {
    /* Recebendo msg de uma tarefa vizinha qualquer */
    MPI_Recv(message, 100, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD,
             &status);
    printf("[No: %d]: Recebendo msg do vizinho %d pela primeira vez\n", my_rank,
           status.MPI_SOURCE);
    /* Enviando para todos os vizinhos de my_rank */
    for (i = 0; i < numeroDeTarefas; i++) {
      if (matrizVizinhanca[my_rank][i] == 1)
        printf("[No: %d]: Enviando mensagem para %d\n", my_rank, i);
      MPI_Send(message, strlen(message) + 1, MPI_CHAR, i, tag, MPI_COMM_WORLD);
    }
    /* Recebendo de todos os vizinhos de my_rank menos 1 */
    for (i = 0; i < (numeroDeVizinhos - 1); i++) {
      MPI_Recv(message, 100, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD,
               &status);
      printf("[No: %d]: Recebendo msg do vizinho %d\n", my_rank,
             status.MPI_SOURCE);
    }
  }
  // Finalização do MPI
  MPI_Finalize();
  return 0;
}
