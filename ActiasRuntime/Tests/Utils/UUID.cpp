#include <ActiasRuntime/Strings/StringSlice.h>
#include <ActiasRuntime/Utils/UUID.h>
#include <gtest/gtest.h>

TEST(UUID, Parse)
{
    Actias::UUID uuid1("62e1b7a1-c14a-4129-ac57-7e77289123e9");
    Actias::UUID uuid2("62E1B7A1-C14A-4129-AC57-7E77289123E9");

#define ACTIAS_CHECK_UUID(uuid)\
        EXPECT_EQ(uuid.Data[0], 0x62);\
        EXPECT_EQ(uuid.Data[1], 0xe1);\
        EXPECT_EQ(uuid.Data[2], 0xb7);\
        EXPECT_EQ(uuid.Data[3], 0xa1);\
        EXPECT_EQ(uuid.Data[4], 0xc1);\
        EXPECT_EQ(uuid.Data[5], 0x4a);\
        EXPECT_EQ(uuid.Data[6], 0x41);\
        EXPECT_EQ(uuid.Data[7], 0x29);\
        EXPECT_EQ(uuid.Data[8], 0xac);\
        EXPECT_EQ(uuid.Data[9], 0x57);\
        EXPECT_EQ(uuid.Data[10], 0x7e);\
        EXPECT_EQ(uuid.Data[11], 0x77);\
        EXPECT_EQ(uuid.Data[12], 0x28);\
        EXPECT_EQ(uuid.Data[13], 0x91);\
        EXPECT_EQ(uuid.Data[14], 0x23);\
        EXPECT_EQ(uuid.Data[15], 0xe9);

    ACTIAS_CHECK_UUID(uuid1);
    ACTIAS_CHECK_UUID(uuid2);

    EXPECT_EQ(uuid1, uuid2);

    auto uuid3 = Actias::StringSlice("62E1B7A1-C14A-4129-AC57-7E77289123E9").Parse<Actias::UUID>();
    EXPECT_EQ(uuid1, uuid3.Unwrap());
    auto uuid4 = Actias::StringSlice("62e1b7a1-c14a-4129-ac57-7e77289123e9").Parse<Actias::UUID>();
    EXPECT_EQ(uuid1, uuid4.Unwrap());
}

TEST(UUID, TryParse)
{
    Actias::UUID result;
    EXPECT_TRUE(Actias::UUID::TryParse("62E1B7A1-C14A-4129-AC57-7E77289123E9", result));
    EXPECT_TRUE(Actias::UUID::TryParse("62e1b7a1-c14a-4129-ac57-7e77289123e9", result));
    EXPECT_TRUE(Actias::UUID::TryParse("62e1B7A1-C14A-4129-Ac57-7E77289123E9", result));

    EXPECT_FALSE(Actias::UUID::TryParse("62E1B7A1C14A-4129-AC57-7E77289123E9", result));
    EXPECT_FALSE(Actias::UUID::TryParse("62E1B7A1=C14A-4129-AC57-7E77289123E9", result));

    // incorrect length
    EXPECT_FALSE(Actias::UUID::TryParse("62E1B7A1-C14A-4129-AC57-7E77289123E", result));
    EXPECT_FALSE(Actias::UUID::TryParse("62E1B7A1-C14A-4129-AC57-7E77289123E99999999999", result));
    EXPECT_TRUE(Actias::UUID::TryParse("62E1B7A1-C14A-4129-AC57-7E77289123E99999999999", result, false));

    EXPECT_FALSE(Actias::UUID::TryParse("", result));
    EXPECT_FALSE(Actias::UUID::TryParse("something weird", result));
    EXPECT_FALSE(Actias::UUID::TryParse("something weird", result, false));

    // G - invalid character in different positions
    EXPECT_FALSE(Actias::UUID::TryParse("62E1B7A1-C14A-4129-AC57-7E77289123EG", result));
    EXPECT_FALSE(Actias::UUID::TryParse("G2E1B7A1-C14A-4129-AC57-7E77289123E9", result));
    EXPECT_FALSE(Actias::UUID::TryParse("6GE1B7A1-C14A-4129-AC57-7E77289123E9", result));
    EXPECT_FALSE(Actias::UUID::TryParse("62G1B7A1-C14A-4129-AC57-7E77289123E9", result));
    EXPECT_FALSE(Actias::UUID::TryParse("62EGB7A1-C14A-4129-AC57-7E77289123E9", result));
    EXPECT_FALSE(Actias::UUID::TryParse("62E1G7A1-C14A-4129-AC57-7E77289123E9", result));
}

TEST(UUID, ToString)
{
    EXPECT_EQ(Actias::UUID("62e1b7a1-c14a-4129-ac57-7e77289123e9").ToString(), "62E1B7A1-C14A-4129-AC57-7E77289123E9");
    EXPECT_EQ(Actias::UUID("62E1B7A1-C14A-4129-AC57-7E77289123E9").ToString(), "62E1B7A1-C14A-4129-AC57-7E77289123E9");
}
