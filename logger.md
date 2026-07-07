# Pong Arena — Daily Execution Log

This log tracks daily execution milestones, debugging sessions, structural issues, and C++ architectural patterns mastered.

---

## 📅 Day 1: July 3, 2026
*Target: Paddle Entity, Independent 2-Player Controls, & Clamping*

### ✅ Tasks Done
1. **Split-Compilation Architecture**: Configured C++ workspace with `entities/paddle.h` (declarations) and `entities/paddle.cpp` (definitions), linking them via `CMakeLists.txt`.
2. **Independent Controls (Option B)**: Bound `upKey` and `downKey` as member variables in the `Paddle` struct to let Player 1 (`W`/`S`) and Player 2 (`UP`/`DOWN` arrows) move independently.
3. **C++ Constructor Implementation**: Created a custom parameter constructor for the `Paddle` struct to eliminate 16 lines of boilerplate initialization in `main.cpp`.
4. **Framerate Independence**: Integrated Delta Time (`GetFrameTime()`) into movement calculations.
5. **Boundary Clamping**: Implemented bounds checking to clamp `y` coordinates between `0` (top) and `HEIGHT - height` (bottom).
6. **Polished Rendering**: Styled drawing with `DrawRectangleRounded` using dynamic color and coordinate structures.
7. **Court Details**: Added center-line division and title text ("MARINO" / "SILICO").

### 🔍 Where I Stuck / Issues Faced
1. **Single-File Compiler Fallback**: Ran into a compiler error because CLion was executing a raw `g++.exe` command on `main.cpp` instead of using the CMake `PongArena` target which links Raylib.
   * *Resolution*: Switched the target run configuration dropdown in CLion to `PongArena`.
2. **CMake Policy Mismatch**: Hit a compatibility error with Raylib 5.0's script on newer CMake versions (`Compatibility with CMake < 3.5 has been removed`).
   * *Resolution*: Set `CMAKE_POLICY_VERSION_MINIMUM 3.5` in the main `CMakeLists.txt` to bypass the policy check on external dependencies.
3. **Variable Mismatch in `.cpp`**: Got compiler errors in `paddle.cpp` because placeholder variables from syntax examples (`x_val`, `round`, `segments`) were written instead of using the actual struct members and literal values.
   * *Resolution*: Bound members (`x`, `y`, `width`, `height`) to the Raylib `Rectangle` struct, and passed literal floats/ints (`0.4f`, `8`) into `DrawRectangleRounded`.
4. **Symbol Mismatch (Cannot Resolve `downKey`)**: Hit a linting error because of a mixed-up control architecture. Tried to assign keys in the constructor, but forgot to declare `upKey` and `downKey` as members in `paddle.h`.
   * *Resolution*: Fully declared the key fields inside the struct.

### 💀 Mistakes Made
1. **Global Variable Instantiation**: Declared `struct Rect { ... } Rect;` in the header, which instantiates a global variable named `Rect` and causes duplicate definition linker errors when included in multiple files.
2. **Uninitialized Speed/Color**: Forgot to set `speed` in `main.cpp`, leaving the memory address to hold random garbage data.
3. **Drawing Sequence Bug**: Called `ClearBackground` *after* drawing the paddle, wiping the paddle off the screen before it could render.
4. **Hardcoded Aesthetics**: Used `BLACK` directly inside `paddle.cpp`'s drawing call instead of utilizing the dynamic `color` variable belonging to the instance.

### 🧠 Concepts Learned
* **Difference between Declaration and Definition**: Declaring *what* exists in `paddle.h` vs. defining *how* it behaves in `paddle.cpp`.
* **CMake Target Linking**: Understanding why raw files compile single-file and fail without target linkage configurations linking external deps (like Raylib).
* **Boilerplate Reduction via Constructors**: Cleaning up object initialization in `main.cpp` using constructors instead of repetitive member assignments.
* **Delta Time & GetFrameTime()**: The core engine math of multiplying movement vectors by elapsed frame time to decouple physics speed from CPU performance.
* **Header Hygiene**: Using header guards (`#ifndef` / `#define` / `#pragma once`) and managing include directory paths relative to nested targets.
* **Global Struct Instantiation Pitfall**: The bug of instantiating variables directly after class/struct braces in C++ headers, causing linker conflicts.
* **Object Ownership**: Structuring object configurations (like movement keys) inside their respective class layouts rather than managing them globally.

### ⭐ Questions to Research
* Why are include guards or `#pragma once` strictly necessary in C++ compared to newer languages?
* Why does compiling/defining non-const global variables in headers throw a duplicate symbol/multiple-definition error at the link stage?
* Why is thread-blocking forbidden in real-time callbacks, and what alternatives (like rings/queues) are used to send trigger cues from main thread to audio thread?

## 📅 Day 2: July 4, 2026
*Target: Ball Entity, Constructor Initialization List, & Wall Bouncing*

### ✅ Tasks Done
1. **Split-Compilation for Ball**: Implemented `entities/ball.h` and `entities/ball.cpp` and linked them inside the `PongArena` target.
2. **Sub-Pixel Precision Physics**: Converted `Cx` and `Cy` coordinate variables from `int` to `float` to enable continuous floating-point physics calculations and prevent integer truncation.
3. **C++ Initializer List Constructor**: Implemented the C++ member initializer list syntax (`: Cx(Cx), Cy(Cy)... {}`) for the `Ball` struct, cleaning up `main.cpp` instantiation to a single line.
4. **Collision Bouncing**: Coded collision checks for all 4 viewport boundaries to invert velocities (`speedX` and `speedY`).
5. **Positional Correction**: Applied boundary adjustments (e.g., `Cy = radius` at top, `Cy = HEIGHT - radius` at bottom) to push the ball out of collision zones upon hit.
6. **Paddle Collisions**: Wired up `CheckCollisionCircleRec` for both paddles inside `main.cpp`'s game loop, correcting X-positions upon hit (`paddle1.x + paddle1.width + ball.radius` and `paddle2.x - ball.radius`) to prevent sticky frames.
7. **Scoring Resets**: Commented out left/right wall bounces in `ball.cpp` and implemented active off-screen resets in `main.cpp` to reset the ball position to screen center when missed.

### 🔍 Where I Stuck / Issues Faced
1. **Integer Physics deadlocks**: Figured out why using `int` for positions caused physics stuttering and deadlocks at high FPS due to float decimals dropping below `1.0f` and truncating to `0`.
2. **Sticky Collision Jitter Loop**: Debugged the issue where the ball gets trapped on a boundary, vibrating back and forth because it doesn't escape the collision box in a single frame.
   * *Resolution*: Applied positional offsets immediately upon collision detection.
3. **Loop Lifecycle Mismatches**: Realized that declaring collision shape trackers (`Vector2 center`, etc.) outside the main `while` loop prevents them from updating as the entities move. Moved them inside the loop.

### 💀 Mistakes Made
1. **Double Inversion**: Initially inverted both `speedX` and `speedY` on paddle collisions, which caused unnatural vertical bounces. Resolved by only inverting `speedX`.
2. **Left Paddle Offset Math**: Initially calculated `paddle1.x - paddle1.width - ball.radius` for the left paddle correction, which pushed the ball off-screen. Resolved to `+` additions.

### 🧠 Concepts Learned
* **Member Initializer Lists**: Mastered the syntax `: member(param)` and understood how it directly constructs members, bypassing the double-overhead of default constructor + assignment.
* **Double-Bounce Physics (Sticky Boundaries)**: Learned how discrete collision checks create sticky boundaries if position isn't corrected or direction isn't checked.
* **C++ Casting Mechanics**: Used C-style casts `(int)` to bridge floating-point physics coordinates with integer-bound drawing APIs.

### ⭐ Questions to Research
* What is the difference between C-style casting `(int)var` and C++ style casting `static_cast<int>(var)`? Which one is safer and why?
* How does the compiler treat Member Initializer Lists under the hood in terms of construction order (does declaration order in the header matter)?
* How do you implement circle-to-rectangle collision response (necessary for paddle hits)?

## 📅 Day 3: July 5-6, 2026
*Target: HP Bar Rendering, Win/Loss States, Fade Overlay, & Replay Scaling*

### ✅ Tasks Done
1. **Header Initializer Refactor**: Refactored the `Paddle` constructor in `paddle.h` to use a C++ Member Initializer List for `hp` and `maxHp`, solving variable shadowing.
2. **Dynamic HP Bars**: Rendered twin, rounded HUD health bars (`DrawRectangleRounded`) scaling dynamically via `paddle.hp / paddle.maxHp`.
3. **Sequential Damage Logic**: Placed HP subtraction checks inside the coordinate-miss blocks, ensuring damage is applied *before* the ball position is reset to the center.
4. **Correct Side Damage Mapping**: Fixed boundary checks so left-side misses (`Cx < 0`) correctly damage the left player (`paddle1`) and right-side misses damage `paddle2`.
5. **Aesthetic Dimming Overlay**: Implemented a semi-transparent screen overlay using `Fade(BLACK, 0.6f)` to dim the gameplay backdrop on win states.
6. **Replay Loop & Speed Escalation**: Programmed a match restart check (`IsKeyPressed(KEY_R)`) that resets player HP and multiplies subsequent ball velocities using a cumulative `multiplier` factor to increase difficulty across matches.

### 🔍 Where I Stuck / Issues Faced
1. **Linker main Overlap**: Hit a build failure (`multiple definition of main`) because CLion auto-added the `DSP/test.cpp` scratch file to the main `add_executable` target.
   * *Resolution*: Removed `DSP/` source files from the main target in `CMakeLists.txt`.
2. **Clangd Index Desync**: Encountered ghost red squiggles on standard Raylib functions (`EndDrawing()`, etc.) even though the code compiled and ran fine.
   * *Resolution*: Reset the CMake cache in CLion and restarted the Clangd Language Engine.
3. **Ghost Health Bar (Zero Width)**: The health bar disappeared when using the percentage equation.
   * *Resolution*: Realized `hp` and `maxHp` were uninitialized garbage floats in the constructor. Initializing them via member initializer lists resolved the math.

### 💀 Mistakes Made
1. **Mutated Check Sequence**: Checked ball borders and reset coordinates *before* evaluating HP reduction, making the HP reduction checks unreachable.
2. **Reversed Damage Targets**: Set the left miss to damage the right paddle, and vice versa.
3. **Missing Semicolon/Parenthesis cascade**: An unfinished `DrawRectangle` argument list caused the parser to throw syntax errors on completely unrelated lines below it.

### 🧠 Concepts Learned
* **Clangd Language Server Mechanics**: Understanding why real-time IDE diagnostics fall out of sync with compilers and how to reset the index cache.
* **Discrete Collision State Transitions**: Structuring checks so state mutation (coordinate resets) doesn't shadow subsequent state evaluation (HP deductions).
* **Faded Layering**: Using alpha-blended transparency overlays (`Fade`) to draw focus to UI alerts without modifying game state cameras or shaders.

### ⭐ Questions to Research
* What is the performance cost of calling `DrawRectangleRounded` multiple times per frame on the GPU?
* How does Raylib's double-buffering (`EndDrawing`) swap system work behind the scenes?

## 📅 Day 4: July 7, 2026
*Target: Architectural Refactoring & Physics Engine Modularization*

### ✅ Tasks Done
1. **Window Branding**: Renamed the game window title to the official name `"vStrike"` in `main.cpp`.
2. **Physics Module Isolation**: Created the `physics/` directory to host physical interactions and scoring rules.
3. **Collision Extrication**: Extracted all circle-to-rectangle paddle collision math and anti-sticking offset corrections into `ResolveCollision()` inside `physics/physicsEngine.cpp`.
4. **Scoring Logic Isolation**: Moved screen boundary exit detections, player HP reductions, and default ball resets into `CheckScoreAndReset()` in the new physics module.
5. **CMake Registration**: Linked the new `physicsEngine` headers and source files in `CMakeLists.txt`.
6. **Main Loop Decoupling**: Reduced the size of `main()` in `main.cpp` by replacing raw blocks with clean, expressive function calls.

### 🔍 Where I Stuck / Issues Faced
1. **Relative Directory Paths**: Had to ensure the relative header search paths (`../entities/` and `../common/`) correctly navigated the new folder structure.

### 💀 Mistakes Made
*None during this sprint. The refactoring compiled cleanly on the first try.*

### 🧠 Concepts Learned
* **Separation of Concerns (SoC)**: Keeping rendering loop controllers (`main.cpp`) focused on draw orchestration while delegate engines (`physicsEngine.cpp`) govern simulation math.
* **Pass-by-Reference Modifiers**: Reinforcing usage of `&` parameters in modular functions to prevent object duplication and ensure mutations apply to the live game state.

