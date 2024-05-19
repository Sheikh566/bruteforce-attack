#include <iostream>
#include <cmath>
#include <string>

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

unsigned long calculateMaxCombinations() {
    unsigned long maxCombinations = 0;
    for (int i = MIN_PASS_LENGTH; i <= MAX_PASS_LENGTH; ++i) {
        maxCombinations += pow(CHAR_SET_SIZE, i);
    }
    return maxCombinations;
}

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

int main() {
    cout << "Executing program" << endl;
    auto start = chrono::high_resolution_clock::now();
    unsigned long totalCombinations = calculateMaxCombinations();

    for (unsigned long i = 0; i < totalCombinations; ++i) {
        string combination = getCombination(i);
        if (login(combination)) {
            cout << "Password found: " << combination << endl;
            break;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Time taken: " << duration.count() << " ms" << endl;

    return 0;
}