#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../clap.hpp"

namespace UnitTests
{
using namespace std::string_view_literals;

constexpr int argc_win32 = 6;
char const* argv_win32[argc_win32];

TEST_CLASS(UnitTestsWin32)
{
public:

    TEST_CLASS_INITIALIZE(InitArgv)
    {
        argv_win32[0] = "cla-test.exe";
        argv_win32[1] = "/?";
        argv_win32[2] = "/std:c++17";
        argv_win32[3] = "/Lib:"
            "C:\\Windows\\System32\\kernel.dll,"
            "C:\\Users\\uname\\dev\\include\\mylib.lib,"
            "C:\\include\\global\\somedll.dll";
        argv_win32[4] = "/W3";
        argv_win32[5] = "/Wd:123,456,789,abc,def,ghi,lmn,opq,rst,uvz";
    }

    TEST_METHOD_INITIALIZE(ResetFlagDataMap)
    {
        clap::parse(argc_win32, argv_win32, clap::style::WINDOWS);
    }

    TEST_METHOD(QueryingForExistingFlagsYieldsTrue)
    {
        Assert::IsTrue(clap::has_flag("cla-test.exe"), L"cla-test.exe not found");
        Assert::IsTrue(clap::has_flag("?"), L"? not found");
        Assert::IsTrue(clap::has_flag("std"), L"std not found");
        Assert::IsTrue(clap::has_flag("Lib"), L"Lib not found");
        Assert::IsTrue(clap::has_flag("W3"), L"W3 not found");
        Assert::IsTrue(clap::has_flag("Wd"), L"Wd not found");
    }

    TEST_METHOD(FlagsWithNoDataReturnsEmptyList)
    {
        Assert::IsFalse(clap::has_data("cla-test.exe"), L"cla-test.exe shouldn't have data");
        Assert::IsFalse(clap::has_data("?"), L"? shouldn't have data");
        Assert::IsFalse(clap::has_data("W3"), L"W3 shouldn't have data");
    }

    TEST_METHOD(FlagsWithDataReturnsNonEmptyList)
    {
        Assert::IsTrue(clap::has_data("Lib"), L"? should have data");
        Assert::IsTrue(clap::has_data("Wd"), L"? should have data");
    }

    TEST_METHOD(StdShouldHaveCPP17)
    {
        auto std = "c++17"sv;
        auto const& vec = *clap::get("std");
        Assert::IsTrue(vec.size() == 1, L"std should have 1 element");
        Assert::IsTrue(vec[0] == std, L"std should be c++17");
    }

    TEST_METHOD(QueryingLibFlagsYieldsExpectedResults)
    {
        auto lib_0 = "C:\\Windows\\System32\\kernel.dll"sv;
        auto lib_1 = "C:\\Users\\uname\\dev\\include\\mylib.lib"sv;
        auto lib_2 = "C:\\include\\global\\somedll.dll"sv;

        auto const& vec = *clap::get("Lib");
        Assert::IsTrue(vec.size() == 3, L"Lib should have 3 libraries");
        Assert::IsTrue(vec[0] == lib_0, L"stored lib should be equal to lib_0");
        Assert::IsTrue(vec[1] == lib_1, L"stored lib should be equal to lib_1");
        Assert::IsTrue(vec[2] == lib_2, L"stored lib should be equal to lib_2");
    }

    TEST_METHOD(WdFlagDataPresentAndCorrect)
    {
        auto wd_123 = "123"sv;
        auto wd_456 = "456"sv;
        auto wd_789 = "789"sv;
        auto wd_abc = "abc"sv;
        auto wd_def = "def"sv;
        auto wd_ghi = "ghi"sv;
        auto wd_lmn = "lmn"sv;
        auto wd_opq = "opq"sv;
        auto wd_rst = "rst"sv;
        auto wd_uvz = "uvz"sv;

        auto const& vec = *clap::get("Wd");
        Assert::IsTrue(vec.size() == 10, L"Wd should have 10 elements");
        Assert::IsTrue(vec[0] == wd_123, L"vec[0] should be 123");
        Assert::IsTrue(vec[1] == wd_456, L"vec[1] should be 456");
        Assert::IsTrue(vec[2] == wd_789, L"vec[2] should be 789");
        Assert::IsTrue(vec[3] == wd_abc, L"vec[3] should be abc");
        Assert::IsTrue(vec[4] == wd_def, L"vec[4] should be def");
        Assert::IsTrue(vec[5] == wd_ghi, L"vec[5] should be ghi");
        Assert::IsTrue(vec[6] == wd_lmn, L"vec[6] should be lmn");
        Assert::IsTrue(vec[7] == wd_opq, L"vec[7] should be opq");
        Assert::IsTrue(vec[8] == wd_rst, L"vec[8] should be rst");
        Assert::IsTrue(vec[9] == wd_uvz, L"vec[9] should be uvz");
    }

    TEST_METHOD(HasFlagReturnsFalseForNotStoredFlag)
    {
        Assert::IsFalse(clap::has_flag("Hello!"), L"Hello! flag shouldn't exist");
        Assert::IsFalse(clap::has_flag(""), L"Empty flag shouldn't exist");
        Assert::IsFalse(clap::has_flag("\0"), L"Null char flag shouldn't exist");
        Assert::IsFalse(clap::has_flag("Std"), L"Std flag shouldn't exist");
    }
};
}
