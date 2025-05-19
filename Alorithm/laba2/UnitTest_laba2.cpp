#include "pch.h"
#include "CppUnitTest.h"
#include "../alg_laba2/alg_laba2.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestlaba2
{
	TEST_CLASS(UnitTestlaba2)
	{
	public:
		
		TEST_METHOD(TestMinimal)
		{
			double result = simpson_rule(function, 5, 7, 2); // минимальоне четное кол-во сегментов
			Assert::AreEqual(2.0, result); //почему-то выдвает ошибку если вводить не дробью, хотя тип дабл
		}

		TEST_METHOD(TestAutoCorrection)
		{
			double result = simpson_rule(function, 5, 7, 5); // функция сама меняет нечетное на ближайшее чётное
			Assert::AreEqual(2.0, result);
		}

		TEST_METHOD(TestGreatCountOfSegments)
		{
			double result = simpson_rule(function, 5, 7, 100); // большое число сегментов
			Assert::AreEqual(2.0, result);
		}
	};
}
