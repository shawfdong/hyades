#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char* argv[])
{
    int rank, size;
    char *cpu_name;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    cpu_name = (char *)calloc(80, sizeof(char));
    gethostname(cpu_name, 80);
    printf("Hello, I am %d of %d running on %s\n", rank, size, cpu_name);
    MPI_Finalize();

    return 0;
}
