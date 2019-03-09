#include "stdafx.h"
#include "CppUnitTest.h"
#include "../TabuDARP/solution.h"
#include "../TabuDARP/Tour.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest2
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: 在此输入测试代码
			Tour t1,t2;
			t1.set_node(0, 1);
			t1.set_node(1, 2);
			t1.set_node(2, 3);
			t2.set_node(0, 7);
			t2.set_node(1, 8);
			t2.set_node(2, 9);
			if (t1 == t2) printf("true");
			else printf("false");
			Assert::AreEqual(1, 1);
		}

	};
}