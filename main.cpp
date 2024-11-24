#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

class ArrayGenerator {
public:
    static std::vector<int> generateRandomArray(int size, int minValue, int maxValue) {
        std::vector<int> array(size);
        std::uniform_int_distribution<int> dist(minValue, maxValue);
        std::mt19937 rng(std::random_device{}());
        for (int i = 0; i < size; ++i) {
            array[i] = dist(rng);
        }
        return array;
    }

    static std::vector<int> generateReverseSortedArray(int size) {
        std::vector<int> array(size);
        for (int i = 0; i < size; ++i) {
            array[i] = size - i;
        }
        return array;
    }

    static std::vector<int> generateNearlySortedArray(int size) {
        int swaps = 10;
        std::vector<int> array = generateRandomArray(size, 0, 6000);
        std::sort(array.begin(), array.end());
        for (int i = 0; i < swaps; ++i) {
            int index1 = rand() % size;
            int index2 = rand() % size;
            std::swap(array[index1], array[index2]);
        }
        return array;
    }
};

void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) {
        arr[k++] = L[i++];
    }

    while (j < n2) {
        arr[k++] = R[j++];
    }
}

void mergeSort(std::vector<int>& array, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(array, left, mid);
        mergeSort(array, mid + 1, right);
        merge(array, left, mid, right);
    }
}

void insertionSort(std::vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void hybridMergeSort(std::vector<int>& array, int left, int right, int threshold) {
    if (right - left + 1 <= threshold) {
        insertionSort(array, left, right);
    } else {
        if (left < right) {
            int mid = left + (right - left) / 2;
            hybridMergeSort(array, left, mid, threshold);
            hybridMergeSort(array, mid + 1, right, threshold);
            merge(array, left, mid, right);
        }
    }
}

class SortTester {
public:
    static void testMergeSort(std::vector<int> array) {
        auto start = std::chrono::high_resolution_clock::now();
        mergeSort(array, 0, array.size() - 1);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        long long msec = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        std::cout  << msec  << '\n';
    }

    static void testHybridMergeSort(std::vector<int> array, int threshold) {
        auto start = std::chrono::high_resolution_clock::now();
        hybridMergeSort(array, 0, array.size() - 1, threshold);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        long long msec = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        std::cout << msec << "\n";
    }
};


int main() {
    for (int size = 500; size <= 10000; size += 100) {
        std::vector<int> randomArray = ArrayGenerator::generateRandomArray(size, 0, 6000);
        std::vector<int> reverseArray = ArrayGenerator::generateReverseSortedArray(size);
        std::vector<int> nearlySortedArray = ArrayGenerator::generateNearlySortedArray(size);

        std::cout << size << " ";
        SortTester::testMergeSort(std::vector<int>(randomArray.begin(), randomArray.begin() + size));
        SortTester::testMergeSort(std::vector<int>(reverseArray.begin(), reverseArray.begin() + size));
        SortTester::testMergeSort(std::vector<int>(nearlySortedArray.begin(), nearlySortedArray.begin() + size));
        SortTester::testHybridMergeSort(std::vector<int>(randomArray.begin(), randomArray.begin() + size), 20);
        SortTester::testHybridMergeSort(std::vector<int>(reverseArray.begin(), reverseArray.begin() + size), 20);
        SortTester::testHybridMergeSort(std::vector<int>(nearlySortedArray.begin(), nearlySortedArray.begin() + size), 20);
    }
}
