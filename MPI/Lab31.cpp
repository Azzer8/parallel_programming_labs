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

    /* --- 1. читаем n (только ранг-0) ------------------------------------ */
    int n_threads = 0;
    if (rank == 0) {
        if (scanf("%d", &n_threads) != 1 || n_threads <= 0) {
            fprintf(stderr, "Bad n_threads\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }
    /* --- 2. рассылаем всем процессам ------------------------------------ */
    MPI_Bcast(&n_threads, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int total_hybrid = n_threads * nprocs;

    /* --- 3. создаём n нитей и печатаем сообщение от каждой -------------- */
    omp_set_num_threads(n_threads);

    /* Чтобы вывод не перемешивался, оборачиваем printf критической секцией */
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        #pragma omp critical
        {
            printf("I am %d thread from %d process. "
                   "Number of hybrid threads = %d\n",
                   tid, rank, total_hybrid);
            fflush(stdout);
        }
    }

    MPI_Finalize();
    return 0;
}