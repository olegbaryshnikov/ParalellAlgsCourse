#include <iostream>
#include "mpi.h"
using namespace std;
int main(int argc, char** argv)
{
	int rank, size, prev, next;
	int rbuf[2], sbuf[2];

	MPI_Init(&argc, &argv);
	MPI_Request reqs[4];
	MPI_Status stats[4];

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	prev = rank - 1;
	next = rank + 1;
	if (rank == 0)
		prev = size - 1;
	if (rank == size - 1)
		next = 0;

	MPI_Recv_init(&rbuf[0], 1, MPI_INT, prev, 5, MPI_COMM_WORLD, &reqs[0]);
	MPI_Recv_init(&rbuf[1], 1, MPI_INT, next, 6, MPI_COMM_WORLD, &reqs[1]);
	MPI_Send_init(&sbuf[0], 1, MPI_INT, prev, 6, MPI_COMM_WORLD, &reqs[2]);
	MPI_Send_init(&sbuf[1], 1, MPI_INT, next, 5, MPI_COMM_WORLD, &reqs[3]);

	int isCompleted = 0;
	for (int i = 0; i < 2; i++)
	{
		sbuf[0] = rank;
		sbuf[1] = rank;

		MPI_Startall(4, reqs);
		MPI_Waitall(4, reqs, stats);

		MPI_Testall(4, reqs, &isCompleted, stats);

		if (isCompleted!=0)
			cout << "Process rank: " << rank << " | Previous: " << rbuf[0] << " | Next: " << rbuf[1] << "\n";
	}

	MPI_Request_free(&reqs[0]);
	MPI_Request_free(&reqs[1]);
	MPI_Request_free(&reqs[2]);
	MPI_Request_free(&reqs[3]);


	MPI_Finalize();
}