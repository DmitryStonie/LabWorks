#include "pch.h"
#include "../LifeGame/GameField.h"
#include "../LifeGame/Console.h"

TEST(TestSuite, poswidth) {
	GameField map("tests/poswidth.txt");
  ASSERT_EQ(map.width, 1235);
  ASSERT_EQ(map.height, 22);
}

TEST(TestSuite, negwidth) {
	GameField map("tests/negwidth.txt");
	ASSERT_EQ(map.width, 124);
	ASSERT_EQ(map.height, 13);
}

TEST(TestSuite, dump_file) {
	GameField map("tests/dump_file_in.txt");
	map.field[map.height - 1][map.width - 1] = ALIVE_CELL;
	map.dump("tests/dump_file_out.txt");
	GameField tmp("tests/dump_file_out.txt");
	ASSERT_EQ(map.return_map(), tmp.return_map());
}

TEST(TestSuite, update_cells) {
	GameField map("tests/update_cells_in.txt");
	GameField tmp("tests/update_cells_out.txt");
	map.iterate(1, SILENCE);
	map.field[map.height - 1][map.width - 1] = ALIVE_CELL;
	ASSERT_EQ(map.return_map(), tmp.return_map());
}

TEST(TestSuite, toroidal) {
	GameField map("tests/toroidal.txt");
	GameField tmp = map;
	tmp.field[map.height - 1][map.width - 1] = DEAD_CELL;
	map.iterate(44, SILENCE);
	ASSERT_EQ(map.return_map(), tmp.return_map());
}

