#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STRING_LEN 100
#define ALPHABET_SIZE 26 // 'a' to 'z'

int main(int argc, char *argv[]) {
    int rank, num_procs;
    int n;                                 // Длина строки
    char input_string[MAX_STRING_LEN + 1]; // +1 для нулевого символа
    int char_counts[ALPHABET_SIZE];        // Массив для подсчета каждого символа 'a'...'z'

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // 1. Ввод данных процессом 0
    if (rank == 0) {
        // Чтение длины строки n
        if (scanf("%d", &n) != 1) {
            fprintf(stderr, "Ошибка: Не удалось прочитать длину строки n.\n");
            MPI_Abort(MPI_COMM_WORLD, 1); // Прервать все процессы MPI
        }

        if (n < 1 || n > MAX_STRING_LEN) {
            fprintf(stderr, "Ошибка: Длина строки n должна быть от 1 до %d.\n", MAX_STRING_LEN);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        if (scanf("%s", input_string) != 1) {
            fprintf(stderr, "Ошибка: Не удалось прочитать строку.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    // 2. Рассылка длины строки n с помощью MPI_Bcast
    // MPI_Bcast(buffer, count, datatype, root, comm)
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // 3. Рассылка самой строки input_string с помощью MPI_Bcast
    MPI_Bcast(input_string, n + 1, MPI_CHAR, 0, MPI_COMM_WORLD);

    // 4. Каждый процесс подсчитывает количество вхождений символов
    // Инициализация массива счетчиков нулями
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        char_counts[i] = 0;
    }

    for (int i = 0; i < n; ++i) { // Идем по первым n символам строки
        char ch = input_string[i];
        if (ch >= 'a' && ch <= 'z') {
            char_counts[ch - 'a']++;
        }
    }

    // 5. Вывод результатов
    if (rank == 0) {
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            if (char_counts[i] > 0) {
                printf("%c = %d\n", (char)('a' + i), char_counts[i]);
            }
        }
    }

    MPI_Finalize();
    return 0;
}