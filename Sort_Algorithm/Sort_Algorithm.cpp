#include <iostream>
#include <random>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;

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
	sort(vec.begin(), vec.end());

	float num_swaps = (1 - percent) / 2 * size;
	uniform_int_distribution<> idxdistrib(0, size - 1);
	for (int i = 0; i < (int)num_swaps; i++) {
		swap(vec[idxdistrib(gen)], vec[idxdistrib(gen)]);
	}

	return vec;
}

vector<int> sortedVector(int size, int max) {
	vector<int> vec = generateRandomVector(size, max);
	sort(vec.begin(), vec.end());
	return vec;
}

vector<int> reversedVector(int size, int max) {
	vector<int> vec = generateRandomVector(size, max);
	sort(vec.begin(), vec.end());
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
	sort(v.begin(), v.end());
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
	cout << "Manual timing: Algorithm Sort took " << duration.count() / 1000.0 << " milliseconds " << endl;
}

double measureAverageTime(int runs, int size, int max) {
	double totalTime = 0.0;
	for (int i = 0; i < runs; i++) {
		vector<int> testArray = generateRandomVector(size, max);
		auto start = chrono::high_resolution_clock::now();
		sort(testArray.begin(), testArray.end());
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
	cout << "---------------------------------" << endl;

	cout << "Test Array Nearly Sorted:" << endl;
	vector<int> arr2 = nearlySortedVector(size, max, 0.8);
	manualTiming(arr2, size);
	cout << "---------------------------------" << endl;

	cout << "Test Array Sorted:" << endl;
	vector<int> arr3 = sortedVector(size, max);
	manualTiming(arr3, size);
	cout << "---------------------------------" << endl;

	cout << "Test Array Reversed:" << endl;
	vector<int> arr4 = reversedVector(size, max);
	manualTiming(arr4, size);
	cout << "---------------------------------" << endl;

	int runs = 10;
	double totalTime = 0.0;
	for (int i = 0; i < runs; i++) {
		vector<int> v1 = generateRandomVector(size, max);
		double SortTime1 = measureExecutionTime([&]() {
			sort(v1.begin(), v1.end());
			});

		vector<int> v2 = nearlySortedVector(size, max, 0.8);
		double SortTime2 = measureExecutionTime([&]() {
			sort(v2.begin(), v2.end());
			});

		vector<int> v3 = sortedVector(size, max);
		double SortTime3 = measureExecutionTime([&]() {
			sort(v3.begin(), v3.end());
			});

		vector<int> v4 = reversedVector(size, max);
		double SortTime4 = measureExecutionTime([&]() {
			sort(v4.begin(), v4.end());
			});

		double tb = (SortTime1 + SortTime2 + SortTime3 + SortTime4) / 4;
		totalTime += tb;
	}
	cout << "Averange sort time over " << runs << " runs: " << totalTime / runs << " milliseconds" << endl;
	return 0;
}