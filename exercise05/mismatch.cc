#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int number;
    if (rank == 0) {
        number = 42;
        // Rank 0 sends to Rank 1
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        std::cout << "Process 0 sent " << number << "\n";
    } else if (rank == 1) {
        // Mismatch: Rank 1 waits to receive from Rank 2 (which never sends)
        MPI_Recv(&number, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Process 1 received " << number << "\n";
    }

    MPI_Finalize();
    return 0;
}
