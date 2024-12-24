#include "pch.h"
#include "CppUnitTest.h"
#include "../Task1/hh.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:

		float x = 0;
		float y = 1;
		float test = 0;
		float r = pow(pow(y, 2) + pow(x, 2), (1 / 2)) / x;
		float s = pow(cos((asin(1.0) / 2) * x), 2);
		// Один тестовый метод
		TEST_METHOD(TestAddFunction)
		{
			Assert::AreEqual(TwoFunk(3, 4), 0.5f);
			// обычные числа
		}
		TEST_METHOD(TestNegativeNumb)
		{
			Assert::AreEqual(TwoFunk(-3, -4), 0.5f);
			// отриц
		}
		TEST_METHOD(TestInf)
		{
			Assert::AreEqual(TwoFunk(0, 1), INFINITY);
			// на бесконечность
		}
		TEST_METHOD(MixtedNumb)
		{
			Assert::AreEqual(TwoFunk(-3, 4), 0.5f);
			// микст
		}
		TEST_METHOD(isSmax)
		{
			Assert::IsTrue(s > r);
		}
	};
}
