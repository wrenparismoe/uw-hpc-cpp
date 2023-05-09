#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <chrono>

void printMatrix (const std::vector<double> &A) {
	int n = sqrt(A.size());
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << A[i * n + j] << " ";
        }
        std::cout << std::endl;
    }
}

static std::random_device rd;
static std::mt19937 gen(rd());

// Generate random matrix
std::vector<double> generateRandomMatrix (int n) {
    std::vector<double> A(n*n);
    std::uniform_real_distribution<double> dist(0,10); // range [0,1] //TODO: CHANGE BACK TO T
    for (int i = 0; i < n*n; ++i) {
		A[i] = dist(gen);
    }
    return A;
}

// Write matrix to file
void writeMatrixToFile(const std::vector<double> &A) {
	int nn = A.size();
	std::ofstream outfile("matrix.bin", std::ios::out | std::ios::binary);
	if (outfile.is_open()) {
		outfile.write(reinterpret_cast<const char *>(A.data()), sizeof(double) * nn);
		outfile.close();
	}
	else {
		std::cerr << "Failed to open file: matrix.bin" << std::endl;
		return;
	}
}

// Read matrix from file
void readMatrixFromFile(int n) {
	std::vector<double> A(n*n);
	std::ifstream infile("matrix.bin", std::ios::binary);
	if (!infile) {
        std::cerr << "Error: could not open file for reading" << std::endl;
        return;
	}
	
	infile.read(reinterpret_cast<char *>(A.data()), sizeof(double) * n*n);
	
	if (!infile) {
		std::cerr << "Failed to open file: matrix.bin" << std::endl;
		return;
	}
}


int main() {
	int n_min = 32;
	int n_max = 16384;
	
	std::cout << sizeof(double) << std::endl;
	return 1;
	
    std::ofstream performanceCSV("performance.csv");
    performanceCSV << "n,write_time,read_time,bytes" << std::endl;
	
	long double micro_to_secondL = 1.e-6L;
	
	for (int n = n_min; n <= n_max; n*=2) {
		std::vector<double> A = generateRandomMatrix(n);
		auto start = std::chrono::high_resolution_clock::now();
		writeMatrixToFile(A);
		auto stop = std::chrono::high_resolution_clock::now();
        auto write_duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        long double write_durationL = static_cast<long double>(write_duration.count());
		
		start = std::chrono::high_resolution_clock::now();
		readMatrixFromFile(n);
		stop = std::chrono::high_resolution_clock::now();
		auto read_duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		long double read_durationL = static_cast<long double>(read_duration.count());
		
		// Measure bytes written
		long int bytes = sizeof(double) * n * n;
		
		// Print (n): write_time, read_time, bytes
		std::cout << "(n=" << n << "): " << write_durationL*micro_to_secondL << ", " << read_durationL*micro_to_secondL << ", " << bytes << std::endl;
		
		// Write to CSV
		performanceCSV << n << "," << write_durationL*micro_to_secondL << "," << read_durationL*micro_to_secondL << "," << bytes << std::endl;
	}
	performanceCSV.close();
}
