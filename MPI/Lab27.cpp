#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    /* Разберёмся, есть ли у процесса родитель */
    MPI_Comm parent_comm;
    MPI_Comm_get_parent(&parent_comm);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    /* ---------------------------------------------------------- */
    /* 1. Ветка «родительских» процессов (parent_comm == NULL)    */
    /* ---------------------------------------------------------- */
    if (parent_comm == MPI_COMM_NULL)
    {
        /* Ранг-0 читает число n и рассылает остальным родителям */
        int n = 0;
        if (world_rank == 0)
        {
            if (scanf("%d", &n) != 1 || n < 0)
            {
                fprintf(stderr, "Incorrect n\n");
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
        }
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

        /* Каждый «родитель» выводит свою информацию                */
        printf("I am %d process from %d processes!\n", world_rank, world_size);
        printf("My parent is none.\n");

        /* Только ранг-0 реально порождает дочерние процессы         */
        if (world_rank == 0 && n > 0)
        {
            /* Аргументы для дочернего процесса (тот же исполняемый файл) */
            char *spawn_argv[] = {NULL};           /* без параметров */

            MPI_Comm intercomm;
            MPI_Info info;
            MPI_Info_create(&info);

            /* Порождаем exactly n дочерних процессов */
            int errcodes[n];
            MPI_Comm_spawn(argv[0],        /* та же программа          */
                           spawn_argv,     /* аргументы                 */
                           n,              /* число дочерних процессов  */
                           info, 0,        /* info, root                */
                           MPI_COMM_SELF,  /* коллективность = только я */
                           &intercomm,
                           errcodes);

            /* После порождения можем, например, дождаться,
               пока дети закончат, но нам это не требуется.
               Закроем intercomm, чтобы корректно завершиться.      */
            MPI_Comm_disconnect(&intercomm);
            MPI_Info_free(&info);
        }
    }
    /* ---------------------------------------------------------- */
    /* 2. Ветка «дочерних» процессов (parent_comm != NULL)         */
    /* ---------------------------------------------------------- */
    else
    {
        /* У детей свой собственный COMM_WORLD размером n           */
        printf("I am %d process from %d processes!\n", world_rank, world_size);

        /* В задаче в качестве «родителя» ждут 0-й процесс из       */
        /* исходного (родительского) мира, поэтому выводим ‘0’.     */
        printf("My parent is 0.\n");

        /* Завершаемся после печати; корректно разъединяемся        */
        MPI_Comm_disconnect(&parent_comm);
    }

    MPI_Finalize();
    return 0;
}