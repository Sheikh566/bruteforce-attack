#include <iostream>
#include <pthread.h>
#include <cmath>
#include <string.h>

#include <chrono>

using namespace std;

#define MIN_PASS_LENGTH 3
#define MAX_PASS_LENGTH 5
#define CHAR_SET (char[]) { \
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', \
    'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', \
    's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', \
    '1', '2', '3', '4', '5', '6', '7', '8', '9'  \
}
#define CHAR_SET_SIZE sizeof(CHAR_SET)/sizeof(CHAR_SET[0])
#define X 4 // No. of threads (ideally be equal to no. of core on your computer)

unsigned long calculateMaxCombinations() {
    unsigned long maxCombinations = 0;
    for (int i = MIN_PASS_LENGTH; i <= MAX_PASS_LENGTH; ++i) {
        maxCombinations += pow(CHAR_SET_SIZE, i);
    }
    return maxCombinations;
}

struct ThreadArgs {
    unsigned long start;
    unsigned long end;
};
// login mockup function
bool login(string password) {
    return password == "4s11x";
}

string getCombination(unsigned long index) {
    string result = "";
    index--; // Adjust index to be 0-based

    // Calculate the length of the combination
    unsigned long len = 1;
    unsigned long total = CHAR_SET_SIZE;
    while (index >= total) {
        index -= total;
        len++;
        total *= CHAR_SET_SIZE;
    }

    // Calculate the combination
    for (unsigned long i = 0; i < len; i++) {
        result = CHAR_SET[index % CHAR_SET_SIZE] + result;
        index /= CHAR_SET_SIZE;
    }

    return result;
}

void* generateCombination(void* arg) {
    struct ThreadArgs* args = static_cast<ThreadArgs*>(arg);
    unsigned long i = args->start;
    for (;i < args->end; ++i) {
        string combination = getCombination(i);
        if (login(combination)) {
            char* retVal = new char[combination.length() + 1];
            strcpy(retVal, combination.c_str());
            return retVal;
        }
    }

    return nullptr;
}


int main() {
    cout << "Executing program with " << X << " threads" << endl;
    auto start = chrono::high_resolution_clock::now();
    unsigned long totalCombinations = calculateMaxCombinations();
    unsigned long WL = totalCombinations / X; // Workload

    pthread_t threads[X];
    struct ThreadArgs threadArgs[X];
    for (int i = 0; i < X; ++i) {
        threadArgs[i].start = i * WL;
        threadArgs[i].end = (i + 1) * WL;
        // Add the remaining combinations to the last thread
        if (i == X - 1) {
            threadArgs[i].end += totalCombinations % X;
        }
        pthread_create(&threads[i], NULL, generateCombination, (void*) &threadArgs[i]);
    }

    void* retVals[X];
    for (int i = 0; i < X; ++i) {
        pthread_join(threads[i], &retVals[i]);
        char* retVal = static_cast<char*>(retVals[i]);
        if (retVal != nullptr) {
            cout << "Password found by thread " << i << ": " << retVal << endl;
            // When password is found, cancel all other threads
            // for (int j = 0; j < X; ++j) {
            //     if (j != i) {
            //         pthread_cancel(threads[j]);
            //     }
            // }
            delete[] retVal;
            break;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Time taken: " << duration.count() << " ms" << endl;

    return 0;
}