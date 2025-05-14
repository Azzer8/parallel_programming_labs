#include <mpi.h>
#include <cstdio>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* ---------- читаем N ---------- */
    long long N = 0;
    if (rank == 0) {
        if (scanf("%lld", &N) != 1 || N <= 0) {
            fprintf(stderr, "Bad N\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }
    MPI_Bcast(&N, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    /* ---------- локальный интеграл ---------- */
    double h      = 1.0 / (double) N;
    long long q   = N / size;
    long long rem = N % size;
    long long beg = rank * q + (rank < rem ? rank      : rem);
    long long len = q        + (rank < rem ? 1         : 0);

    double sum = 0.0;
    for (long long i = 0; i < len; ++i) {
        double x = h * (beg + i + 0.5);
        sum += 4.0 / (1.0 + x * x);
    }
    double local_pi = h * sum;

    /* ---------- RMA-окно ---------- */
    MPI_Win   win;
    double   *baseptr = nullptr;
    MPI_Aint  winsz   = (rank == 0) ? sizeof(double) : 0;

    MPI_Win_allocate(winsz, sizeof(double),
                     MPI_INFO_NULL, MPI_COMM_WORLD,
                     &baseptr, &win);

    if (rank == 0)        /* инициализировать аккумулятор */
        *baseptr = 0.0;

    /* ---------- односторонняя аккумуляция ---------- */
    MPI_Win_lock_all(0, win);
    MPI_Accumulate(&local_pi, 1, MPI_DOUBLE,
                0, 0, 1, MPI_DOUBLE, MPI_SUM, win);
    MPI_Win_flush(0, win);
    MPI_Win_unlock_all(win);

    /* --- ⇩  ДОБАВЬТЕ ЭТО  ⇩ ------------------------------------ */
    MPI_Barrier(MPI_COMM_WORLD);     // 1) ждём, пока ВСЕ накопили

    /* 2) делаем запись видимой локальным load-ам процесса-0 */
    MPI_Win_lock(MPI_LOCK_SHARED, 0, 0, win);
    MPI_Win_sync(win);               // синхронизация окна с памятью
    MPI_Win_unlock(0, win);
    /* ----------------------------------------------------------- */

    if (rank == 0)
        printf("%.8f\n", *baseptr);

    MPI_Win_free(&win);   /* освобождает и окно, и память */
    MPI_Finalize();
    return 0;
}