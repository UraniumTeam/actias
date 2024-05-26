#include <Actias/Containers/HashSet.hpp>
#include <Actias/Strings/String.hpp>
#include <Actias/Time/DateTime.hpp>
#include <Tests/Common/Common.hpp>
#include <unordered_set>

using Actias::FixStr;
using Actias::HashSet;
using Actias::List;
using Actias::String;

TEST(HashSet, ConstructDestructObject)
{
    auto mock = std::make_shared<MockConstructors>();
    EXPECT_CALL(*mock, Construct()).Times(1);
    EXPECT_CALL(*mock, Destruct()).Times(1);
    EXPECT_CALL(*mock, Copy()).Times(0);
    EXPECT_CALL(*mock, Move()).Times(1);

    HashSet<MockObject> set;
    set.Add(MockObject{ mock });
}

TEST(HashSet, AddContains)
{
    HashSet<String> set;
    EXPECT_TRUE(set.Add("abcdef"));
    EXPECT_TRUE(set.Add("abc"));
    EXPECT_FALSE(set.Add("abc"));

    EXPECT_TRUE(set.Contains("abcdef"));
    EXPECT_TRUE(set.Contains("abc"));
    EXPECT_FALSE(set.Contains("abcd"));
}

TEST(HashSet, AddContainsWithCollisions)
{
    HashSet<String, 1> set;
    EXPECT_TRUE(set.Add("abcdef"));
    EXPECT_TRUE(set.Add("abc"));
    EXPECT_FALSE(set.Add("abc"));

    EXPECT_TRUE(set.Contains("abcdef"));
    EXPECT_TRUE(set.Contains("abc"));
    EXPECT_FALSE(set.Contains("abcd"));
}

TEST(HashSet, Iterate)
{
    HashSet<String> set;
    set.Add("1");
    set.Add("2");
    set.Add("2");
    set.Add("2");
    set.Add("3");
    set.Add("3");

    USize count = 0;
    for (const String& s : set)
    {
        ++count;
        EXPECT_TRUE(s == "1" || s == "2" || s == "3");
    }

    EXPECT_EQ(count, 3);
}

TEST(HashSet, IterateWithCollisions)
{
    HashSet<String, 2> set;
    set.Add("1");
    set.Add("2");
    set.Add("2");
    set.Add("2");
    set.Add("3");
    set.Add("3");

    USize count = 0;
    for (const String& s : set)
    {
        ++count;
        EXPECT_TRUE(s == "1" || s == "2" || s == "3");
    }

    EXPECT_EQ(count, 3);
}

TEST(HashSet, LargeTest)
{
    const auto time = Actias::UTCDateTime::Now();
    srand(static_cast<unsigned int>(time.UnixTime()));

    HashSet<Int32> set;
    std::unordered_set<Int32> stdSet;
    for (USize i = 0; i < 100000; ++i)
    {
        const Int32 val = rand();
        set.Add(val);
        stdSet.insert(val);
    }

    EXPECT_EQ(set.Size(), stdSet.size());
}
