// Alg_Lab2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Array.h"
#include <gtest/gtest.h>


TEST(InsertWithoutExpansionInt, T1)
{
	Array <int> arr(4);
	arr.insert(1);
	arr.insert(2);
	arr.insert(3);

	for (int i = 0; i < arr.size(); i++)
	{
		EXPECT_EQ(arr[i], i+1) << "Sorting err" << i;
		//std::cout << arr[i] << std::endl;
	}
}

TEST(InsertWithExpansionInt, T2)
{
	Array <int> arr(4);
	arr.insert(1);
	arr.insert(2);
	arr.insert(3);
	arr.insert(4);
	arr.insert(5);
	arr.insert(6);

	for (int i = 0; i < arr.size(); i++)
	{
		EXPECT_EQ(arr[i], i + 1) << "Sorting err" << i;
		//std::cout << arr[i] << std::endl;
	}

}

TEST(InsertIndexWithExpansionInt, T3)
{
	Array <int> arr(4);
	arr.insert(1,0);
	arr.insert(2,0);
	arr.insert(3,0);
	arr.insert(4,0);
	arr.insert(5,0);

	for (int i = 0; i < arr.size(); i++)
	{
		EXPECT_EQ(arr[i], arr.size()-i) << "Sorting err" << i;
		//std::cout << arr[i] << std::endl;
	}

}

TEST(InsertNotCorrectIndexInt, T4)
{
	Array <int> arr(4);
	arr.insert(1);
	arr.insert(2);
	arr.insert(3);
	arr.insert(4);
	arr.insert(5);
	arr.insert(10,6);
	arr.insert(144, 10);

	for (int i = 0; i < arr.size(); i++)
	{
		EXPECT_EQ(arr[i], i + 1) << "Sorting err" << i;
		//std::cout << arr[i] << std::endl;
	}

}

TEST(ChangeValueInt, T5)
{
	Array <int> arr(4);
	arr.insert(1);
	arr.insert(2);
	arr.insert(3);
	arr.insert(4);
	arr.insert(5);
	arr.insert(10, 6);
	arr.insert(144, 10);

	for (int i = 0; i < arr.size(); i++)
	{
		arr[i] = 0;
		EXPECT_EQ(arr[i], 0) << "Sorting err" << i;
		//std::cout << arr[i] << std::endl;
	}
}

TEST(InsertWithoutExpansionStrLong, T6)
{
	Array <std::string> arr(4);
	arr.insert("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	arr.insert("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	arr.insert("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

	for (int i = 0; i < arr.size(); i++)
	{
		EXPECT_EQ(arr[i],"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa") << "Sorting err" << i;
		//std::cout << arr[i] << std::endl;
	}

}

TEST(InsertWithExpansionStrLong, T7)
{
	Array <std::string> arr(4);
	arr.insert("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	arr.insert("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	arr.insert("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	arr.insert("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	arr.insert("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	arr.insert("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

	for (int i = 0; i < arr.size(); i++)
	{
		EXPECT_EQ(arr[i], "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa") << "Sorting err" << i;
		//std::cout << arr[i] << std::endl;
	}

}

TEST(RemoveElementWithoutExpansionInt, T8)
{
	Array <int> arr(4);
	arr.insert(1);
	arr.insert(2);
	arr.insert(3);
	arr.remove(0);
	arr.remove(1);

	for (int i = 0; i < arr.size(); i++)
	{
		EXPECT_EQ(arr[i], 2) << "Sorting err" << i;
		//std::cout << arr[i] << std::endl;
	}

}

TEST(RemoveElementWithExpansionInt, T9)
{
	Array <int> arr(2);
	arr.insert(1);
	arr.insert(2);
	arr.insert(3);
	arr.remove(0);
	arr.remove(1);

	for (int i = 0; i < arr.size(); i++)
	{
		EXPECT_EQ(arr[i], 2) << "Sorting err" << i;
		//std::cout << arr[i] << std::endl;
	}

}

TEST(RemoveElementWithExpansionStr, T10)
{
	Array <std::string> arr(4);
	arr.insert("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	arr.insert("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
	arr.insert("c");
	arr.insert("dddddddddddddddddddddddddddddddddddddddddddddddddddddd");
	arr.insert("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	arr.insert("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	arr.remove(0);
	arr.remove(4);
	arr.remove(3);
	arr.remove(0);

	EXPECT_EQ(arr[0], "c") << "Sorting err";
	EXPECT_EQ(arr[1], "dddddddddddddddddddddddddddddddddddddddddddddddddddddd") << "Sorting err";

}

TEST(RemoveElementIncorrectIndex, T11)
{
	Array <int> arr(4);
	arr.insert(1);
	arr.insert(2);
	arr.insert(3);
	arr.remove(-1);

	for (int i = 0; i < arr.size(); i++)
	{
		EXPECT_EQ(arr[i], i+1) << "Sorting err" << i;
		//std::cout << arr[i] << std::endl;
	}
}

TEST(RemoveAllElements, T12)
{
	Array <int> arr(4);
	arr.insert(1);
	arr.insert(2);
	arr.insert(3);
	arr.remove(0);
	arr.remove(0);
	arr.remove(0);
	arr.remove(0);

	for (int i = 0; i < arr.size(); i++)
	{
		EXPECT_EQ(arr[i], i + 1) << "Sorting err" << i;
		//std::cout << arr[i] << std::endl;
	}
}

TEST(InitArrayByOther, T13)
{
	Array <int> arr(4);
	arr.insert(1);
	arr.insert(2);
	arr.insert(3);
	Array <int> arr2 = arr;

	EXPECT_EQ(arr.size(), arr2.size());

	for (int i = 0; i < arr.size(); i++)
	{
		EXPECT_EQ(arr[i], arr2[i]) << "Sorting err" << i;
	}
}

TEST(InitArrayByOther2, T14)
{
	Array <std::string> arr(4);
	arr.insert("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	arr.insert("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
	arr.insert("cccccccccccccccccccccccccccccccccccccccc");
	Array <std::string> arr2 = arr;

	EXPECT_EQ(arr.size(), arr2.size());

	for (int i = 0; i < arr.size(); i++)
	{
		EXPECT_EQ(arr[i], arr2[i]) << " err" << i;
	}
}

TEST(MoveSemantics, T15)
{
	Array<int> a;
	Array<int> b;
	for (int i = 0; i < 10; ++i)
		a.insert(i + 1);

	b = std::move(a);
	EXPECT_EQ(b.size(), 10);
	for (int i = 0; i < 10; ++i)
	{
		EXPECT_EQ(b[i], i + 1);
	}
}

TEST(IteratorInCycle, T16)
{
	Array<int> a;
	for (int i = 0; i < 10; ++i)
		a.insert(i + 1);

	for (int i = 0; i < a.size(); ++i)
		a[i] *= 2;

	int i = 1;
	for (auto it = a.iterator(); it.isLast(); it.next())
	{
		EXPECT_EQ(it.get(), (i) * 2);
		i++;
	}

}


TEST(ReverseIteratorInCycle, T17)
{
	Array<int> a;
	for (int i = 10; i > 0; i--)
		a.insert(i);


	int i = 1;
	for (auto it = a.reverseIterator(); it.isLast(); it.next())
	{
		EXPECT_EQ(it.get(), (i) );
		i++;
	}
}

TEST(IteratorSetInCycle, T18)
{
	Array<int> a;
	for (int i = 0; i < 10; i++)
		a.insert(i);


	int i = 1;
	for (auto it = a.iterator(); it.isLast(); it.next())
	{
		it.set(0);
	}

	for (auto it = a.iterator(); it.isLast(); it.next())
	{
		EXPECT_EQ(it.get(), 0);
	}



}

void test1()
{
	Array <int> arr(4);
	arr.insert(1);
	arr.insert(2);
	//arr.insert(1, 2);
	arr.insert(3);
	arr.insert(4);
	arr.insert(5);

	arr.remove(2);
	//arr.insert(4, 100);

	arr[0] = -1;
	arr.remove(2);

	for (int i = 0; i < arr.size(); i++)
	{
		std::cout << arr[i] << std::endl;
	}

	//Array <int> arr2(3);
	//arr2.insert(2);
	//arr2 = arr;
	//arr = arr;

	std::cout << "";

	/*for (int i = 0; i < arr2.size(); i++)
	{
		std::cout << arr2[i] << std::endl;
	}*/

	/*for (int i = 0; i < arr2.size(); i++)
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
	}*/
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

	for (auto it = a.iterator(); it.isLast(); it.next())
	{
		it.set(2.5);
		std::cout << it.get() << std::endl;
	}
}

void test4()
{
	Array<int> a;
	Array<int> b;
	for (int i = 0; i < 10; ++i)
		a.insert(i + 1);

	b = std::move(a);

	for (int i = 0; i < 10; ++i)
	{

	}
}

int main(int argc, char* argv[])
{

	//test4();

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();

	//Array<int> a;
	//for (int i = 0; i < 10; ++i)
	//	a.insert(i + 1);
	//for (int i = 0; i < a.size(); ++i)
	//	a[i] *= 2;
	//for (auto it = a.reverseIterator(); it.isLast(); it.next())
	//	std::cout << it.get() << std::endl;
}

