#include <mpi.h>
#include <stdio.h>
#include <stdlib.h> // Для EXIT_SUCCESS

// Обычно мастер-процесс имеет ранг 0
#define MASTER_RANK 0

int main(int argc, char** argv) {
    // Инициализация MPI
    MPI_Init(&argc, &argv);

    int world_size;
    // Получение общего количества процессов
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    // Получение ранга текущего процесса
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_rank == MASTER_RANK) {
        // Логика для мастер-процесса
        int received_message;
        MPI_Status status; // Структура для получения информации о сообщении

        for (int source_rank = 0; source_rank < world_size; ++source_rank) {
            if (source_rank == MASTER_RANK) {
                // Мастер не получает сообщение от самого себя в этой схеме
                continue;
            }


            MPI_Recv(&received_message, 1, MPI_INT, source_rank, 0, MPI_COMM_WORLD, &status);
            printf("receive message '%d' from %d\n", received_message, source_rank);
        }

    } else {
        int message_to_send = world_rank;
        MPI_Send(&message_to_send, 1, MPI_INT, MASTER_RANK, 0, MPI_COMM_WORLD);
    }

    // Завершение MPI
    MPI_Finalize();

    return EXIT_SUCCESS;
}