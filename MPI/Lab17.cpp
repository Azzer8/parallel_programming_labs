#include <mpi.h>
#include <iostream>
#include <cstring>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0) {
            std::cerr << "This program requires at least 2 processes!" << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        // Процесс 0 отправляет сообщение процессу 1
        const char* message = "Hi";
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    } 
    else if (rank == 1) {
        // Процесс 1 принимает сообщение
        char received_message[100];
        MPI_Status status;
        MPI_Recv(received_message, 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        
        // Выводим полученное сообщение
        std::cout << "receive message '" << received_message << "'" << std::endl;
    }

    MPI_Finalize();
    return 0;
}