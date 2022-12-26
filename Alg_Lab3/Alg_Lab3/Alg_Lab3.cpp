
#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING  1
#include <gtest/gtest.h>
#include <iostream>
#include "Sort.h"




using namespace std;


TEST(Q_Sort_10, QSort10)
{
    int size = 1000;
    int* a = (int*)malloc(size * sizeof(int));
    int* b = (int*)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        a[i] = rand() % 100;
        b[i] = a[i];
    }
    quickSort(a, a + size, [](int a, int b) {return a > b; }, false);

    std::sort(b, b + size, [](int a, int b) {return a > b; });


    for (int i = 0; i < size; ++i) {
        EXPECT_EQ(a[i], b[i]) << "Sorting err" << i;
        
    }
    free(a);
    free(b);
}

TEST(Q_Sort_1, QSort1)
{
    int* a = (int*)malloc(1 * sizeof(int));
    int* b = (int*)malloc(1 * sizeof(int));
    for (int i = 0; i < 1; i++)
    {
        a[i] = rand() % 100;
        b[i] = a[i];
    }
    quickSort(a, a, [](int a, int b) {return a > b; }, false);
    std::sort(b, b + 1, [](int a, int b) {return a > b; });

    for (int i = 0; i < 1; ++i) {
        EXPECT_EQ(a[i], b[i]) << "Sorting err" << i;

    }
    free(a);
    free(b);
}

TEST(Q_Sort_IndenticalFloat, QSortIdentFloat)
{
    float* a = (float*)malloc(100 * sizeof(float));
    float* b = (float*)malloc(100 * sizeof(float));
    for (int i = 0; i < 100; i++)
    {
        a[i] = 1.5f;
        b[i] = a[i];
    }
    quickSort(a, a + 100, [](float a, float b) {return a > b; }, false);
    std::sort(b, b + 100, [](float a, float b) {return a > b; });

    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(a[i], b[i]) << "Sorting err" << i;

    }
    free(a);
    free(b);
}

TEST(Q_Sort_Str, QSortStr)
{
    const int size = 100;
    std::string a[size];
    std::string b[size];
    a[0] = "a";
    b[0] = "a";

    for (int i = 1; i < size; i++)
    {
        a[i] = a[i - 1].append("a");
        b[i] = b[i - 1].append("a");
    }

    std::sort(a, a + size, [](string a, string b) {return a.length() > b.length(); });
    quickSort(b, b + size, [](string& a, string& b) {return a.length() > b.length(); }, false);

    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(a[i], b[i]) << "Sorting err" << i;

    }
}

TEST(I_Sort_1000, ISort1000)
{
    const int size = 1000;
    int a[size];
    int b[size];

    srand(13);
    for (int i = 0; i < size; i++)
    {
        a[i] = rand() % 1000;
        b[i] = a[i];
    }

    std::sort(a, a + size, [](int a, int b) {return a<b; });
    insertionSort(b, b + size, [](int a, int b) {return a < b; });

    for (int i = 0; i < size; ++i) {
        EXPECT_EQ(a[i], b[i]) << "Sorting err" << i;
    }

}


TEST(I_Sort_Str_100, ISortStr100)
{
    const int size = 100;
    string a[size];
    string b[size];

    a[0] = "a";
    b[0] = "a";
    for (int i = 1; i < size; i++)
    {
        a[i] = a[i - 1].append("a");
        b[i] = b[i-1].append("a");
    }

    std::sort(a, a + size, [](string a, string b) {return a.length() > b.length(); });
    insertionSort(b, b + size, [](string a, string b) {return a.length() > b.length(); });

    for (int i = 0; i < size; ++i) {
        EXPECT_EQ(a[i], b[i]) << "Sorting err" << i;
    }

}

TEST(I_Sort_1, ISort1)
{
    const int size = 1;
    int a[size];
    int b[size];

    srand(13);
    for (int i = 0; i < size; i++)
    {
        a[i] = rand() % 1000;
        b[i] = a[i];
    }

    std::sort(a, a + size, [](int a, int b) {return a < b; });
    insertionSort(b, b + size, [](int a, int b) {return a < b; });

    for (int i = 0; i < size; ++i) {
        EXPECT_EQ(a[i], b[i]) << "Sorting err" << i;
    }

}

TEST(H_Sort_1000, Hybryd_Sort_1000)
{
    const int size = 1000;
    int a[size];
    int b[size];

    srand(13);
    for (int i = 0; i < size; i++)
    {
        a[i] = rand() % 1000;
        b[i] = a[i];
    }

    std::sort(a, a + size, [](int a, int b) {return a < b; });
    quickSort(b, b + size, [](int a, int b) {return a < b; },true);

    for (int i = 0; i < size; ++i) {
        EXPECT_EQ(a[i], b[i]) << "Sorting err" << i;
    }

}

TEST(H_Sort_1000_R, Hybryd_Sort_1000_R)
{
    const int size = 1000;
    int a[size];
    int b[size];

    srand(13);
    for (int i = 0; i < size; i++)
    {
        a[i] = rand() % 1000;
        b[i] = a[i];
    }

    std::sort(a, a + size, [](int a, int b) {return a > b; });
    quickSort(b, b + size, [](int a, int b) {return a > b; }, true);

    for (int i = 0; i < size; ++i) {
        EXPECT_EQ(a[i], b[i]) << "Sorting err" << i;
    }

}

int main(int argc, char* argv[])
{
   //performanceCheck();
   testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();

}


