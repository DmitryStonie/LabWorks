#include "pch.h"
#include "../LifeGame/GameField.h"

using namespace gamefield;
namespace gamefield {
	class GameFieldTest : public::testing::Test {
	};

	TEST(TestGameField, run) {
		GameField test("CalculateSize1.txt");
		EXPECT_EQ(test.width, 14);
		EXPECT_EQ(test.height, 4);

	}
}