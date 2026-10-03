#include <mpi.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Allocate buffer for Bsend (message size + MPI overhead)
    int buffer_size = sizeof(int) + MPI_BSEND_OVERHEAD;
    char *buffer = (char *)malloc(buffer_size);
    MPI_Buffer_attach(buffer, buffer_size);

    int number;
    for (int i = 0; i < 3; i++) {
        if (rank == 0) {
            number = i * 10;
            // Buffered send copies data into user-provided buffer and returns immediately
            MPI_Bsend(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            std::cout << "Process 0 sent " << number << "\n";
        } else if (rank == 1) {
            MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Process 1 received " << number << "\n";
        }
    }

    // Detach and release buffer before finalizing
    MPI_Buffer_detach(&buffer, &buffer_size);
    free(buffer);

    MPI_Finalize();
    return 0;
}
