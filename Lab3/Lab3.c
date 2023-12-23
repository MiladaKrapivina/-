#include "mpi.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int i, rank, size, namelen;
    char name[MPI_MAX_PROCESSOR_NAME];
    /* структура содержащая:    
    int count; количество полученных заявок, 
    int cancelled; указание, был ли соответствующий запрос отменен.
    int MPI_SOURCE; источник сообщения.
    int MPI_TAG; значение тега сообщения.
    int MPI_ERROR Ошибка, связанная с сообщением.
    */
    MPI_Status stat;
    // Инициализация MPI-окружения
    MPI_Init(&argc, &argv);
    //Определение общего числа процессов в коммуникаторе MPI_COMM_WORLD
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //Определение ранга текущего процесса в коммуникаторе MPI_COMM_WORLD.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // получение имени процессора, на котором выполняется текущий процесс
    MPI_Get_processor_name(name, &namelen);

    if (rank == 0) {
        printf("Hello world: rank %d of %d running on %s\n", rank, size, name);

        for (i = 1; i < size; i++) {
            /* Выполняет операцию приема и не возвращается до тех пор, пока не будет получено соответствующее сообщение.
            Возращает MPI_Status* status
        Аргументы:
        buf [in, optional] Указатель на буфер, содержащий данные, подлежащие приему.
        count Количество элементов в буфере. Если часть данных сообщения пуста, установите для параметра count значение 0.
        datatype Тип данных элементов в буфере.
        dest Ранг процесса назначения в коммуникаторе, который задается параметром comm.
        tag Тег сообщения, который можно использовать для различения различных типов сообщений.
        comm служит для подключения коммуникатора.
            */
            MPI_Recv(&rank, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &stat);
            MPI_Recv(&size, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &stat);
            MPI_Recv(&namelen, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &stat);
            MPI_Recv(name, namelen + 1, MPI_CHAR, i, 1, MPI_COMM_WORLD, &stat);
            printf("Hello world: rank %d of %d running on %s\n", rank, size, name);
        }
    }
    else {
        /* MPI_Send Выполняет операцию отправки в стандартном режиме и возвращается, когда буфер отправки можно безопасно использовать повторно.
        Аргументы:
        buf [in, optional] Указатель на буфер, содержащий данные, подлежащие отправке.
        count Количество элементов в буфере. Если часть данных сообщения пуста, установите для параметра count значение 0.
        datatype Тип данных элементов в буфере.
        dest Ранг процесса назначения в коммуникаторе, который задается параметром comm.
        tag Тег сообщения, который можно использовать для различения различных типов сообщений.
        comm служит для подключения коммуникатора.
        */
        MPI_Send(&rank, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(&size, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(&namelen, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(name, namelen + 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
    }
    // Завершение MPI-окружения
    MPI_Finalize();
    return (0);
}
