#include <mpi.h>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Создаем группу для четных процессов
    MPI_Group world_group, even_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);

    std::vector<int> even_ranks;
    for (int i = 0; i < world_size; i += 2) {
        even_ranks.push_back(i);
    }

    MPI_Group_incl(world_group, even_ranks.size(), even_ranks.data(), &even_group);
    
    // Создаем новый коммуникатор
    MPI_Comm even_comm;
    MPI_Comm_create(MPI_COMM_WORLD, even_group, &even_comm);

    // Чтение входных данных (только процесс 0)
    std::string message = "no";
    if (world_rank == 0) {
        std::getline(std::cin, message);
        if (message.length() < 1 || message.length() > 10) {
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    // Широковещательная рассылка в новой группе
    int even_rank = MPI_UNDEFINED, even_size = 0;
    char buffer[11] = {'n','o','\0'};
    
    if (even_comm != MPI_COMM_NULL) {
        MPI_Comm_rank(even_comm, &even_rank);
        MPI_Comm_size(even_comm, &even_size);

        // Подготовка сообщения
        if (even_rank == 0) {
            message.copy(buffer, message.size());
            buffer[message.size()] = '\0';
        }

        MPI_Bcast(buffer, 10, MPI_CHAR, 0, even_comm);
    }

    // Вывод результатов
    std::cout << "MPI_COMM_WORLD: " << world_rank << " from " << world_size << ". ";
    std::cout << "New comm: ";
    
    if (even_comm != MPI_COMM_NULL) {
        std::cout << even_rank << " from " << even_size << ". ";
        std::cout << "Message = " << buffer << std::endl;
        MPI_Comm_free(&even_comm);
    } else {
        std::cout << "no from no. Message = no" << std::endl;
    }

    MPI_Group_free(&world_group);
    MPI_Group_free(&even_group);
    MPI_Finalize();
    return 0;
}