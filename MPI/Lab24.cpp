#include <mpi.h>
#include <stdio.h>
#include <math.h> // Для fabs, если нужно будет для сравнения, но не для самого расчета
#include <stdlib.h> // Для MPI_Abort

int main(int argc, char *argv[]) {
    int rank, num_procs;
    long long n_intervals; // Количество интервалов для точности (может быть большим)
    double local_sum, global_sum_pi;
    double h, x;
    int i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // 1. Процесс 0 читает N (количество интервалов)
    if (rank == 0) {
        if (scanf("%lld", &n_intervals) != 1 || n_intervals <= 0) {
            fprintf(stderr, "Ошибка: Не удалось прочитать N или N некорректно.\n");
            n_intervals = 0; // Установить невалидное значение для проверки ниже
        }
        
        if (n_intervals <= 0) {
             MPI_Abort(MPI_COMM_WORLD, 1); // Прервать все процессы
        }
    }

    // 2. Рассылка N всем процессам
    // MPI_Bcast(buffer, count, datatype, root, comm)
    MPI_Bcast(&n_intervals, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    // 3. Каждый процесс вычисляет свою часть суммы
    local_sum = 0.0;
    h = 1.0 / (double)n_intervals; // Ширина одного подынтервала

    // Распределяем итерации между процессами
    // Каждый процесс i вычисляет для интервалов i, i + num_procs, i + 2*num_procs, ...
    for (i = rank; i < n_intervals; i += num_procs) {
        x = h * ((double)i + 0.5); // Средняя точка i-го подынтервала
        local_sum += (4.0 / (1.0 + x*x));
    }
    // Умножаем локальную сумму на ширину h, чтобы получить вклад этого процесса в интеграл
    local_sum *= h;


    // 4. Собираем (редуцируем) локальные суммы в глобальную сумму на процессе 0
    // MPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm)
    //   sendbuf: локальная частичная сумма (local_sum)
    //   recvbuf: буфер для результата на root-процессе (global_sum_pi).
    //            Значим только на процессе root. На других процессах может быть NULL,
    //            но безопаснее предоставить валидный буфер.
    //   count: количество элементов для редукции (1, так как у нас одно число double)
    //   datatype: тип данных (MPI_DOUBLE)
    //   op: операция редукции (MPI_SUM)
    //   root: ранг процесса, который получит результат (0)
    //   comm: коммуникатор (MPI_COMM_WORLD)
    MPI_Reduce(&local_sum, &global_sum_pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // 5. Процесс 0 выводит результат
    if (rank == 0) {
        printf("%.8f\n", global_sum_pi); // Вывод с 10 знаками после запятой
    }

    MPI_Finalize();
    return 0;
}
