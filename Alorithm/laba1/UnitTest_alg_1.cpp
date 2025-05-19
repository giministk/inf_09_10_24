#include "pch.h"
#include "CppUnitTest.h"
#include "../alg_laba1/alg_laba1.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestalg1
{
	TEST_CLASS(UnitTestalg1)
	{
	public:
		
		TEST_METHOD(FindExistingElement)
		{
			std::vector<int> arr = {1, 2, 3, 4, 5};
			int target = 3;

			int result = linear_search(arr, target);
			Assert::AreEqual(2, result);
		}
		TEST_METHOD(ElementNotFound)
		{
			std::vector<int> arr = {1, 2, 3, 4, 5};
			int target = 6; 

			int result = linear_search(arr, target);
			Assert::AreEqual(-1, result);
		}

		TEST_METHOD(EmptyArray)
		{
			std::vector<int> arr;
			int target = 3;

			int result = linear_search(arr, target);
			Assert::AreEqual(-1, result);
		}
	};
}
