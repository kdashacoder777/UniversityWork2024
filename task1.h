#include "common.h"

#ifndef UNIVERWORK2024_TASK1_H
#define UNIVERWORK2024_TASK1_H

void matrixVectorMultiplication(int n, int numThreads, const std::vector<std::vector<double>>& matrix,
                                const std::vector<double>& vector, std::vector<double>& result) {
    omp_set_num_threads(numThreads);
    double sum;
#pragma omp parallel for private(sum)
    for (int i = 0; i < n; ++i)
    {
        sum = 0;
        for (int j = 0; j < n; ++j)
        {
//            int threadId = omp_get_thread_num();
//            std::cout << "Hello from thread " << threadId << std::endl;
            sum +=  COEF * matrix[i][j] * vector[j];
        }
        result[i] = sum;
    }
}

double matrixVectorMultiplicationParallelRunTime(int n, int numThreads, const std::vector<std::vector<double>>& mat,
                                                 const std::vector<double>& vec, std::vector<double>& result) {
    auto start = std::chrono::high_resolution_clock::now();
    matrixVectorMultiplication(n, numThreads, mat, vec, result);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration_ms = end - start;
    std::cout << "Programma vypolnena dlya " << numThreads << " potoka(ov) za "<< duration_ms.count() << " millisekund" << std::endl;
    return duration_ms.count();
}


void task1() {
    double runTimeOneThread = 0.0;
    std::vector<double> vec;
    std::vector<std::vector<double>> mat;
    std::vector<double> vecResult;

    std::ofstream file("statisticOfRunTimeMultiMatrixVectorThreads.csv");
    file << "Razmer obektov;"
            "Posledovatelnyj algoritm (vremya);"
            "2 processa (vremya);"
            "2 processa (izmenenie);"
            "4 processa (vremya);"
            "4 processa (izmenenie);"
            "8 processov (vremya);"
            "8 processov (izmenenie);"
            "16 processov (vremya);"
            "16 processov (izmenenie);"
         << std::endl;

    int minSize = 10000;
    int maxSize = 60000;
    std::vector<int> listSizeOfObjects = {minSize, 2 * minSize, 3 * minSize, 4 * minSize, maxSize};
    std::vector<int> listOfThreads = {2, 4, 8, 16};

    std::vector<double> listOfRunTimes = {};
    
    for (auto &sizeOfObject: listSizeOfObjects) {

        ResizeMatrix(sizeOfObject, mat);
        vec.resize(sizeOfObject, 0.);
        vecResult.resize(sizeOfObject, 0.);
        // generating random vector
        generateVector(sizeOfObject, vec);
        // generating random matrix
        generateMatrix(sizeOfObject, mat);

        std::cout << "Size of object n = " << sizeOfObject << std::endl;
        // matrix Vector Multiplication for 1 thread
        runTimeOneThread = matrixVectorMultiplicationParallelRunTime(sizeOfObject, 1, mat, vec, vecResult);

        // matrix Vector Multiplication for list of thread
        for (auto &numOfThread: listOfThreads) {
            listOfRunTimes.push_back(matrixVectorMultiplicationParallelRunTime(sizeOfObject, numOfThread, mat, vec, vecResult));
        }

        std::cout << "Write to file..." << std::endl;
        // output statistic
        file << std::to_string(sizeOfObject) << ";";
        file << runTimeOneThread << ";";
        for (auto &runTimeOfThread: listOfRunTimes) {
            file << runTimeOfThread << ";" << runTimeOneThread - runTimeOfThread << ";" ;
        }
        file << std::endl;
        listOfRunTimes = {};
    }
    file.close();
}

#endif //UNIVERWORK2024_TASK1_H
