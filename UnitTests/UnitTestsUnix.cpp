#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../clap.hpp"

namespace UnitTests
{
using namespace std::string_view_literals;

constexpr int argc_unix = 7;
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
        argv_unix[6] = "-xvfcq";
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

    TEST_METHOD(LibIncludeDataCheck)
    {
        auto lib_0 = "/usr/lib/include/usrlib.a"sv;
        auto lib_1 = "/etc/lib/libmain.so"sv;
        auto lib_2 = "~/include/lib/mylib.so"sv;

        auto const& vec = *cla::get("lib_include");
        Assert::IsTrue(vec.size() == 3, L"lib_include should have 3 elements");
        Assert::IsTrue(vec[0] == lib_0, L"lib_include lib_0 mismatch");
        Assert::IsTrue(vec[1] == lib_1, L"lib_include lib_1 mismatch");
        Assert::IsTrue(vec[2] == lib_2, L"lib_include lib_2 mismatch");
    }

    TEST_METHOD(WdisableDataCheck)
    {
        auto wd_0 = "unused-var"sv;
        auto wd_1 = "switch-fallthrough"sv;
        auto wd_2 = "sign-cmp-mismatch"sv;

        auto const& vec = *cla::get("Wdisable");
        Assert::IsTrue(vec.size() == 3, L"Wdisable should have 3 elements");
        Assert::IsTrue(vec[0] == wd_0, L"Wdisable wd_0 mismatch");
        Assert::IsTrue(vec[1] == wd_1, L"Wdisable wd_1 mismatch");
        Assert::IsTrue(vec[2] == wd_2, L"Wdisable wd_2 mismatch");
    }

    TEST_METHOD(SingleHyphenMultiFlag)
    {
        // -xvfcq
        Assert::IsTrue(cla::has_flag("x"), L"x not found");
        Assert::IsTrue(cla::has_flag("v"), L"v not found");
        Assert::IsTrue(cla::has_flag("f"), L"f not found");
        Assert::IsTrue(cla::has_flag("c"), L"c not found");
        Assert::IsTrue(cla::has_flag("q"), L"q not found");

        Assert::IsFalse(cla::has_data("x"), L"x shouldn't have data");
        Assert::IsFalse(cla::has_data("v"), L"v shouldn't have data");
        Assert::IsFalse(cla::has_data("f"), L"f shouldn't have data");
        Assert::IsFalse(cla::has_data("c"), L"c shouldn't have data");
        Assert::IsFalse(cla::has_data("q"), L"q shouldn't have data");
    }

    TEST_METHOD(UnexistingFlagsShouldBeFound)
    {
        Assert::IsFalse(cla::has_flag("-"), L"- is not allowed as flag");
        Assert::IsFalse(cla::has_flag("--"), L"-- is not allowed as flag");
        Assert::IsFalse(cla::has_flag(""), L"<empty> is not allowed as flag");
        Assert::IsFalse(cla::has_flag("xvfcq"), L"xvfcq shouldn't exist");
        Assert::IsFalse(cla::has_flag("\0"), L"\0 shouldn't exist");
    }
};
}
