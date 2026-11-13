#pragma once

// Board dimensions shared by the Xonix game and its testable classes.
// Kept in their own header so Grid.h and Player.h are self-contained:
// they compile wherever they are included, with no SFML and no globals
// that the includer has to define first.

constexpr int HEIGHT = 25;  // rows of tiles
constexpr int WIDTH = 40;   // columns of tiles
constexpr int tileSize = 18; // pixels per tile
