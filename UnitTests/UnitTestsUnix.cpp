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
};
}
