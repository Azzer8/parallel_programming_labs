#include <mpi.h>
#include <stdio.h>
#include <stdlib.h> // Для exit

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Status status;

    // Инициализация MPI
    MPI_Init(&argc, &argv);
    // Получение номера текущего процесса
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Получение общего количества процессов
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Если только один процесс, ему не с кем обмениваться
    if (size <= 1 && rank == 0) {
        MPI_Finalize();
        return 0;
    }
    
    int send_buffer = rank; // Сообщение, которое процесс отправляет - его собственный ранг
    int recv_buffer;

    for (int partner_rank = 0; partner_rank < size; ++partner_rank) {
        if (partner_rank == rank) {
            continue; // Процесс не обменивается сам с собой в этой схеме
        }

        if (rank < partner_rank) {
            // Текущий процесс (rank) имеет меньший ранг, он отправляет первым
            MPI_Send(&send_buffer, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
            // Затем текущий процесс (rank) принимает от partner_rank
            MPI_Recv(&recv_buffer, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, &status);
        } else { // rank > partner_rank
            // Текущий процесс (rank) имеет больший ранг, он принимает первым
            MPI_Recv(&recv_buffer, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, &status);
            // Затем текущий процесс (rank) отправляет partner_rank
            MPI_Send(&send_buffer, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
        }
        
        printf("[%d]: receive message '%d' from %d\n", rank, recv_buffer, partner_rank);
    }

    // Завершение MPI
    MPI_Finalize();
    return 0;
}