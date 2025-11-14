# SFML Games - Refactoring Exercise Guide


## How to Use This Resource

1. **Choose a game** from the list below
3. **Refactor the code** to improve quality without changing behavior

## Games by Difficulty Level

### 🟢 Beginner Level (Simple refactorings)

1. **snake_messy.cpp** - Perfect for beginners
   - Simple game loop
   - Clear duplication in drawing code
   - Good for extracting functions

2. **tetris_messy.cpp** - Classic block game
   - Nested conditionals to simplify
   - Magic numbers everywhere
   - Good practice for extracting constants

3. **doodle_jump_messy.cpp** - Platform jumping game
   - Arithmetic in conditionals
   - Dead code (unused variables)
   - Good for simplifying expressions

### 🟡 Intermediate Level (Multiple refactoring types)

4. **arkanoid_messy.cpp** - Breakout clone
   - Significant code duplication
   - Bad variable names (a, b, c, d)
   - Extract method opportunities

5. **tron_messy.cpp** - Light cycle game
   - Repeated control structures
   - Duplicate player input handling
   - Good for introducing helper functions

6. **minesweeper_messy.cpp** - Classic puzzle
   - Repeated mine counting logic
   - Nested loops to extract
   - Magic numbers for grid size

7. **fifteen_puzzle_messy.cpp** - Sliding tile puzzle
   - Adjacent tile logic duplication
   - Animation code to extract
   - Magic numbers for tile size

8. **bejeweled_messy.cpp** - Match-3 game
   - Complex match-finding logic
   - Duplicate horizontal/vertical checks
   - Multiple nested loops

### 🟠 Advanced Level (Complex refactorings)

9. **racing_messy.cpp** - Top-down racing
   - Complex car physics
   - Collision detection to extract
   - Movement and acceleration logic

10. **mahjong_messy.cpp** - Tile matching
    - 3D coordinate system
    - Complex tile matching rules
    - File I/O and map generation

11. **netwalk_messy.cpp** - Pipe puzzle
    - Recursive flood-fill algorithm
    - Puzzle generation logic
    - Pipe rotation mechanics

12. **outrun_messy.cpp** - 3D racing
    - Pseudo-3D projection math
    - Road rendering system
    - Sprite scaling and clipping

### 🔴 Expert Level (Largest refactorings)

13. **asteroids_messy.cpp** - Space shooter
    - Class hierarchy with Entity
    - Collision detection system
    - Dynamic entity management
    - Animation system

14. **chess_messy.cpp** - Chess with AI
    - Chess notation conversion
    - Move validation and castling
    - External engine integration
    - Drag-and-drop interaction

## Common Refactoring Opportunities

### 1. Extract Method
**Before:**
```cpp
// Do something complex inline
for (int i=0; i<10; i++) {
    // 20 lines of logic
}
```

**After:**
```cpp


void doSomethingComplex() {
// 20 lines of logic
}

for (int i=0; i<10; i++) {
        doSomethingComplex();
    }
```

### 2. Rename Variable
**Before:**
```cpp
int a = 6, b = 5;  // What are these?
float c = 300, d = 300;
```

**After:**
```cpp
int ballSpeedX = 6, ballSpeedY = 5;
float ballX = 300, ballY = 300;
```

### 3. Extract Constant
**Before:**
```cpp
if (x > 520) { /* bounds check */ }
if (y > 450) { /* another bounds check */ }
window.create(VideoMode(520, 450));
```

**After:**
```cpp
const int WINDOW_WIDTH = 520;
const int WINDOW_HEIGHT = 450;

if (x > WINDOW_WIDTH) { /* bounds check */ }
if (y > WINDOW_HEIGHT) { /* another bounds check */ }
window.create(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT));
```

### 4. Consolidate Duplicate Code
**Before:**
```cpp
// Check horizontal collision
for (int i=0; i<n; i++) {
    float ballLeft = c+3;
    float ballTop = d+3;
    FloatRect ball_rect = FloatRect({ballLeft, ballTop}, {6, 6});
    if (ball_rect.findIntersection(block[i].getGlobalBounds())) {
        block[i].setPosition({-100, 0});
        a=-a;
    }
}

// Check vertical collision
for (int i=0; i<n; i++) {
    float ballLeft = c+3;
    float ballTop = d+3;
    FloatRect ball_rect = FloatRect({ballLeft, ballTop}, {6, 6});
    if (ball_rect.findIntersection(block[i].getGlobalBounds())) {
        block[i].setPosition({-100, 0});
        b=-b;
    }
}
```

**After:**
```cpp
void checkBlockCollision(float& velocity) {
    for (int i=0; i<n; i++) {
        FloatRect ballRect = createBallRect();
        if (ballRect.findIntersection(block[i].getGlobalBounds())) {
            hideBlock(block[i]);
            velocity = -velocity;
        }
    }
}

checkBlockCollision(ballSpeedX);
checkBlockCollision(ballSpeedY);
```

### 5. Simplify Conditionals
**Before:**
```cpp
if (x<0) { dx=-dx; }
if (x>520) { dx=-dx; }
if (y<0) { dy=-dy; }
if (y>450) { dy=-dy; }
```

**After:**
```cpp
if (x < 0 || x > WINDOW_WIDTH) dx = -dx;
if (y < 0 || y > WINDOW_HEIGHT) dy = -dy;
```

### 6. Remove Dead Code
**Before:**
```cpp
int score = 0;  // declared but never used
bool gameOver = false;  // never checked
```

**After:**
```cpp
// Remove unused variables
```

### 7. Replace Magic Number with Constant
**Before:**
```cpp
for (int i=0; i<10; i++)
    for (int j=0; j<10; j++)
        grid[i][j] = 0;
```

**After:**
```cpp
const int GRID_SIZE = 10;

for (int i=0; i<GRID_SIZE; i++)
    for (int j=0; j<GRID_SIZE; j++)
        grid[i][j] = 0;
```

## Refactoring Checklist

When refactoring any game, look for these issues:

- [ ] **Bad variable names** (a, b, c, x1, y1, n, etc.)
- [ ] **Magic numbers** (hardcoded values without names)
- [ ] **Duplicated code** (same logic repeated multiple times)
- [ ] **Long functions** (>50 lines, doing multiple things)
- [ ] **Nested conditionals** (can be simplified or extracted)
- [ ] **Dead code** (unused variables, commented code)
- [ ] **Inconsistent style** (x=x+1 vs x+=1)
- [ ] **Poor separation** (rendering, logic, input all mixed)
- [ ] **Complex expressions** (arithmetic in conditionals)
- [ ] **Repeated patterns** (can be extracted to helper functions)


## Assessment Criteria

Rate your refactoring on these dimensions:

1. **Readability** (1-10): Can someone understand it quickly?
2. **Maintainability** (1-10): How easy is it to change?
3. **DRY** (1-10): How much duplication remains?
4. **Naming** (1-10): Are names clear and consistent?
5. **Structure** (1-10): Is code well-organized?

**Goal**: Achieve 8+ in all categories

## Tips for Success

1. **Read the code first** - Understand what it does before changing it
2. **One refactoring at a time** - Don't try to fix everything at once
3. **Test frequently** - Make sure it still works after each change
4. **Compare with clean version** - But only after you've tried!
5. **Keep notes** - Document what refactorings you applied
6. **Time yourself** - Track how long each refactoring takes
7. **Review your work** - Can you simplify further?

## Advanced Exercises

After completing the basic refactorings:

1. **Extract classes** - Create Ball, Paddle, Block classes for arkanoid
2. **Introduce design patterns** - Add Observer for game events
3. **Separate concerns** - Split rendering, logic, and input
4. **Add features** - Extend the games with new functionality
5. **Performance optimization** - Profile and optimize hot paths
6. **Unit testing** - Add tests for extracted functions

## Resources

- Martin Fowler's "Refactoring" book
- Clean Code by Robert Martin
- Refactoring.guru website
- Your IDE's refactoring tools

## Common Mistakes to Avoid

1. ❌ **Over-engineering** - Don't create 20 classes for a simple game
2. ❌ **Breaking behavior** - Test after every change
3. ❌ **Inconsistent naming** - Pick a convention and stick to it
4. ❌ **Premature optimization** - Focus on clarity first
5. ❌ **Skipping the clean version** - Use it as a learning tool

## Conclusion

These exercises provide hands-on practice with real code that has realistic problems. The games are fully functional, so you can focus purely on improving code quality without worrying about correctness.

Start with the easier games and work your way up. Don't be discouraged if the advanced games take several hours - that's expected!

Happy refactoring! 🛠️
