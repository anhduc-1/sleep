#include <iostream>
#include <vector>
#include <random>
#include <chrono>

using namespace std;


void merge(vector<int>& v, int left, int mid, int right) {
	int n1 = mid - left + 1, n2 = right - mid;
	vector<int> L(n1), R(n2);

	for (int i = 0; i < n1; i++) L[i] = v[left + i];
	for (int i = 0; i < n2; i++) R[i] = v[mid + 1 + i];

	int i = 0, j = 0, k = left;
	while (i < n1 && j < n2) {
		if (L[i] < R[j]) v[k++] = L[i++];
		else v[k++] = R[j++];
	}
	while (i < n1) v[k++] = L[i++];
	while (j < n2) v[k++] = R[j++];
}

void naturalMergeSort(vector<int>& v) {
	int n = v.size();
	bool sorted = false;
	while (!sorted) {
		sorted = true;
		int left = 0, mid = 0, right = 0;
		while (left < n) {
			mid = left;
			while (mid < n - 1 && v[mid] <= v[mid + 1]) mid++;
			right = mid + 1;
			if (right < n - 1) {
				while (right < n - 1 && v[right] <= v[right + 1]) right++;
				merge(v, left, mid, right);
				sorted = false;
				left = right + 1;
			}
			else left = n;
		}
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
	naturalMergeSort(vec);

	uniform_int_distribution<int> indexDistrib(0, size - 1);
	float num_swaps = (1 - percent) / 2 * size;
	for (int i = 0; i < (int)num_swaps; i++) {
		int idx1 = indexDistrib(gen);
		int idx2 = indexDistrib(gen);
		swap(vec[idx1], vec[idx2]);
	}

	return vec;
}

vector<int> sortedVector(int size, int max) {
	vector<int> vec = generateRandomVector(size, max);
	naturalMergeSort(vec);
	return vec;
}


vector<int> reversedVector(int size, int max) {
	vector<int> vec = generateRandomVector(size, max);
	naturalMergeSort(vec);
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
	naturalMergeSort(v);
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
	cout << "Manual timing: Natural Merge Sort took " << duration.count() / 1000.0 << " milliseconds " << endl;
}

double measureAverageTime(int runs, int size, int max) {
	double totalTime = 0.0;
	for (int i = 0; i < runs; i++) {
		vector<int> testArray = generateRandomVector(size, max);
		auto start = chrono::high_resolution_clock::now();
		naturalMergeSort(testArray);
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

	cout << "Test Array Reversed:" << endl;
	vector<int> arr4 = reversedVector(size, max);
	manualTiming(arr4, size);
	cout << "---------------------------------" << endl;

	int runs = 10;
	double totalTime = 0.0;
	for (int i = 0; i < runs; i++) {
		vector<int> v1 = generateRandomVector(size, max);
		double naturalMergeSortTime1 = measureExecutionTime([&]() {
			naturalMergeSort(v1);
			});

		vector<int> v2 = nearlySortedVector(size, max, 0.8);
		double naturalMergeSortTime2 = measureExecutionTime([&]() {
			naturalMergeSort(v2);
			});

		vector<int> v3 = sortedVector(size, max);
		double naturalMergeSortTime3 = measureExecutionTime([&]() {
			naturalMergeSort(v3);
			});

		vector<int> v4 = reversedVector(size, max);
		double naturalMergeSortTime4 = measureExecutionTime([&]() {
			naturalMergeSort(v4);
			});

		double tb = (naturalMergeSortTime1 + naturalMergeSortTime2 + naturalMergeSortTime3 + naturalMergeSortTime4) / 4;
		totalTime += tb;
	}
	cout << "Averange natural merge sort time over " << runs << " runs: " << totalTime / runs << " milliseconds" << endl;
	return 0;
}