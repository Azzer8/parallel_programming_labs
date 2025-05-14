#include <mpi.h>
#include <stdio.h>
#include <stdlib.h> // Для malloc, free, MPI_Abort

// Главная функция
int main(int argc, char *argv[]) {
    int rank, num_procs;
    int n;

    double *A_flat = NULL; // Вся матрица A, только на root процессе
    double *C_flat = NULL; // Вся матрица C, только на root процессе для результата
    
    double *local_A_strip = NULL; // Локальная "полоса" (набор строк) матрицы A для каждого процесса
    double *local_C_strip = NULL; // Локальная "полоса" (набор строк) матрицы C для каждого процесса
    double *B_broadcasted = NULL; // Вся матрица B, будет разослана и доступна всем процессам

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // Шаг 1: Процесс 0 читает размер матрицы n
    if (rank == 0) {
        if (scanf("%d", &n) != 1) {
            fprintf(stderr, "Root (rank 0): Failed to read matrix size n.\n");
            MPI_Abort(MPI_COMM_WORLD, 1); // Прервать все MPI процессы
        }
    }

    // Шаг 2: Рассылка n всем процессам
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Шаг 3: Проверка корректности n и делимости на num_procs
    if (n < 1 || n > 10) { // Ограничение из задания
        if (rank == 0) {
            fprintf(stderr, "Error: Matrix size n must be between 1 and 10. Got %d.\n", n);
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (n % num_procs != 0) {
        if (rank == 0) {
            fprintf(stderr, "Error: For this MPI_Scatter/Gather implementation, "
                            "matrix size n (%d) must be divisible by "
                            "the number of processes (%d).\n", n, num_procs);
            fprintf(stderr, "Please run with a number of processes that is a divisor of n.\n");
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Расчет количества строк и элементов на процесс
    int rows_per_proc = n / num_procs;
    // Поскольку n >= 1 и n % num_procs == 0, то num_procs <= n.
    // Следовательно, rows_per_proc >= 1.
    // И elements_per_proc_data = rows_per_proc * n >= 1 * n >= 1.
    int elements_per_proc_data = rows_per_proc * n;

    // Шаг 4: Выделение памяти
    // Матрица B нужна всем процессам
    B_broadcasted = (double*)malloc(n * n * sizeof(double));
    if (!B_broadcasted) { perror("Failed to allocate B_broadcasted"); MPI_Abort(MPI_COMM_WORLD, 1); }

    // Локальные полосы для A и C нужны всем процессам (кроме root, если он не участвует в вычислениях, но здесь участвует)
    local_A_strip = (double*)malloc(elements_per_proc_data * sizeof(double));
    local_C_strip = (double*)malloc(elements_per_proc_data * sizeof(double));
    if (!local_A_strip || !local_C_strip) { perror("Failed to allocate local_A_strip or local_C_strip"); MPI_Abort(MPI_COMM_WORLD, 1); }

    if (rank == 0) {
        A_flat = (double*)malloc(n * n * sizeof(double));
        C_flat = (double*)malloc(n * n * sizeof(double));
        if (!A_flat || !C_flat) { perror("Root: Failed to allocate A_flat or C_flat"); MPI_Abort(MPI_COMM_WORLD, 1); }

        // Чтение матриц A и B на процессе 0
        // Матрицу B читаем сразу в буфер B_broadcasted, который потом будет разослан
        for (int i = 0; i < n * n; ++i) {
            if (scanf("%lf", &A_flat[i]) != 1) {
                fprintf(stderr, "Root: Failed to read element for matrix A.\n");
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
        }
        for (int i = 0; i < n * n; ++i) {
             if (scanf("%lf", &B_broadcasted[i]) != 1) { // Читаем B на root
                fprintf(stderr, "Root: Failed to read element for matrix B.\n");
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
        }
    }

    // Шаг 5: Распределение строк матрицы A с помощью MPI_Scatter
    // MPI_Scatter(send_buffer, send_count, send_datatype, recv_buffer, recv_count, recv_datatype, root_process, communicator)
    // send_count и recv_count здесь - это количество элементов, которое отправляется/принимается одним процессом.
    MPI_Scatter(A_flat, elements_per_proc_data, MPI_DOUBLE,
                local_A_strip, elements_per_proc_data, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    // Шаг 6: Рассылка всей матрицы B всем процессам с помощью MPI_Bcast
    MPI_Bcast(B_broadcasted, n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Шаг 7: Локальное вычисление части матрицы C каждым процессом
    for (int i = 0; i < rows_per_proc; ++i) { // i - локальный индекс строки в local_A_strip и local_C_strip
        for (int j = 0; j < n; ++j) {         // j - индекс столбца в результирующей строке C
            double sum = 0.0;
            for (int k = 0; k < n; ++k) {     // k - внутренний индекс для скалярного произведения строк и столбцов
                sum += local_A_strip[i * n + k] * B_broadcasted[k * n + j];
            }
            local_C_strip[i * n + j] = sum;
        }
    }
    
    // Шаг 8: Сбор локальных частей матрицы C на процессе 0 с помощью MPI_Gather
    // MPI_Gather(send_buffer, send_count, send_datatype, recv_buffer, recv_count, recv_datatype, root_process, communicator)
    // recv_count здесь - это количество элементов, принимаемых от *каждого* процесса.
    MPI_Gather(local_C_strip, elements_per_proc_data, MPI_DOUBLE,
               C_flat, elements_per_proc_data, MPI_DOUBLE, 
               0, MPI_COMM_WORLD);

    // Шаг 9: Процесс 0 выводит результирующую матрицу C
    if (rank == 0) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                // Используем %g для вывода: убирает ".0" для целых чисел, иначе печатает как есть.
                printf("%g%s", C_flat[i * n + j], (j == n - 1) ? "" : " ");
            }
            printf("\n");
        }
    }

    // Шаг 10: Освобождение выделенной памяти
    if (rank == 0) {
        free(A_flat);
        free(C_flat);
    }
    free(B_broadcasted);
    free(local_A_strip);
    free(local_C_strip);

    MPI_Finalize();
    return 0;
}