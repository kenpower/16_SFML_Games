#pragma once
// PipeGrid: the rules of Netwalk, with no SFML.
//
// This is where your extracted class goes in week 2. It is already included by
// Tests/netwalk_test.cpp, so it compiles in the Tests program from day 1.
//
// Rules for this file:
//   - no #include <SFML/...> (directly or through another header)
//   - no global variables, no rand(), no clock, no keyboard or mouse
//   - everything stays inside namespace netwalk_game
// You may rename the class (that is a refactoring too) - update the test file if you do.

#include <string>

namespace netwalk_game
{
    class PipeGrid
    {
    public:
        // A placeholder so the class has something to test. Replace it with real rules.
        std::string name() const { return "Netwalk"; }
    };
}
