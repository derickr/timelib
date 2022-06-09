#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include <cstring>

TEST_GROUP(dow)
{
	timelib_sll iy;
	timelib_sll iw;
	timelib_sll id;

	void test_ymd(timelib_sll y, timelib_sll m, timelib_sll d)
	{
		timelib_isodate_from_date(y, m, d, &iy, &iw, &id);
	}

	void check_ywd(timelib_sll e_iy, timelib_sll e_iw, timelib_sll e_id)
	{
		LONGS_EQUAL(iy, e_iy);
		LONGS_EQUAL(iw, e_iw);
		LONGS_EQUAL(id, e_id);
	}
};

TEST(dow, t_n0101_12_29) { test_ymd(-101, 12, 29); check_ywd(-101, 52, 5); }
TEST(dow, t_n0101_12_30) { test_ymd(-101, 12, 30); check_ywd(-101, 52, 6); }
TEST(dow, t_n0101_12_31) { test_ymd(-101, 12, 31); check_ywd(-101, 52, 7); }
TEST(dow, t_n0100_01_01) { test_ymd(-100,  1,  1); check_ywd(-100,  1, 1); }
TEST(dow, t_n0100_01_02) { test_ymd(-100,  1,  2); check_ywd(-100,  1, 2); }
TEST(dow, t_n0100_01_03) { test_ymd(-100,  1,  3); check_ywd(-100,  1, 3); }
TEST(dow, t_n0100_01_04) { test_ymd(-100,  1,  4); check_ywd(-100,  1, 4); }
TEST(dow, t_n0100_01_05) { test_ymd(-100,  1,  5); check_ywd(-100,  1, 5); }
TEST(dow, t_n0100_01_06) { test_ymd(-100,  1,  6); check_ywd(-100,  1, 6); }
TEST(dow, t_n0100_01_07) { test_ymd(-100,  1,  7); check_ywd(-100,  1, 7); }
TEST(dow, t_n0100_01_08) { test_ymd(-100,  1,  8); check_ywd(-100,  2, 1); }
TEST(dow, t_n0100_01_09) { test_ymd(-100,  1,  9); check_ywd(-100,  2, 2); }

TEST(dow, t_n0100_12_29) { test_ymd(-100, 12, 29); check_ywd(-100, 52, 6); }
TEST(dow, t_n0100_12_30) { test_ymd(-100, 12, 30); check_ywd(-100, 52, 7); }
TEST(dow, t_n0100_12_31) { test_ymd(-100, 12, 31); check_ywd( -99,  1, 1); }
TEST(dow, t_n0099_01_01) { test_ymd( -99,  1,  1); check_ywd( -99,  1, 2); }
TEST(dow, t_n0099_01_02) { test_ymd( -99,  1,  2); check_ywd( -99,  1, 3); }
TEST(dow, t_n0099_01_03) { test_ymd( -99,  1,  3); check_ywd( -99,  1, 4); }
TEST(dow, t_n0099_01_04) { test_ymd( -99,  1,  4); check_ywd( -99,  1, 5); }
TEST(dow, t_n0099_01_05) { test_ymd( -99,  1,  5); check_ywd( -99,  1, 6); }
TEST(dow, t_n0099_01_06) { test_ymd( -99,  1,  6); check_ywd( -99,  1, 7); }
TEST(dow, t_n0099_01_07) { test_ymd( -99,  1,  7); check_ywd( -99,  2, 1); }
TEST(dow, t_n0099_01_08) { test_ymd( -99,  1,  8); check_ywd( -99,  2, 2); }
TEST(dow, t_n0099_01_09) { test_ymd( -99,  1,  9); check_ywd( -99,  2, 3); }

TEST(dow, t_n0001_12_26) { test_ymd(-1, 12, 26); check_ywd(-1, 51, 7); }
TEST(dow, t_n0001_12_27) { test_ymd(-1, 12, 27); check_ywd(-1, 52, 1); }
TEST(dow, t_n0001_12_28) { test_ymd(-1, 12, 28); check_ywd(-1, 52, 2); }
TEST(dow, t_n0001_12_29) { test_ymd(-1, 12, 29); check_ywd(-1, 52, 3); }
TEST(dow, t_n0001_12_30) { test_ymd(-1, 12, 30); check_ywd(-1, 52, 4); }
TEST(dow, t_n0001_12_31) { test_ymd(-1, 12, 31); check_ywd(-1, 52, 5); }
TEST(dow,  t_0000_01_01) { test_ymd( 0,  1,  1); check_ywd(-1, 52, 6); }
TEST(dow,  t_0000_01_02) { test_ymd( 0,  1,  2); check_ywd(-1, 52, 7); }
TEST(dow,  t_0000_01_03) { test_ymd( 0,  1,  3); check_ywd( 0,  1, 1); }
TEST(dow,  t_0000_01_04) { test_ymd( 0,  1,  4); check_ywd( 0,  1, 2); }
TEST(dow,  t_0000_01_05) { test_ymd( 0,  1,  5); check_ywd( 0,  1, 3); }
TEST(dow,  t_0000_01_06) { test_ymd( 0,  1,  6); check_ywd( 0,  1, 4); }
TEST(dow,  t_0000_01_07) { test_ymd( 0,  1,  7); check_ywd( 0,  1, 5); }
TEST(dow,  t_0000_01_08) { test_ymd( 0,  1,  8); check_ywd( 0,  1, 6); }
TEST(dow,  t_0000_01_09) { test_ymd( 0,  1,  9); check_ywd( 0,  1, 7); }
TEST(dow,  t_0000_01_10) { test_ymd( 0,  1, 10); check_ywd( 0,  2, 1); }
TEST(dow,  t_0000_01_11) { test_ymd( 0,  1, 11); check_ywd( 0,  2, 2); }
TEST(dow,  t_0000_01_12) { test_ymd( 0,  1, 12); check_ywd( 0,  2, 3); }
TEST(dow,  t_0000_01_13) { test_ymd( 0,  1, 13); check_ywd( 0,  2, 4); }
