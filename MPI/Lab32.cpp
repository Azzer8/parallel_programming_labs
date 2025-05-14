#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    /* ---------- читаем N (только ранг-0) ---------- */
    long long N = 0;
    if (rank == 0) {
        if (scanf("%lld", &N) != 1 || N <= 0) {
            fprintf(stderr, "Bad N\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }
    MPI_Bcast(&N, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    /* ---------- интеграл в каждом процессе ---------- */
    double h   = 1.0 / (double)N;          /* ширина прямоугольника  */
    double sum = 0.0;

    /* простое блочное разбиение диапазона по процессам */
    long long chunk = N / nprocs;
    long long rest  = N % nprocs;
    long long first = rank * chunk + (rank < rest ? rank      : rest);
    long long localN= chunk           + (rank < rest ? 1       : 0);

    /* параллельная OpenMP-редукция внутри процесса */
    #pragma omp parallel for reduction(+:sum) schedule(static)
    for (long long i = 0; i < localN; ++i) {
        double x = h * (first + i + 0.5);
        sum += 4.0 / (1.0 + x * x);
    }
    double local_pi = h * sum;

    /* ---------- глобальная редукция ---------- */
    double pi = 0.0;
    MPI_Reduce(&local_pi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("%.8f\n", pi);

    MPI_Finalize();
    return 0;
}