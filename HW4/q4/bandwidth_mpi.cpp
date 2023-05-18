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

    // Loop over the message sizes.
    for (int i = 0; i <= 12; i++) {
      // Set the message size.
      int message_size = 8 * (1 << i);

      // Create a buffer to store the data.
      char* buffer = new char[message_size];

      // Initialize the data in the buffer.
      for (int j = 0; j < message_size; j++) {
        buffer[j] = 'a';
      }

      double start_time = MPI_Wtime();
      my_broadcast(buffer, message_size, 0, MPI_COMM_WORLD);
      double end_time = MPI_Wtime();
      my_bandwidth[i] = (message_size / (end_time - start_time)) / (1024 * 1024);

      start_time = MPI_Wtime();
      MPI_Bcast(buffer, message_size, MPI_BYTE, 0, MPI_COMM_WORLD);
      end_time = MPI_Wtime();
      bcast_bandwidth[i] = (message_size / (end_time - start_time)) / (1024 * 1024);

      // Delete the buffer.
      delete[] buffer;
    }

    // Write the measurements to a CSV file.
    if (rank == 0) {
      std::ofstream csv_file("bandwidth.csv");
      csv_file << "message_size,my_bandwidth,bcast_bandwidth" << std::endl;
      for (int i = 0; i <= 12; i++) {
        int message_size = 8 * (1 << i);
        csv_file << message_size << "," << my_bandwidth[i] << "," << bcast_bandwidth[i] << std::endl;
      }
      csv_file.close();
    }

    // Finalize MPI.
    MPI_Finalize();

    return 0;
}
