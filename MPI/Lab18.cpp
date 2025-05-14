#include <mpi.h>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int message;
    MPI_Status status;

    if (rank == 0) {
        // Процесс 0 начинает эстафету
        message = 0;
        
        MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        std::cout << "[" << rank << "]: send message '" << message << "' to 1" << std::endl;

        MPI_Recv(&message, 1, MPI_INT, size-1, 0, MPI_COMM_WORLD, &status);
        std::cout << "[" << rank << "]: receive message '" << message << "'" << std::endl;
    }
    else {
        int prev = (rank == 0) ? size-1 : rank-1;
        MPI_Recv(&message, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);
        std::cout << "[" << rank << "]: receive message '" << message << "'" << std::endl;

        message++;

        int next = (rank == size-1) ? 0 : rank+1;
        MPI_Send(&message, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        std::cout << "[" << rank << "]: send message '" << message << "' to " << next << std::endl;
    }

    MPI_Finalize();
    return 0;
}