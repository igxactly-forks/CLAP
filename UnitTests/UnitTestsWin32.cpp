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
            Assert::IsTrue(cla::has_flag("cla-test.exe"), L"cla-test.exe not found");
            Assert::IsTrue(cla::has_flag("?"), L"? not found");
            Assert::IsTrue(cla::has_flag("std"), L"std not found");
            Assert::IsTrue(cla::has_flag("Lib"), L"Lib not found");
            Assert::IsTrue(cla::has_flag("W3"), L"W3 not found");
            Assert::IsTrue(cla::has_flag("Wd"), L"Wd not found");
		}

        TEST_METHOD(FlagsWithNoDataReturnsEmptyList)
        {
            Assert::IsFalse(cla::has_data("cla-test.exe"), L"cla-test.exe shouldn't have data");
            Assert::IsFalse(cla::has_data("?"), L"? shouldn't have data");
            Assert::IsFalse(cla::has_data("std"), L"std shouldn't have data");
            Assert::IsFalse(cla::has_data("W3"), L"W3 shouldn't have data");
        }

        TEST_METHOD(FlagsWithDataReturnsNonEmptyList)
        {
            Assert::IsTrue(cla::has_data("Lib"), L"? should have data");
            Assert::IsTrue(cla::has_data("Wd"), L"? should have data");
        }
	};
}
