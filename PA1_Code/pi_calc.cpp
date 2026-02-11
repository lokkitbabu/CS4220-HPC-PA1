#include "pi.h"
#include <cstdlib>
#include <ctime>

double pi_calc(long int n, int rank, int size){
    // Write your code below
    ////////////////////////////////////////
    
    // Seed the random number generator differently for each processor
    srand(time(NULL) + rank);
    
    // Divide work among processors
    long int points_per_proc = n / size;
    long int remainder = n % size;
    
    // Distribute remainder points to the first 'remainder' processors
    long int local_n;
    if (rank < remainder) {
        local_n = points_per_proc + 1;
    } else {
        local_n = points_per_proc;
    }
    
    // Perform local Monte Carlo simulation
    long int local_count = 0;
    for (long int i = 0; i < local_n; i++) {
        // Generate random point in [0,1] x [0,1]
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        
        // Check if point is inside the quarter circle
        if (x * x + y * y <= 1.0) {
            local_count++;
        }
    }
    
    // Aggregate results at rank 0 using MPI_Send and MPI_Recv
    long int total_count;
    
    if (rank == 0) {
        // Rank 0 starts with its own count
        total_count = local_count;
        
        // Receive counts from all other processors
        for (int i = 1; i < size; i++) {
            long int received_count;
            MPI_Recv(&received_count, 1, MPI_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_count += received_count;
        }
        
        // Calculate and return the estimated value of pi
        double pi_estimate = 4.0 * total_count / n;
        return pi_estimate;
        
    } else {
        // Other processors send their local count to rank 0
        MPI_Send(&local_count, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD);
        return 0.0;
    }

    ////////////////////////////////////////
}
