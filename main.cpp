#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

const int processor_count = thread::hardware_concurrency();

int* mergesort(int size1, int* arr1, int size2, int* arr2){
    int* ans = new int[size1 + size2];
    int i = 0;
    int j = 0;
    for (int k = 0; k < size1 + size2; k++){
        if (i >= size1){
            ans[k] = arr2[j];
            continue;
        }
        if (j >= size2){
            ans[k] = arr1[i];
            continue;
        }
        if (arr1[i] < arr2[j]){
            ans[k] = arr1[i];
            i++;
        }else{
            ans[k] = arr2[j];
            j++;
        }
    }
    return ans;
}


int* sort(int size, int* arr){
    int size1 = size / 2;
    int size2 = size - size1;
    int* arr1 = new int[size1];
    int* arr2 = new int[size2];
    for (int i = 0; i < size; i++){
        if (i < size1){
            arr1[i] = arr[i];
        }else{
            arr2[i - size1] = arr[i];
        }
    }
#pragma omp parallel sections num_threads(processor_count)
    {
        #pragma omp section
        {
            if (size1 != 1){
                arr1 = sort(size1, arr1);
            }
        }
        #pragma omp section
        {
            if (size2 != 1){
                arr2 = sort(size2, arr2);
            }
        }
    }
    int* ans = mergesort(size1, arr1, size2, arr2);
    delete[] arr1;
    delete[] arr2;
    return ans;
}

int main()
{
    const int n = 10000;
    int* arr = new int[n];
    int* ans;
    for (int i = 0; i < n; i++){
        arr[i] = rand() % 100;
    }
    auto begin_time = chrono::steady_clock::now();
    ans = sort(n, arr);
    auto end_time = chrono::steady_clock::now();
    auto elapsed_ms = chrono::duration_cast<chrono::nanoseconds>(end_time - begin_time);

    for (int i = 0; i < n; i++){
        cout << ans[i] << " ";
    }
    cout << endl;

    cout << "Ответ получен за: " << elapsed_ms.count() << "ns" << endl;

    delete[] ans;

    return 0;
}
