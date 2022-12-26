#pragma once

#include <iostream>
#include <Windows.h>
#include <time.h>
double PCFreq = 0.0;
__int64 CounterStart = 0;

int constexpr THRESHOLD = 16;

template <typename T>
void swap(T* t1, T* t2)
{
    T temp = std::move(*t1);
    *t1 = std::move(*t2);
    *t2 = std::move(temp);
}

void StartCounter()
{
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        std::cout << "QueryPerformanceFrequency failed!\n";
    PCFreq = double(li.QuadPart) / 1000.0;
    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - CounterStart) / PCFreq;
}

template <typename T, typename Compare>
T pickMedian(T* first, T* middle, T* last, Compare compare)
{
    if (compare(*first, *middle))
    {
        if (compare(*middle, *last))
            return *middle;
        else if (compare(*first, *last))
            return *last;
        else
            return *first;
    }
    else
    {
        if (compare(*first, *last))
            return *first;
        else if (compare(*middle, *last))
            return *last;
        else
            return *middle;
    }
}

template <typename T, typename Compare>
T* partition(T* left, T* right, Compare compare)
{
    T pivot = std::move(pickMedian(left, left + (right - left) / 2, right, compare));
    T* i = left;
    T* j = right-1;
    while (i < j)
    {
        while (compare(*i, pivot))
            i++;
        while (compare(pivot, *j))
            j--;

        if (*i == *j)
            i++;

        if (i <= j)
        {
            swap(i, j);
        }
    }
    return j;
}

template <typename T, typename Compare>
void quickSort(T* first, T* last, Compare compare, bool isHybrid)
{

    if (first<last&&last - first <= THRESHOLD && isHybrid)
    {
        insertionSort(first, last, compare);
    }
    else
    {
        while (first < last) {
            T* pivot = partition(first, last, compare);

            if (pivot - first < last - pivot + 1) {
                quickSort(first, pivot, compare, isHybrid);
                first = pivot + 1;
            }
            else {
                quickSort(pivot + 1, last, compare, isHybrid);
                last = pivot;
            }
        }
    }
}

template <typename T, typename Compare>
void insertionSort(T* first, T* last, Compare comp)
{

    T* right;
    T key;
    for (T* left = (first + 1); left < last; left++)
    {
        key = std::move(*left);
        right = left - 1;

        while (right >= first && !comp(*right, key))
        {
            *(right + 1) = std::move(*right);
            right--;
        }
        *(right + 1) = std::move(key);
    }
}

void performanceCheck()
{
    double PCFreq = 0.0;
    __int64 CounterStart = 0;


    int* a = (int*)malloc(1000 * sizeof(int));

    for (int it = 1; it < 100; it++)
    {
        double timeQuick = 0;
        int n = 10000;

        for (int step = 0; step < n; step++)
        {

            for (int i = 0; i < it; i++)
                a[i] = it - i;

            StartCounter();
            insertionSort(a, a + it, [](int a, int b) {return a < b; });

            double diff_sec = GetCounter();
            timeQuick += diff_sec;


        }

        std::cout << timeQuick * 1000 / n << std::endl;
    }
    std::cout << std::endl;
    srand((unsigned int)time(NULL));

    for (int it = 1; it < 100; it++)
    {
        double timeQuick = 0;
        int n = 10000;



        for (int step = 0; step < n; step++)
        {

            for (int i = 0; i < it; i++)
                a[i] = it-i;


            StartCounter();

            quickSort(a, a + it, [](int a, int b) {return a > b; }, false);
            //insertionSort(a, a + it, [](int a, int b) {return a < b; });

            double diff_sec = GetCounter();
            timeQuick += diff_sec;


        }
        std::cout << timeQuick * 1000 / n << std::endl;
    }

    free(a);
}