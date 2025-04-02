#include <iostream>
#include <vector>
#include <chrono>
#include <random>

using namespace std;


void heapify(vector<int>& v, int n, int i) {
	int largest = i;
	int l = 2 * i + 1, r = 2 * i + 2;

	if (l < n && v[l] > v[largest]) largest = l;
	if (r < n && v[r] > v[largest]) largest = r;

	if (largest != i) {
		swap(v[i], v[largest]);
		heapify(v, n, largest);
	}
}

void heapSort(vector<int>& v) {
	int n = v.size();
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(v, n, i);
	for (int i = n - 1; i >= 0; i--) {
		swap(v[0], v[i]);
		heapify(v, i, 0);
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
	heapSort(vec);

	float num_swaps = (1 - percent) / 2 * size;
	uniform_int_distribution<> idxdistrib(0, size - 1);

	for (int i = 0; i < num_swaps; i++) {
		swap(vec[idxdistrib(gen)], vec[idxdistrib(gen)]);
	}

	return vec;
}

vector<int> sortedVector(int size, int max) {
	vector<int> vec = generateRandomVector(size, max);
	heapSort(vec);
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
	heapSort(v);
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
	cout << "Manual timing: Heap Sort took " << duration.count() / 1000.0 << " milliseconds " << endl;
}

double measureAverageTime(int runs, int size, int max) {
	double totalTime = 0.0;
	for (int i = 0; i < runs; i++) {
		vector<int> testArray = generateRandomVector(size, max);
		auto start = chrono::high_resolution_clock::now();
		heapSort(testArray);
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
	
	vector<int> arr1 = generateRandomVector(size, max);
	cout << "Test Array Random:" << endl;
	manualTiming(arr1, size);
	cout << "--------------------------------" << endl;

	vector<int> arr2 = nearlySortedVector(size, max, 0.8);
	cout << "Test Array Nearly Sorted:" << endl;
	manualTiming(arr2, size);
	cout << "--------------------------------" << endl;

	vector<int> arr3 = sortedVector(size, max);
	cout << "Test Array Sorted:" << endl;
	manualTiming(arr3, size);
	cout << "--------------------------------" << endl;

	vector<int> arr4 = reversedVector(size, max);
	cout << "Test Array Reversed:" << endl;
	manualTiming(arr4, size);
	cout << "--------------------------------" << endl;

	int runs = 10;
	double totalTime = 0.0;
	for (int i = 0; i < runs; i++) {
		vector<int> v1 = generateRandomVector(size, max);
		double heapSortTime1 = measureExecutionTime([&]() {
			heapSort(v1);
			});

		vector<int> v2 = nearlySortedVector(size, max, 0.8);
		double heapSortTime2 = measureExecutionTime([&]() {
			heapSort(v2);
			});

		vector<int> v3 = sortedVector(size, max);
		double heapSortTime3 = measureExecutionTime([&]() {
			heapSort(v3);
			});

		vector<int> v4 = reversedVector(size, max);
		double heapSortTime4 = measureExecutionTime([&]() {
			heapSort(v4);
			});

		double tb = (heapSortTime1 + heapSortTime2 + heapSortTime3 + heapSortTime4) / 4;
		totalTime += tb;
	}
	cout << "Averange heap sort time over " << runs << " runs: " << totalTime / runs << " milliseconds" << endl;
	return 0;
}