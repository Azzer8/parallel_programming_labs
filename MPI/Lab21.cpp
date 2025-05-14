#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int send_buf;
    int recv_buf;
    MPI_Status status;

    // Инициализация MPI
    MPI_Init(&argc, &argv);
    // Получение номера текущего процесса
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Получение общего количества процессов
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Если size = 1, процесс отправит сообщение сам себе.
    if (size == 0) {
        if (rank == 0) { // Обычно rank 0 существует, если MPI_Init прошел успешно
            fprintf(stderr, "Ошибка: Количество процессов не может быть 0.\n");
        }
        MPI_Finalize();
        return 1;
    }
    
    // Каждый процесс будет отправлять свой номер (rank)
    send_buf = rank;

    // Определяем, кому отправлять и от кого получать
    int dest_rank = (rank + 1) % size;
    int source_rank = (rank - 1 + size) % size; // +size для корректной обработки rank=0

    if (rank % 2 == 0) { // Четные процессы
        MPI_Send(&send_buf, 1, MPI_INT, dest_rank, 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_buf, 1, MPI_INT, source_rank, 0, MPI_COMM_WORLD, &status);
    } else { // Нечетные процессы
        MPI_Recv(&recv_buf, 1, MPI_INT, source_rank, 0, MPI_COMM_WORLD, &status);
        MPI_Send(&send_buf, 1, MPI_INT, dest_rank, 0, MPI_COMM_WORLD);
    }
    printf("[%d]: receive message '%d'\n", rank, recv_buf);

    MPI_Finalize();
    return 0;
}