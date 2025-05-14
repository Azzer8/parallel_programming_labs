#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int    rank, num_procs;
    long long n_intervals;          /* точность */
    double local_sum, global_sum_pi;
    double h, x;
    int    i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    /* ---------- читаем N (только ранг-0) ---------- */
    if (rank == 0) {
        if (scanf("%lld", &n_intervals) != 1 || n_intervals <= 0) {
            fprintf(stderr, "Ошибка: некорректное N.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    /* ---------- рассылаем N ---------- */
    MPI_Bcast(&n_intervals, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    /* ---------- синхронизируемся и запускаем секундомер ---------- */
    MPI_Barrier(MPI_COMM_WORLD);
    double t_start = MPI_Wtime();

    /* ---------- вычисляем локальную сумму ---------- */
    h         = 1.0 / (double)n_intervals;
    local_sum = 0.0;

    for (i = rank; i < n_intervals; i += num_procs) {
        x = h * ((double)i + 0.5);
        local_sum += (4.0 / (1.0 + x * x));
    }
    local_sum *= h;

    /* ---------- редукция ---------- */
    MPI_Reduce(&local_sum, &global_sum_pi,
               1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    /* ---------- останавливаем секундомер ---------- */
    MPI_Barrier(MPI_COMM_WORLD);
    double t_end = MPI_Wtime();
    double elapsed = t_end - t_start;   /* время каждого процесса */

    /* ---------- вывод ---------- */
    if (rank == 0) {
        printf("%.8f\n", global_sum_pi);
        printf("Time: %.6f sec (P=%d)\n", elapsed, num_procs);
    }

    MPI_Finalize();
    return 0;
}


#include <vector>
#include <cstdint>

struct RunResult {
    int N;   // параметр точности
    int threads;
    double seconds; // сюда заносим измеренное время
};

// таблица (комментарий слева — номер строки из задания)
std::vector<RunResult> results = {
    {100, 1, 0.000017},  // 57
    {10000000, 1, 0.041922},  // 58
    {400000000, 1, 1.109832},  // 59

    {100, 2, 0.000017},  // 60
    {10000000,   2, 0.015525},  // 61
    {400000000, 2, 0.536624},  // 62

    {100, 4, 0.000130},  // 63
    {10000000, 4, 0.024273},  // 64
    {400000000, 4, 0.358922},  // 65

    {100, 6, 0.000222},  // 66
    {10000000, 6, 0.009963},  // 67
    {400000000, 6, 0.292622},  // 68

    {100, 8, 0.000503},  // 69
    {10000000, 8, 0.011807},  // 70
    {400000000, 8, 0.276192},  // 71

    {100, 10, 0.000888},  // 72
    {10000000, 10, 0.017841},  // 73
    {400000000, 10, 0.303422},  // 74

    {100, 12, 0.000924},  // 75
    {10000000, 12, 0.011825},  // 76
    {400000000, 12, 0.288956}   // 77
};