#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>

std::vector<std::vector<double>> readMatrixFromFile(const std::string& filename, int& n) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return {};
    }

    std::vector<std::vector<double>> matrix;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<double> row;
        double num;
        while (iss >> num) {
            row.push_back(num);
        }
        if (!row.empty()) {
            matrix.push_back(row);
        }
    }

    n = matrix.size();
    for (const auto& row : matrix) {
        if (row.size() != n + 1) {
            std::cerr << "Error: Invalid matrix format in file " << filename << std::endl;
            return {};
        }
    }

    return matrix;
}


void printSolution(const std::vector<double>& solution) {
    std::cout << "Solution using Gaussian elimination:\n";
    for (size_t i = 0; i < solution.size(); i++) {
        std::cout << "x" << i << " = " << solution[i] << std::endl;
    }
}

void gaussianElimination(std::vector<std::vector<double>>& matrix, std::vector<double>& solution) {
    int n = matrix.size();
    for (int k = 0; k < n; k++) {
        int maxRowIndex = k;
        double maxAbsValue = std::abs(matrix[k][k]);

        for (int i = k + 1; i < n; i++) {
            if (std::abs(matrix[i][k]) > maxAbsValue) {
                maxRowIndex = i;
                maxAbsValue = std::abs(matrix[i][k]);
            }
        }

        if (maxRowIndex != k) {
            std::swap(matrix[k], matrix[maxRowIndex]);
        }

        double diagonalValue = matrix[k][k];
        matrix[k][k] = 1;
        for (int j = k + 1; j < n + 1; j++) {
            matrix[k][j] /= diagonalValue;
        }

        for (int i = k + 1; i < n; i++) {
            double multiplier = matrix[i][k];
            matrix[i][k] = 0;
            if (multiplier != 0) {
                for (int j = k + 1; j < n + 1; j++) {
                    matrix[i][j] -= multiplier * matrix[k][j];
                }
            }
        }
    }

    for (int i = n - 1; i >= 0; i--) {
        double sum = matrix[i][n];
        for (int j = i + 1; j < n; j++) {
            sum -= matrix[i][j] * solution[j];
        }
        solution[i] = sum;
    }
}


bool compareVectors(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    if (vec1.size() != vec2.size()) return false;
    for (size_t i = 0; i < vec1.size(); ++i) {
        if (std::abs(vec1[i] - vec2[i]) > 1e-6) return false; 
    }
    return true;
}


void runTest(const std::string& filename, const std::vector<double>& expectedSolution) {
    int n;
    std::vector<std::vector<double>> matrix = readMatrixFromFile(filename, n);
    if (matrix.empty()) {
        std::cerr << "Error reading matrix from file: " << filename << std::endl;
        return;
    }

    std::vector<double> solution(n, 0);
    gaussianElimination(matrix, solution);

    printSolution(solution);

    if (compareVectors(solution, expectedSolution)) {
        std::cout << "Test passed for file: " << filename << std::endl;
    }
    else {
        std::cerr << "Test failed for file: " << filename << ". Expected solution: ";
        printSolution(expectedSolution);
        std::cerr << "Actual solution: ";
        printSolution(solution);
    }
}

int main() {
    runTest("matrix1.txt", { 1, -4.44089e-16, 3 });
	std::cout << std::endl;
    runTest("matrix2.txt", { 0.5, -1, 2.5 });
    std::cout << std::endl;

    runTest("invalid_matrix.txt", {});

    return 0;
}
