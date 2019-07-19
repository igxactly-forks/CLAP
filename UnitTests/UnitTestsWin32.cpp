#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../clap.hpp"

namespace UnitTests
{
    constexpr int argc = 6;
    char const* argv[argc];

	TEST_CLASS(UnitTestsWin32)
	{
	public:

        TEST_CLASS_INITIALIZE(InitArgv)
        {
            argv[0] = "cla-test.exe";
            argv[1] = "/?";
            argv[2] = "/std:c++17";
            argv[3] = "/Lib:"
                "C:\\Windows\\System32\\kernel.dll,"
                "C:\\Users\\uname\\dev\\include\\mylib.lib,"
                "C:\\include\\global\\somedll.dll";
            argv[4] = "/W3";
            argv[5] = "/Wd:123,456,789,abc,def,ghi,lmn,opq,rst,uvz";
        }

        TEST_METHOD_INITIALIZE(ResetFlagDataMap)
        {
            cla::parse(argc, argv, cla::opt_t::WINDOWS);
        }

		TEST_METHOD(QueryingForExistingFlagsYieldsTrue)
		{
            Assert::IsTrue(cla::has("cla-test.exe"), L"cla-test.exe not found");
            Assert::IsTrue(cla::has("?"), L"? not found");
            Assert::IsTrue(cla::has("std"), L"std not found");
            Assert::IsTrue(cla::has("Lib"), L"Lib not found");
            Assert::IsTrue(cla::has("W3"), L"W3 not found");
            Assert::IsTrue(cla::has("Wd"), L"Wd not found");
		}

        TEST_METHOD(FlagsWithNoDataReturnsEmptyList)
        {
            Assert::IsTrue(cla::get("cla-test.exe")->empty(), L"cla-test.exe shouldn't have data");
            Assert::IsTrue(cla::get("?")->empty(), L"? shouldn't have data");
            Assert::IsTrue(cla::get("std")->empty(), L"std shouldn't have data");
            Assert::IsTrue(cla::get("W3")->empty(), L"W3 shouldn't have data");
        }
	};
}
