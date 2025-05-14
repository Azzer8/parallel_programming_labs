#include <mpi.h>
#include <stdio.h>
#include <string.h> // Для strlen

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Request send_request, recv_request;
    MPI_Status status;

    // Инициализация MPI
    MPI_Init(&argc, &argv);
    // Получение номера текущего процесса
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Получение общего количества процессов
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Программа требует как минимум 2 процесса
    if (size < 2) {
        if (rank == 0) {
            fprintf(stderr, "Эта программа требует как минимум 2 процесса.\n");
        }
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        // Процесс 0 отправляет сообщение
        char message_to_send[] = "hello"; // Сообщение для отправки
        int message_len = strlen(message_to_send) + 1; // +1 для нулевого символа
        MPI_Isend(message_to_send, message_len, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &send_request);
        MPI_Wait(&send_request, &status);

    } else if (rank == 1) {
        char received_message[100]; // Буфер для приема сообщения, должен быть достаточно большим
        int buffer_size = 100;
        MPI_Irecv(received_message, buffer_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &recv_request);
        MPI_Wait(&recv_request, &status);
        printf("receive message '%s'\n", received_message);
    }

    MPI_Finalize();
    return 0;
}