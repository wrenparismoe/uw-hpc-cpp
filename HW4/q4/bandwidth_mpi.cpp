#include <fstream>
#include <iostream>
#include <vector>

#include <mpi.h>

template <typename T>
void my_broadcast(T* data, int count, int root, MPI_Comm comm) {
    // Get the rank of the current process, and number of processes.
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    // If the current process is not the root process, then receive the data from the root.
    if (rank != root) {
        MPI_Recv(data, count, MPI_BYTE, root, 0, comm, MPI_STATUS_IGNORE);
    } else {
        // If the current process is the root process, then send the data to all other processes.
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Send(data, count, MPI_BYTE, i, 0, comm);
            }
        }
    }
}

int main(int argc, char** argv) {
    // Initialize MPI.
    MPI_Init(&argc, &argv);

    // Get the number of processes.
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Get the rank of the current process.
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Create a vector to store the measurements.
    std::vector<double> my_bandwidth(61);
    std::vector<double> bcast_bandwidth(61);
    int index = 0;

    // Loop over the message sizes.
    for (int i = 8; i <= 128*1024*1024; i*=2) {

        // Create a buffer to store the data.
        char* buffer = new char[i];

        // Initialize the data in the buffer.
        for (int j = 0; j < i; j++) {
            buffer[j] = 'a';
        }

        double start_time = MPI_Wtime();
        my_broadcast(buffer, i, 0, MPI_COMM_WORLD);
        double end_time = MPI_Wtime();
        my_bandwidth[index] = (i / (end_time - start_time));

        start_time = MPI_Wtime();
        MPI_Bcast(buffer, i, MPI_BYTE, 0, MPI_COMM_WORLD);
        end_time = MPI_Wtime();
        bcast_bandwidth[index] = (i / (end_time - start_time));

        // Print message size, my_bandwidth, bcast_bandwidth for rank 0 at current iteration.
        if (rank == 0) {
            std::cout << "message_size: " << i << " Bytes | My Bandwidth: " << my_bandwidth[index] << " Bcast Bandwidth: " << bcast_bandwidth[index] << std::endl;
        }

        // Delete the buffer.
        delete[] buffer;
        index++;
    }

    // Write the measurements to a CSV file.
    if (rank == 0) {
        std::ofstream csv_file("bandwidth.csv");
        csv_file << "message_size,my_bandwidth,bcast_bandwidth" << std::endl;
        index = 0;
        for (int i = 8; i <= 128*1024*1024; i*=2) {
            int message_size = 8 * (1 << i);
            csv_file << message_size << "," << my_bandwidth[index] << "," << bcast_bandwidth[index] << std::endl;
            index += 1;
        }
        csv_file.close();
    }

    // Finalize MPI.
    MPI_Finalize();

    return 0;
}
