#include <mpi.h>
#include <cstdlib>
#include <cstdio>
#include <cstdint>

// naive:
//   auto tmp = (1.0 / n) * (i - 0.5);
//   partial_sum += 4.0 / (1.0 + tmp * tmp);
//
// The below rearranges calculations to attempt to stay withing the
// range and precisions of a double for intermediate calculations
// under large values of n.
static double calc(int rank, int size, long long n) {
    double partial_sum = 0.0;

    // precalculate intermediate values that are the same across
    // each iteration
    const double n_squared = static_cast<double>(n) * n;
    const double four_n = 4.0 * n;

    for (long long i = rank; i < n; i += static_cast<long long>(size)) {
        // Rearranged: 4n^2 / (n^2 + (i - 0.5)^2)
        // Also, 4n is used instead of 4n^2 since there would be a
        // division by n at the end anyway, so the division by n
        // can be omitted by factoring it out here. So the partial
        // sum for each iteration is boiled down to:
        // 4n / (n^2 + (i - 0.5)^2)
        double tmp = i - 0.5;
        partial_sum += four_n / (n_squared + (tmp * tmp));
    }

    double sum = 0.0;
    MPI_Reduce(&partial_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    return sum;
}

int main(int argc, char** argv) {
    int rank = -1;
    int size = -1;
    double starttime = 0.0;
    double endtime = 0.0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    long long n = 0;
    if (rank == 0 && argc > 1) {
        starttime = MPI_Wtime();
        n = std::atoll(argv[1]);
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (n < 1) {
        if (rank == 0) {
            if (argc < 2) {
                fprintf(stderr, "Please provide an integer value\n");
            } else {
                fprintf(stderr, "Invalid value of n: %lli\n", n);
            }
        }
    } else {
        double result = calc(rank, size, n);
        if (rank == 0) {
            endtime = MPI_Wtime();
            printf("%.12f, %.12f\n", result, endtime - starttime);
        }
    }

    return MPI_Finalize();
}
