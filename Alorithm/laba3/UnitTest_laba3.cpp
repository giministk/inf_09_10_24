#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include "../ajg_laba3/ajg_laba3.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SortingTests
{
    TEST_CLASS(InsertSortTests)
    {
    public:
        TEST_METHOD(InsertNoNeedToSortArray)
        {
            std::vector<int> arr = { 1, 2, 3, 4, 5 };
            std::vector<int> expected = { 1, 2, 3, 4, 5 };
            insert_sort(arr);
            Assert::IsTrue(std::is_sorted(arr.begin(), arr.end()));
        }

        TEST_METHOD(InsertSortReverseArray)
        {
            std::vector<int> arr = { 5, 4, 3, 2, 1 };
            std::vector<int> expected = { 1, 2, 3, 4, 5 };
            insert_sort(arr);
            Assert::IsTrue(std::is_sorted(arr.begin(), arr.end()));
        }

        TEST_METHOD(InsertSortRandomArray)
        {
            std::vector<int> arr = { 3, 1, 4, 1, 5, 9, 2, 6 };
            std::vector<int> expected = { 1, 1, 2, 3, 4, 5, 6, 9 };
            insert_sort(arr);
            Assert::IsTrue(std::is_sorted(arr.begin(), arr.end()));
        }
    };

    TEST_CLASS(RadixSortTests)
    {
    public:
        TEST_METHOD(RadixNoNeedToSortArray)
        {
            std::vector<int> arr = { 1, 2, 3, 4, 5 };
            std::vector<int> expected = { 1, 2, 3, 4, 5 };
            radix_sort(arr);
            Assert::IsTrue(std::is_sorted(arr.begin(), arr.end()));
        }

        TEST_METHOD(RadixSortReverseArray)
        {
            std::vector<int> arr = { 5, 4, 3, 2, 1 };
            std::vector<int> expected = { 1, 2, 3, 4, 5 };
            radix_sort(arr);
            Assert::IsTrue(std::is_sorted(arr.begin(), arr.end()));
        }

        TEST_METHOD(RadixSortRandomArray)
        {
            std::vector<int> arr = { 3, 1, 4, 1, 5, 9, 2, 6 };
            std::vector<int> expected = { 1, 1, 2, 3, 4, 5, 6, 9 };
            radix_sort(arr);
            Assert::IsTrue(std::is_sorted(arr.begin(), arr.end()));
        }
    };

    TEST_CLASS(FastSortTests)
    {
    public:
        TEST_METHOD(FastNoNeedToSortArray)
        {
            std::vector<int> arr = { 1, 2, 3, 4, 5 };
            std::vector<int> expected = { 1, 2, 3, 4, 5 };
            fast_sort(arr, 0, arr.size() - 1);
            Assert::IsTrue(std::is_sorted(arr.begin(), arr.end()));
        }

        TEST_METHOD(FastSortReverseArray)
        {
            std::vector<int> arr = { 5, 4, 3, 2, 1 };
            std::vector<int> expected = { 1, 2, 3, 4, 5 };
            fast_sort(arr, 0, arr.size() - 1);
            Assert::IsTrue(std::is_sorted(arr.begin(), arr.end()));
        }

        TEST_METHOD(FastSortRandomArray)
        {
            std::vector<int> arr = { 3, 1, 4, 1, 5, 9, 2, 6 };
            std::vector<int> expected = { 1, 1, 2, 3, 4, 5, 6, 9 };
            fast_sort(arr, 0, arr.size() - 1);
            Assert::IsTrue(std::is_sorted(arr.begin(), arr.end()));
        }
    };
}