#include "mpi.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int i, rank, size, namelen;
    char name[MPI_MAX_PROCESSOR_NAME];
    /* ��������� ����������:    
    int count; ���������� ���������� ������, 
    int cancelled; ��������, ��� �� ��������������� ������ �������.
    int MPI_SOURCE; �������� ���������.
    int MPI_TAG; �������� ���� ���������.
    int MPI_ERROR ������, ��������� � ����������.
    */
    MPI_Status stat;
    // ������������� MPI-���������
    MPI_Init(&argc, &argv);
    //����������� ������ ����� ��������� � ������������� MPI_COMM_WORLD
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //����������� ����� �������� �������� � ������������� MPI_COMM_WORLD.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // ��������� ����� ����������, �� ������� ����������� ������� �������
    MPI_Get_processor_name(name, &namelen);

    if (rank == 0) {
        printf("Hello world: rank %d of %d running on %s\n", rank, size, name);

        for (i = 1; i < size; i++) {
            /* ��������� �������� ������ � �� ������������ �� ��� ���, ���� �� ����� �������� ��������������� ���������.
            ��������� MPI_Status* status
        ���������:
        buf [in, optional] ��������� �� �����, ���������� ������, ���������� ������.
        count ���������� ��������� � ������. ���� ����� ������ ��������� �����, ���������� ��� ��������� count �������� 0.
        datatype ��� ������ ��������� � ������.
        dest ���� �������� ���������� � �������������, ������� �������� ���������� comm.
        tag ��� ���������, ������� ����� ������������ ��� ���������� ��������� ����� ���������.
        comm ������ ��� ����������� �������������.
            */
            MPI_Recv(&rank, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &stat);
            MPI_Recv(&size, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &stat);
            MPI_Recv(&namelen, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &stat);
            MPI_Recv(name, namelen + 1, MPI_CHAR, i, 1, MPI_COMM_WORLD, &stat);
            printf("Hello world: rank %d of %d running on %s\n", rank, size, name);
        }
    }
    else {
        /* MPI_Send ��������� �������� �������� � ����������� ������ � ������������, ����� ����� �������� ����� ��������� ������������ ��������.
        ���������:
        buf [in, optional] ��������� �� �����, ���������� ������, ���������� ��������.
        count ���������� ��������� � ������. ���� ����� ������ ��������� �����, ���������� ��� ��������� count �������� 0.
        datatype ��� ������ ��������� � ������.
        dest ���� �������� ���������� � �������������, ������� �������� ���������� comm.
        tag ��� ���������, ������� ����� ������������ ��� ���������� ��������� ����� ���������.
        comm ������ ��� ����������� �������������.
        */
        MPI_Send(&rank, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(&size, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(&namelen, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(name, namelen + 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
    }
    // ���������� MPI-���������
    MPI_Finalize();
    return (0);
}
