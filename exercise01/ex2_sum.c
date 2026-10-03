#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10000000LL

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double start_time = MPI_Wtime();

    long long chunk = N / size;
    long long start = rank * chunk + 1;
    long long end = (rank == size - 1) ? N : (rank + 1) * chunk;

    long long local_sum = 0;
    for (long long i = start; i <= end; i++) {
        local_sum += i;
    }

    long long global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    double elapsed = MPI_Wtime() - start_time;

    if (rank == 0) {
        long long expected = N * (N + 1) / 2;
        printf("Global Sum   = %lld\n", global_sum);
        printf("Expected     = %lld\n", expected);
        printf("Correct?     = %s\n", global_sum == expected ? "YES" : "NO");
        printf("Time taken   = %f seconds\n", elapsed);
    }

    MPI_Finalize();
    return 0;
}
