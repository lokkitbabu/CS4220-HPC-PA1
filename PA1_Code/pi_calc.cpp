#include "pi.h"
#include <cstdlib>

double pi_calc(long int n, int rank, int size){
    long int local_n = n / size + (rank < (n % size) ? 1 : 0);
    srand(time(NULL) + rank);
    long int local_count = 0;
    for (long int i = 0; i < local_n; ++i) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0) local_count++;
    }
    int step = 1;
    while (step < size) {
        if (rank % (2 * step) == 0) {
            int source = rank + step;
            if (source < size) {
                long int received_count;
                MPI_Recv(&received_count, 1, MPI_LONG, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_count += received_count;
            }
        } else {
            int dest = rank - step;
            MPI_Send(&local_count, 1, MPI_LONG, dest, 0, MPI_COMM_WORLD);
            break;
        }
        step *= 2;
    }
    if (rank == 0) {
        return 4.0 * (double)local_count / (double)n;
    }
    return 0.0;
}