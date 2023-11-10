#include "pch.h"

const int HEIGHT = 25;
const int WIDTH = 40;
const int tileSize = 18;

#include"../16_SFML_Games/Grid.h"

TEST(Grid, HasWallsAndInterior) {

	Grid grid;

	EXPECT_EQ(Grid::tile::WALL, grid.cell(0, 0));
	EXPECT_EQ(Grid::tile::WALL, grid.cell(24, 39));
	EXPECT_EQ(Grid::tile::EMPTY, grid.cell(10, 10));

}

TEST(Grid, CreateNewWall) {

	Grid grid;

	grid.newWall(10, 10);
	EXPECT_EQ(Grid::tile::NEW_WALL, grid.cell(10, 10));

}
