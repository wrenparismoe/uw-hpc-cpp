#include <iostream>
#include <fstream>
#include <random>
#include <vector>

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " n" << std::endl;
    return 1;
  }

  const int n = std::atoi(argv[1]);

  // Create a random number generator
  std::mt19937_64 rng(std::random_device{}());
  std::uniform_real_distribution<double> dist(0.0, 1.0);

  // Create the matrix and fill it with random values
  std::vector<double> matrix(n * n);
  for (int i = 0; i < n * n; i++)
  {
    matrix[i] = dist(rng);
  }

  // Write the matrix to file in binary format
  std::ofstream outfile("matrix.bin", std::ios::out | std::ios::binary);
  if (outfile.is_open())
  {
    outfile.write(reinterpret_cast<const char *>(matrix.data()), sizeof(double) * n * n);
    outfile.close();
  }
  else
  {
    std::cerr << "Failed to open file: matrix.bin" << std::endl;
    return 1;
  }

  return 0;
}
