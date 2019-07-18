#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../clap.h"
#include "../clap.hpp"

namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{
	public:
		
        int const argc = 6;
        char const* argv[6];

        TEST_CLASS_INITIALIZE(InitClassWithFlags)
        {

        }

        TEST_METHOD_INITIALIZE(InitMethodsWithParsing)
        {

        }

		TEST_METHOD(QueryingForExistingFlagsYieldsTrue)
		{

		}
	};
}
