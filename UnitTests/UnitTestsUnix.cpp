#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../clap.hpp"

namespace UnitTests
{
using namespace std::string_view_literals;

constexpr int argc_unix = 6;
char const* argv_unix[argc_unix];

TEST_CLASS(UnitTestsUnix)
{
public:

    TEST_CLASS_INITIALIZE(InitArgv)
    {
        argv_unix[0] = "cla-test";
        argv_unix[1] = "-h";
        argv_unix[2] = "--std=c++17";
        argv_unix[3] = "--lib_include="
            "/usr/lib/include/usrlib.a,"
            "/etc/lib/libmain.so,"
            "~/include/lib/mylib.so";
        argv_unix[4] = "--Wall";
        argv_unix[5] = "--Wdisable="
            "unused-var,"
            "switch-fallthrough,"
            "sign-cmp-mismatch";
    }

    TEST_METHOD_INITIALIZE(ResetFlagDataMap)
    {
        cla::parse(argc_unix, argv_unix, cla::opt_t::UNIX);
    }

    TEST_METHOD(ExistingFlagsAreFound)
    {
        Assert::IsTrue(cla::has_flag("cla-test"), L"cla-test not found");
        Assert::IsTrue(cla::has_flag("h"), L"h not found");
        Assert::IsTrue(cla::has_flag("std"), L"std not found");
        Assert::IsTrue(cla::has_flag("lib_include"), L"lib_include not found");
        Assert::IsTrue(cla::has_flag("Wall"), L"Wall not found");
        Assert::IsTrue(cla::has_flag("Wdisable"), L"Wdisable not found");
    }

    TEST_METHOD(DataFlagsHaveData)
    {
        Assert::IsTrue(cla::has_data("std"), L"std doesn't have data");
        Assert::IsTrue(cla::has_data("lib_include"), L"lib_include doesn't have data");
        Assert::IsTrue(cla::has_data("Wdisable"), L"Wdisable doesn't have data");
    }

    TEST_METHOD(HasDataReturnsFalseWithFlagsWithoutData)
    {
        Assert::IsFalse(cla::has_data("cla-test"), L"cla-test shouldn't have data");
        Assert::IsFalse(cla::has_data("h"), L"h shouldn't have data");
        Assert::IsFalse(cla::has_data("Wall"), L"Wall shouldn't have data");
    }

    TEST_METHOD(StdDataCheck)
    {
        auto std = "c++17"sv;

        auto const& vec = *cla::get("std");
        Assert::IsTrue(vec.size() == 1, L"std should have only 1 element");
        Assert::IsTrue(vec[0] == std, L"std should be c++17");
    }
};
}
