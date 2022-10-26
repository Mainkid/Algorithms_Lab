// Alg_Lab2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Array.h"

void test1()
{
	Array <int> arr(4);
	arr.insert(1);
	arr.insert(3);
	arr.insert(1, 2);
	arr.insert(4);
	arr.insert(4, 100);

	arr[0] = -1;
	arr.remove(0);

	for (int i = 0; i < arr.size(); i++)
	{
		std::cout << arr[i] << std::endl;
	}

	Array <int> arr2(10);

	arr2 = arr;


	for (int i = 0; i < arr2.size(); i++)
	{
		std::cout << arr2[i] << std::endl;
	}

	arr2.remove(0);
	arr2.insert(42);

	for (int i = 0; i < arr2.size(); i++)
	{
		std::cout << arr2[i] << std::endl;
	}

	for (int i = 0; i < arr.size(); i++)
	{
		std::cout << arr[i] << std::endl;
	}
}

void test2()
{
	Array <int> arr(2);
	Array <int> arr2;

	arr2 = arr;

	arr.insert(1);

	arr.insert(2);
	arr2.insert(3);
	arr.insert(4);
	arr.insert(5);

	for (int i = 0; i < arr.size(); i++)
	{
		std::cout << arr[i] << std::endl;
	}

	for (int i = 0; i < arr2.size(); i++)
	{
		std::cout << arr2[i] << std::endl;
	}
}

void test3()
{
	Array<int> a;
	for (int i = 0; i < 10; ++i)
		a.insert(i + 1);

	for (int i = 0; i < a.size(); ++i)
		a[i] *= 2;

	for (auto it = a.iterator(); it.hasNext(); it.next())
	{
		it.set(2.5);
		std::cout << it.get() << std::endl;
	}
}

int main()
{
	test1();
}

