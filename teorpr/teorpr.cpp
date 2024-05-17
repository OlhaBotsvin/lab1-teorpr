// Програма для розв'язання СЛАР методом головних елементів
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>

// Функція для зчитування матриці з файлу
std::vector<std::vector<double>> readMatrixFromFile(const std::string& filename, int& n) {
    std::ifstream file(filename);
	if (!file.is_open()) { // Перевірка чи файл відкрито
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return {};
    }

    std::vector<std::vector<double>> matrix;
    std::string line;

	while (std::getline(file, line)) { // Зчитування рядків з файлу
        std::istringstream iss(line);
        std::vector<double> row;
        double num;
		while (iss >> num) { // Зчитування чисел з рядка
            row.push_back(num);
        }
		if (!row.empty()) { // Додавання рядка до матриці
            matrix.push_back(row);
        }
    }

    n = matrix.size();
	for (const auto& row : matrix) { // Перевірка на правильність формату матриці
        if (row.size() != n + 1) {
            std::cerr << "Error: Invalid matrix format in file " << filename << std::endl;
            return {};
        }
    }

    return matrix;
}

// Функція для виведення розв'язку
void printSolution(const std::vector<double>& solution) {
    std::cout << "Solution using Gaussian elimination:\n";
    for (size_t i = 0; i < solution.size(); i++) {
        std::cout << "x" << i << " = " << solution[i] << std::endl;
    }
}

// Функція для розв'язання СЛАР методом головних елементів
void gaussianElimination(std::vector<std::vector<double>>& matrix, std::vector<double>& solution) {
    int n = matrix.size();
    for (int k = 0; k < n; k++) {
        int maxRowIndex = k;
        double maxAbsValue = std::abs(matrix[k][k]);

		for (int i = k + 1; i < n; i++) { // Пошук головного елемента
			if (std::abs(matrix[i][k]) > maxAbsValue) { // Пошук максимального елемента
                maxRowIndex = i;
                maxAbsValue = std::abs(matrix[i][k]);
            }
        }

		if (maxRowIndex != k) { // Зміна рядків місцями
            std::swap(matrix[k], matrix[maxRowIndex]);
        }

		double diagonalValue = matrix[k][k]; // Ділення рядка на головний елемент
        matrix[k][k] = 1;
        for (int j = k + 1; j < n + 1; j++) {
            matrix[k][j] /= diagonalValue;
        }

		for (int i = k + 1; i < n; i++) { // Віднімання рядків
			double multiplier = matrix[i][k]; // Множник
            matrix[i][k] = 0;
			if (multiplier != 0) { // Віднімання рядків
                for (int j = k + 1; j < n + 1; j++) {
                    matrix[i][j] -= multiplier * matrix[k][j];
                }
            }
        }
    }

	for (int i = n - 1; i >= 0; i--) {  // Знаходження розв'язку
        double sum = matrix[i][n];
		for (int j = i + 1; j < n; j++) { // Знаходження суми
			sum -= matrix[i][j] * solution[j]; // Віднімання від суми
        }
        solution[i] = sum;
    }
}

// Функція для порівняння двох векторів
bool compareVectors(const std::vector<double>& vec1, const std::vector<double>& vec2) {
	if (vec1.size() != vec2.size()) return false; // Перевірка на рівність розмірів векторів
	for (size_t i = 0; i < vec1.size(); ++i) { // Перевірка на рівність елементів векторів
        if (std::abs(vec1[i] - vec2[i]) > 1e-6) return false; 
    }
    return true;
}

// Функція для тестування
void runTest(const std::string& filename, const std::vector<double>& expectedSolution) {
    int n;
	std::vector<std::vector<double>> matrix = readMatrixFromFile(filename, n); // Зчитування матриці з файлу
	if (matrix.empty()) { // Перевірка на помилку
        std::cerr << "Error reading matrix from file: " << filename << std::endl;
        return;
    }

    std::vector<double> solution(n, 0);
    gaussianElimination(matrix, solution);

    printSolution(solution);

	if (compareVectors(solution, expectedSolution)) { // Перевірка на правильність розв'язку
        std::cout << "Test passed for file: " << filename << std::endl;
    }
    else {
		std::cerr << "Test failed for file: " << filename << ". Expected solution: "; // Якщо розв'язок неправильний
        printSolution(expectedSolution);
        std::cerr << "Actual solution: ";
        printSolution(solution);
    }
}

int main() {
	// Тестування
    runTest("matrix1.txt", { 1, -4.44089e-16, 3 }); 
	std::cout << std::endl;
    runTest("matrix2.txt", { 0.88, 0.32, 1.08 });
    std::cout << std::endl;

    runTest("invalid_matrix.txt", {});

    return 0;
}
