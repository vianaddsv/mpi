🚀 Estudos em High-Performance Computing (HPC) com MPI

Este repositório contém uma coleção de exercícios práticos desenvolvidos em C utilizando a interface Message Passing Interface (OpenMPI). O objetivo destes códigos é explorar conceitos fundamentais de programação paralela e distribuída, desde a passagem básica de mensagens até à divisão de carga de trabalho (Data Partitioning).

📂 Exercícios Desenvolvidos

1. Comunicação em Anel (Ring Topology)

Um programa clássico de passagem de mensagens onde um "token" (um valor inteiro) é transmitido sequencialmente entre os processos numa estrutura de anel fechado.

Conceitos explorados: MPI_Send, MPI_Recv, topologias lógicas, sincronização de processos e prevenção de deadlocks.

Comportamento: O Rank 0 inicia a mensagem, envia para o Rank 1, que envia para o Rank 2, e assim por diante até regressar ao Rank 0.

2. Produto Escalar de Dois Vetores (Dot Product)

Uma aplicação prática de paralelismo de dados. O programa calcula o produto escalar de dois vetores de grandes dimensões, dividindo o trabalho de forma igualitária entre os núcleos de processamento disponíveis.

Conceitos explorados:

Versão Coletiva: Utilização de MPI_Scatter para distribuir os dados e MPI_Reduce para agrupar e somar os resultados finais.

Versão Ponto-a-Ponto: Implementação manual da distribuição de memória utilizando offsets, MPI_Send e MPI_Recv, e gestão rigorosa de memória com malloc e free.

🛠️ Como Compilar e Executar

Certifica-te de que tens o ecossistema OpenMPI instalado no teu sistema Linux.

1. Compilação:
Utiliza o wrapper do compilador C para MPI:

mpicc nome_do_ficheiro.c -o programa_executavel


2. Execução Básica:
Para executar o programa, especifica o número de processos desejados com a flag -n:

mpiexec -n 4 ./programa_executavel


3. Execução Avançada (Oversubscribe):
Caso queiras testar o programa com mais processos do que o número de núcleos físicos/slots disponíveis no teu CPU, utiliza a flag --oversubscribe:

mpiexec --oversubscribe -n 10 ./programa_executavel


