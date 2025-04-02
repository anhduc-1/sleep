#include <iostream>
#include <vector>
#include <chrono>
#include <random>

using namespace std;
// hahaha


int partition(vector<int>& v, int left, int right) {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> idxdistrib(left, right);
	int randomPivotIdx = idxdistrib(gen);
	swap(v[randomPivotIdx], v[right]);

	int	pivot = v[right];
	int i = left - 1;
	for (int j = left; j < right; j++) {
		if (v[j] <= pivot) {
			i++;
			swap(v[j], v[i]);
		}
	}
	swap(v[i + 1], v[right]);
	return i + 1;
}

void quickSort(vector<int>& v, int left, int right) {
	if (right > left) {
		int pi = partition(v, left, right);
		quickSort(v, left, pi - 1);
		quickSort(v, pi + 1, right);
	}
}

vector<int> generateRandomVector(int size, int max) {
	vector<int> vec(size);
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distrib(0, max);
	for (int i = 0; i < size; i++) {
		vec[i] = distrib(gen);
	}
	return vec;
}

vector<int> nearlySortedVector(int size, int max, float percent) {
	vector<int> vec(size);
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distrib(0, max);

	for (int i = 0; i < size; i++) vec[i] = distrib(gen);
	quickSort(vec, 0, size - 1);

	float num_swaps = (1 - percent) / 2 * size;
	uniform_int_distribution<> idxdistrib(0, size - 1);
	for (int i = 0; i < (int)num_swaps; i++) {
		swap(vec[idxdistrib(gen)], vec[idxdistrib(gen)]);
	}

	return vec;
}

vector<int> sortedVector(int size, int max) {
	vector<int> vec = generateRandomVector(size, max);
	quickSort(vec, 0, size - 1);
	return vec;
}

vector<int> reversedVector(int size, int max) {
	vector<int> vec = generateRandomVector(size, max);
	quickSort(vec, 0, size - 1);
	reverse(vec.begin(), vec.end());
	return vec;
}

template <typename Func, typename... Args>
double measureExecutionTime(Func func, Args&&... args) {
	auto start = chrono::high_resolution_clock::now();
	func(forward<Args>(args)...);
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
	return duration.count() / 1000.0;
}

void manualTiming(vector<int> v, int size) {
	auto start = chrono::high_resolution_clock::now();
	quickSort(v, 0, size - 1);
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
	cout << "Manual timing: Quick Sort took " << duration.count() / 1000.0 << " milliseconds " << endl;
}

double measureAverageTime(int runs, int size, int max) {
	double totalTime = 0.0;
	for (int i = 0; i < runs; i++) {
		vector<int> testArray = generateRandomVector(size, max);
		auto start = chrono::high_resolution_clock::now();
		quickSort(testArray, 0, size - 1);
		auto end = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
		totalTime += duration.count();
	}
	return (totalTime / runs) / 1000.0;
}

int main()
{
	int size = 10000;
	int max = 1500000000;
	cout << "Test Array Random:" << endl;
	vector<int> arr1 = generateRandomVector(size, max);
	manualTiming(arr1, size);
	cout << "--------------------------------" << endl;

	cout << "Test Array Nearly Sorted:" << endl;
	vector<int> arr2 = nearlySortedVector(size, max, 0.8);
	manualTiming(arr2, size);
	cout << "--------------------------------" << endl;

	cout << "Test Array Sorted:" << endl;
	vector<int> arr3 = sortedVector(size, max);
	manualTiming(arr3, size);
	cout << "--------------------------------" << endl;

	cout << "Test Array Reversed:" << endl;
	vector<int> arr4 = reversedVector(size, max);
	manualTiming(arr4, size);
	cout << "--------------------------------" << endl;

	int runs = 10;
	double totalTime = 0.0;
	for (int i = 0; i < runs; i++) {
		vector<int> v1 = generateRandomVector(size, max);
		double quickSortTime1 = measureExecutionTime([&]() {
			quickSort(v1, 0, size - 1);
			});

		vector<int> v2 = nearlySortedVector(size, max, 0.8);
		double quickSortTime2 = measureExecutionTime([&]() {
			quickSort(v2, 0, size - 1);
			});

		vector<int> v3 = sortedVector(size, max);
		double quickSortTime3 = measureExecutionTime([&]() {
			quickSort(v3, 0, size - 1);
			});

		vector<int> v4 = reversedVector(size, max);
		double quickSortTime4 = measureExecutionTime([&]() {
			quickSort(v4, 0, size - 1);
			});

		double tb = (quickSortTime1 + quickSortTime2 + quickSortTime3 + quickSortTime4) / 4;
		totalTime += tb;
	}
	cout << "Averange quick sort time over " << runs << " runs: " << totalTime / runs << " milliseconds" << endl;
	return 0;
}