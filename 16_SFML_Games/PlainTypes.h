#pragma once
// Small plain types for game rules, with no SFML.
//
// Use these in your extracted class instead of sf::Vector2i, sf::Vector2f and sf::FloatRect,
// so the class compiles in the Tests project (which does not link SFML).
// The game file can convert when it draws, e.g. sprite.setPosition({pos.x, pos.y}).
//
// They are in namespace plain so they don't clash with types of the same name in the games.
// Use them as plain::Point, or add `using plain::Rect;` inside your own namespace.

namespace plain
{
    // A cell on a grid, or a whole-number position.
    struct Point
    {
        int x = 0;
        int y = 0;
    };

    inline bool operator==(Point a, Point b) { return a.x == b.x && a.y == b.y; }
    inline bool operator!=(Point a, Point b) { return !(a == b); }
    inline Point operator+(Point a, Point b) { return {a.x + b.x, a.y + b.y}; }

    // A position or velocity in pixels.
    struct Vec2f
    {
        float x = 0;
        float y = 0;
    };

    inline Vec2f operator+(Vec2f a, Vec2f b) { return {a.x + b.x, a.y + b.y}; }

    // An axis-aligned rectangle: top-left corner (x, y), width w, height h.
    struct Rect
    {
        float x = 0;
        float y = 0;
        float w = 0;
        float h = 0;
    };

    // True when the rectangles share some area.
    // Rectangles that only touch along an edge do not overlap, the same as sf::FloatRect::findIntersection.
    inline bool overlaps(Rect a, Rect b)
    {
        return a.x < b.x + b.w && b.x < a.x + a.w
            && a.y < b.y + b.h && b.y < a.y + a.h;
    }
}
