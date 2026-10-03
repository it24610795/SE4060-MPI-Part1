#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TOTAL_TRIALS 10000000LL

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long local_trials = TOTAL_TRIALS / size;
    long long local_inside = 0;
unsigned int seed = (unsigned int)(time(NULL) ^ (rank * 12345));

    double start_time = MPI_Wtime();

    for (long long i = 0; i < local_trials; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        if (x * x + y * y <= 1.0) {
            local_inside++;
        }
    }

    long long global_inside = 0;
    MPI_Reduce(&local_inside, &global_inside, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    double elapsed = MPI_Wtime() - start_time;

    if (rank == 0) {
        double pi = 4.0 * (double)global_inside / (double)TOTAL_TRIALS;
        printf("Estimated Pi = %f\n", pi);
        printf("Time taken   = %f seconds\n", elapsed);
    }

    MPI_Finalize();
    return 0;
}
