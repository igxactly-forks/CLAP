#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../clap.hpp"

namespace UnitTests
{
	TEST_CLASS(UnitTestsWin32)
	{
	public:
		
        int const argc = 6;
        char const* argv[6];

        TEST_CLASS_INITIALIZE(InitClassWithFlags)
        {
            argv[0] = 'cla-test.exe';
            argv[1] = '/?';
            argv[2];
        }

        TEST_METHOD_INITIALIZE(InitMethodsWithParsing)
        {
            
        }

		TEST_METHOD(QueryingForExistingFlagsYieldsTrue)
		{

		}
	};
}
