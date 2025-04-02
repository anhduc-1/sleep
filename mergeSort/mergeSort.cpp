#include <iostream>
#include <vector>
#include <chrono>
#include <random>

using namespace std;


void merge(vector<int>& v, int left, int mid, int right) {
	int n1 = mid - left + 1, n2 = right - mid;
	vector<int> L(n1), R(n2);

	for (int i = 0; i < n1; i++) L[i] = v[left + i];
	for (int i = 0; i < n2; i++) R[i] = v[mid + i + 1];

	int l = 0, r = 0, k = left;
	while (l < n1 && r < n2) {
		if (L[l] <= R[r])
			v[k++] = L[l++];
		else
			v[k++] = R[r++];
	}
	while (l < n1) v[k++] = L[l++];
	while (r < n2) v[k++] = R[r++];
}

void mergeSort(vector<int>& v, int left, int right) {
	if (left < right) {
		int mid = left + (right - left) / 2;
		mergeSort(v, left, mid);
		mergeSort(v, mid + 1, right);
		merge(v, left, mid, right);
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
	mergeSort(vec, 0, size - 1);

	float num_swaps = (1 - percent) / 2 * size;
	uniform_int_distribution<> idxdistrib(0, size - 1);
	for (int i = 0; i < (int)num_swaps; i++) {
		swap(vec[idxdistrib(gen)], vec[idxdistrib(gen)]);
	}

	return vec;
}

vector<int> sortedVector(int size, int max) {
	vector<int> vec = generateRandomVector(size, max);
	mergeSort(vec, 0, size - 1);
	return vec;
}

vector<int> reversedVector(int size, int max) {
	vector<int> vec = sortedVector(size, max);
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
	mergeSort(v, 0, size - 1);
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
	cout << "Manual timing: Merge Sort took " << duration.count() / 1000.0 << " milliseconds " << endl;
}

double measureAverageTime(int runs, int size, int max) {
	double totalTime = 0.0;
	for (int i = 0; i < runs; i++) {
		vector<int> testArray = generateRandomVector(size, max);
		auto start = chrono::high_resolution_clock::now();
		mergeSort(testArray, 0, size - 1);
		auto end = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
		totalTime += duration.count();
	}
	return (totalTime / runs) / 1000.0;
}

int main()
{
	int size = 5000;
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

	cout << "Test Array Reverse Sorted:" << endl;
	vector<int> arr4 = reversedVector(size, max);
	manualTiming(arr4, size);
	cout << "---------------------------------" << endl;

	int runs = 10;
	double totalTime = 0.0;
	for (int i = 0; i < runs; i++) {
		vector<int> v1 = generateRandomVector(size, max);
		double mergeSortTime1 = measureExecutionTime([&]() {
			mergeSort(v1, 0, size - 1);
			});

		vector<int> v2 = nearlySortedVector(size, max, 0.8);
		double mergeSortTime2 = measureExecutionTime([&]() {
			mergeSort(v2, 0, size - 1);
			});

		vector<int> v3 = sortedVector(size, max);
		double mergeSortTime3 = measureExecutionTime([&]() {
			mergeSort(v2, 0, size - 1);
			});

		vector<int> v4 = reversedVector(size, max);
		double mergeSortTime4 = measureExecutionTime([&]() {
			mergeSort(v3, 0, size - 1);
			});

		double tb = (mergeSortTime1 + mergeSortTime2 + mergeSortTime3 + mergeSortTime4) / 4;
		totalTime += tb;
	}
	cout << "Averange merge sort time over " << runs << " runs: " << totalTime / runs << " milliseconds" << endl;
	return 0;
}