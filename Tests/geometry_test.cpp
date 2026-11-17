// Unit tests for Geometry.h: points, vectors and rectangles with no SFML.
// They also show how to test small value types: build them, combine them, compare the result.

#include <gtest/gtest.h>
#include "Geometry.h"

using geom::Point;
using geom::Rect;
using geom::Vec2f;
using geom::overlaps;

// ---- Point ----

TEST(GeometryPoint, StartsAtZero)
{
    Point p;

    EXPECT_EQ(0, p.x);
    EXPECT_EQ(0, p.y);
}

TEST(GeometryPoint, PointsWithSameCoordinatesAreEqual)
{
    EXPECT_TRUE((Point{3, 4} == Point{3, 4}));
    EXPECT_FALSE((Point{3, 4} != Point{3, 4}));
}

TEST(GeometryPoint, PointsDifferingInOneCoordinateAreNotEqual)
{
    EXPECT_FALSE((Point{3, 4} == Point{3, 5}));
    EXPECT_TRUE((Point{3, 4} != Point{2, 4}));
}

TEST(GeometryPoint, AddingPointsAddsEachCoordinate)
{
    Point moved = Point{3, 4} + Point{1, -1};

    EXPECT_EQ(4, moved.x);
    EXPECT_EQ(3, moved.y);
}

// ---- Vec2f ----

TEST(GeometryVec2f, AddingVectorsAddsEachComponent)
{
    Vec2f sum = Vec2f{1.5f, 2.0f} + Vec2f{0.5f, -1.0f};

    EXPECT_FLOAT_EQ(2.0f, sum.x);
    EXPECT_FLOAT_EQ(1.0f, sum.y);
}

// ---- Rect and overlaps() ----

TEST(GeometryOverlaps, RectanglesSharingAreaOverlap)
{
    EXPECT_TRUE(overlaps(Rect{0, 0, 10, 10}, Rect{5, 5, 10, 10}));
}

TEST(GeometryOverlaps, RectangleInsideAnotherOverlaps)
{
    EXPECT_TRUE(overlaps(Rect{0, 0, 10, 10}, Rect{2, 2, 3, 3}));
}

TEST(GeometryOverlaps, OverlapDoesNotDependOnOrder)
{
    Rect a{0, 0, 10, 10};
    Rect b{5, 5, 10, 10};

    EXPECT_EQ(overlaps(a, b), overlaps(b, a));
}

TEST(GeometryOverlaps, RectanglesFarApartDoNotOverlap)
{
    EXPECT_FALSE(overlaps(Rect{0, 0, 10, 10}, Rect{50, 50, 10, 10}));
}

// Edge cases: rectangles that only touch share no area, so they don't overlap
// (the same as sf::FloatRect::findIntersection).

TEST(GeometryOverlaps, RectanglesTouchingSideBySideDoNotOverlap)
{
    EXPECT_FALSE(overlaps(Rect{0, 0, 10, 10}, Rect{10, 0, 10, 10}));
}

TEST(GeometryOverlaps, RectanglesTouchingTopToBottomDoNotOverlap)
{
    EXPECT_FALSE(overlaps(Rect{0, 0, 10, 10}, Rect{0, 10, 10, 10}));
}

TEST(GeometryOverlaps, RectanglesTouchingAtACornerDoNotOverlap)
{
    EXPECT_FALSE(overlaps(Rect{0, 0, 10, 10}, Rect{10, 10, 10, 10}));
}

TEST(GeometryOverlaps, RectanglesOverlappingByOnePixelOverlap)
{
    EXPECT_TRUE(overlaps(Rect{0, 0, 10, 10}, Rect{9, 0, 10, 10}));
}

TEST(GeometryOverlaps, RectanglesOverlappingSidewaysButNotVerticallyDoNotOverlap)
{
    EXPECT_FALSE(overlaps(Rect{0, 0, 10, 10}, Rect{5, 20, 10, 10}));
}
