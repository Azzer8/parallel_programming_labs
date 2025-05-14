#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Процесс 0 выводит общее количество процессов
        std::cout << size << " processes." << std::endl;
    }

    // Синхронизация перед выводом
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank != 0) {
        if (rank % 2 == 1) {
            std::cout << "I am " << rank << " process: FIRST!" << std::endl;
        } else {
            std::cout << "I am " << rank << " process: SECOND!" << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}