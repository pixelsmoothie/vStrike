# Pong Arena — Daily Execution Log

This log tracks daily execution milestones, debugging sessions, structural issues, and C++ architectural patterns mastered.

---

## Day 8: July 16, 2026
*Target: Gameplay OOP Screen Refactoring & Mode Selection*

### Tasks Done
1. **Base Gameplay Screen**: Created `core/gameView.h` as an abstract base class holding `paddle1`, `paddle2`, `ball`, and `multiplier` as `protected` members.
2. **Shared Rendering & Physics**: Implemented the common `Draw()` and `updatePhysics(dt)` methods inside `GameView` to eliminate all code duplication between game modes.
3. **Derived View Screens**: Created `LocalView` (`core/localView.h`) and `AIView` (`core/aiView.h`) inheriting from `GameView` to specialize keyboard vs static AI behaviors.
4. **Mode Selection Integration**: Wired mouse selection clicks on the selection menu to load either the local 1v1 gameplay or the AI court.
5. **Global Fade Transitions**: Implemented the `Fader` class (`UI/fader.h`) to handle smooth black screen fade-outs and fade-ins during state transitions.
6. **High-Res Font Loading**: Loaded `.otf` custom typography using `LoadFontEx` at a large 96px resolution, applying bilinear filtering to prevent blurry scaling.
7. **Pause State & Menu Overlay**: Programmed a keyboard space-toggle to freeze physics/movements while rendering a translucent gradient overlay with custom font buttons ("RESUME", "SETTINGS", "EXIT").

### Where I Stuck / Issues Faced
1. **Scope and Placement Constraints**: Resolved a compiler error where statements (`updatePhysics`) were incorrectly written outside member function bodies in `localView.h`.
2. **Pointer Type Syntax Warning**: Resolved a CLion dangling pointer warning by nullifying `currentScreen = nullptr` immediately after calling `delete` in the swapper.
3. **Copy-Paste Router Instantiation**: Resolved a bug where AI mode loaded the local court because of a copy-pasted class name (`new LocalView()`) in `main.cpp`'s switch block.
4. **Input Method Typo**: Resolved mouse click failures on the mode selection buttons by changing `IsKeyPressed` to `IsMouseButtonPressed`.
5. **Dangling Logical Else**: Found that omitting `else if` for scroll checks (leaving it as a raw `else`) evaluated stationary scroll wheels (`0.0f`) as a backward scroll, causing the selection state to flash at 60 FPS.
6. **Unreachable Code Warnings**: Discovered a logic error where the starting state in `Start()` was set to `STATE_OUT` instead of `STATE_IN`, making the entire fade-in update block dead code.
7. **Parser Failure with Hanging Operators**: Found that an incomplete `&&` operator on a conditional check broke code compilation and created ghost Clangd warnings on unrelated symbols.

### Mistakes Made
* **Copy-Paste Class Name**: Instantiated the wrong class under the `STATE_AI_VIEW` case block.
* **Dangling Else on Scroll Check**: Left scroll logic falling into the negative-wrap block when mouse movement was zero.
* **Mismatched Initial Transition State**: Initialized the fader to the outgoing state instead of the incoming state.

### Concepts Learned
* **C++ Protected Access Modifier**: Using `protected:` to let derived classes read/write member data of the parent class while keeping it hidden from the rest of the program.
* **Pure Virtual vs. Overridden Virtuals**: Understanding when to write a pure virtual function (`= 0`) to force derived class implementation vs. implementing a shared method directly in the base class to avoid duplication.
* **Dangling Pointer Safety**: Nullifying deleted pointers immediately to prevent them from pointing to deallocated memory blocks.
* **Modulo Carousel wrapping**: Using positive modulo offsets to easily rotate menu index loops.
* **Ternary Operator syntax**: Refreshing shorthand evaluations `(condition) ? trueVal : falseVal`.
* **Texture Resizing and Scaling**: Understanding why low-res rasterized fonts blur when scaled up, and how to scale down high-res atlases using `LoadFontEx` and `SetTextureFilter`.
* **Unreachable Code analysis**: Using IDE compiler warnings to detect logic paths that are impossible to reach.



## Day 7: July 15, 2026
*Target: FSM UI Expansion & Mouse Button Interactive Collisions*

### Tasks Done
1. **Mouse Coordinate Collision**: Integrated real-time mouse hover and click detection using Raylib's `GetMousePosition()` and `CheckCollisionPointRec()` inside screen controllers.
2. **Interactive GUI Buttons**: Designed custom rounded outlines for menu buttons ("START", "SETTINGS") and the settings screen "BACK" button, with dynamically centered button text.
3. **Settings Screen Integration**: Created the `SettingsScreen` class inside `global/states/settingScreen.h` inheriting from `GameScreen`.
4. **Extended Router Switch**: Wired `GameStates::STATE_SETTINGS` into the main switcher in `main.cpp` to seamlessly transition between the Menu and Settings.

### Where I Stuck / Issues Faced
1. **Relative Paths inside Nested Folders**: Had to ensure the relative header search paths (like `../constants.h`) correctly resolved inside the nested `global/states/` subdirectory.

### Mistakes Made
*None. Excellent independent implementation of mouse bounds.*

### Concepts Learned
* **Point-in-Rectangle Geometry**: How to mathematically resolve cursor hover conditions using bounding box coordinate checks.
* **FSM Scalability**: Observing how easily a state machine expands to include new UI screens with zero modification to core gameplay logic.


## Day 6: July 13, 2026
*Target: Hybrid Game Loop State Machine Router*

### Tasks Done
1. **State Machine Switchboard**: Restructured `main.cpp`'s main loop into a top-level `if/else` block separating active gameplay frames from UI screen frames.
2. **Polymorphic Memory Swapper**: Programmed the transition checker to delete the active screen object, update `currentState`, and instantiate new screen children.
3. **Screen Interface Refactoring**: Changed `Update()`'s return type in `GameScreen` and `MenuScreen` from `void` to `GameStates` to enable transition messaging.
4. **Minimalist Menu UI**: Designed a pure, flat, dark minimalist main menu displaying `"vstrike"` and `"press enter"`.

### Where I Stuck / Issues Faced
1. **Null Pointer Segmentation Fault**: Realized that calling `Draw()` or `Update()` on `currentScreen` after deleting it (when entering gameplay) causes a crash. Resolved by setting it to `nullptr` and wrapping draw calls in safety checks.
2. **Single-Frame Loop Trap**: Realized that nesting gameplay inside a transition switch runs it for only one frame. Resolved by separating frame loops.

### Mistakes Made
* **State vs. Transition Nesting**: Accidentally placed the drawing and updating calls inside the transition block, freezing the game loop.

### Concepts Learned
* **Unidirectional State Flow**: Designing screens to return states to the parent coordinator instead of mutating values externally.
* **Anti-Sticking Math**: Aligning positional correction values with boundary checks to prevent endless collision jitter loops.


## Day 5: July 12, 2026
*Target: Codebase Modularity Refactoring & GameState Enum Class*

### Tasks Done
1. **HUD Modularity**: Decoupled the HUD health bar rendering into a dedicated `UI/HealthBar.h` and `UI/HealthBar.cpp` module.
2. **Outcome Decoupling**: Moved the win state screen dimming, text overlay, and restart logic into `GameOutcomeAndRestart()` inside `physics/physicsEngine.cpp`.
3. **Core Loop Cleanliness**: Reduced the size of `main.cpp` to a compact 49-line main loop.
4. **Header Architecture Splitting**: Renamed the `common/` folder to `global/` and split its contents into `global/constants.h` (dimensions) and a new `global/gameStates.h` (state enums).
5. **Type-Safe Game States**: Declared a modern C++ `enum class GameStates` containing `STATE_MENU`, `STATE_CHAR_SELECT`, `STATE_GAMEPLAY`, and `STATE_GAMEOVER`.

### Where I Stuck / Issues Faced
1. **Compilation Paths Update**: Had to verify and update relative include paths in `main.cpp` and `CMakeLists.txt` to reflect the folder migration from `common/` to `global/`.

### Mistakes Made
*None during this short 1-hour session. Clean execution.*

### Concepts Learned
* **Modern C++ Strong Typing (`enum class`)**: Understanding the scope advantages of scoped enums (`enum class`) over traditional C-style enums, preventing naming collisions.
* **Component-Level UI Separation**: Isolating visual rendering details from state controllers.


## Day 4: July 7, 2026
*Target: Architectural Refactoring & Physics Engine Modularization*

### Tasks Done
1. **Window Branding**: Renamed the game window title to the official name `"vStrike"` in `main.cpp`.
2. **Physics Module Isolation**: Created the `physics/` directory to host physical interactions and scoring rules.
3. **Collision Extrication**: Extracted all circle-to-rectangle paddle collision math and anti-sticking offset corrections into `ResolveCollision()` inside `physics/physicsEngine.cpp`.
4. **Scoring Logic Isolation**: Moved screen boundary exit detections, player HP reductions, and default ball resets into `CheckScoreAndReset()` in the new physics module.
5. **CMake Registration**: Linked the new `physicsEngine` headers and source files in `CMakeLists.txt`.
6. **Main Loop Decoupling**: Reduced the size of `main()` in `main.cpp` by replacing raw blocks with clean, expressive function calls.

### Where I Stuck / Issues Faced
1. **Relative Directory Paths**: Had to ensure the relative header search paths (`../entities/` and `../common/`) correctly navigated the new folder structure.

### Mistakes Made
*None during this sprint. The refactoring compiled cleanly on the first try.*

### Concepts Learned
* **Separation of Concerns (SoC)**: Keeping rendering loop controllers (`main.cpp`) focused on draw orchestration while delegate engines (`physicsEngine.cpp`) govern simulation math.
* **Pass-by-Reference Modifiers**: Reinforcing usage of `&` parameters in modular functions to prevent object duplication and ensure mutations apply to the live game state.


## Day 3: July 5-6, 2026
*Target: HP Bar Rendering, Win/Loss States, Fade Overlay, & Replay Scaling*

### Tasks Done
1. **Header Initializer Refactor**: Refactored the `Paddle` constructor in `paddle.h` to use a C++ Member Initializer List for `hp` and `maxHp`, solving variable shadowing.
2. **Dynamic HP Bars**: Rendered twin, rounded HUD health bars (`DrawRectangleRounded`) scaling dynamically via `paddle.hp / paddle.maxHp`.
3. **Sequential Damage Logic**: Placed HP subtraction checks inside the coordinate-miss blocks, ensuring damage is applied *before* the ball position is reset to the center.
4. **Correct Side Damage Mapping**: Fixed boundary checks so left-side misses (`Cx < 0`) correctly damage the left player (`paddle1`) and right-side misses damage `paddle2`.
5. **Aesthetic Dimming Overlay**: Implemented a semi-transparent screen overlay using `Fade(BLACK, 0.6f)` to dim the gameplay backdrop on win states.
6. **Replay Loop & Speed Escalation**: Programmed a match restart check (`IsKeyPressed(KEY_R)`) that resets player HP and multiplies subsequent ball velocities using a cumulative `multiplier` factor to increase difficulty across matches.

### Where I Stuck / Issues Faced
1. **Linker main Overlap**: Hit a build failure (`multiple definition of main`) because CLion auto-added the `DSP/test.cpp` scratch file to the main `add_executable` target.
   * *Resolution*: Removed `DSP/` source files from the main target in `CMakeLists.txt`.
2. **Clangd Index Desync**: Encountered ghost red squiggles on standard Raylib functions (`EndDrawing()`, etc.) even though the code compiled and ran fine.
   * *Resolution*: Reset the CMake cache in CLion and restarted the Clangd Language Engine.
3. **Ghost Health Bar (Zero Width)**: The health bar disappeared when using the percentage equation.
   * *Resolution*: Realized `hp` and `maxHp` were uninitialized garbage floats in the constructor. Initializing them via member initializer lists resolved the math.

### Mistakes Made
1. **Mutated Check Sequence**: Checked ball borders and reset coordinates *before* evaluating HP reduction, making the HP reduction checks unreachable.
2. **Reversed Damage Targets**: Set the left miss to damage the right paddle, and vice versa.
3. **Missing Semicolon/Parenthesis cascade**: An unfinished `DrawRectangle` argument list caused the parser to throw syntax errors on completely unrelated lines below it.

### Concepts Learned
* **Clangd Language Server Mechanics**: Understanding why real-time IDE diagnostics fall out of sync with compilers and how to reset the index cache.
* **Discrete Collision State Transitions**: Structuring checks so state mutation (coordinate resets) doesn't shadow subsequent state evaluation (HP deductions).
* **Faded Layering**: Using alpha-blended transparency overlays (`Fade`) to draw focus to UI alerts without modifying game state cameras or shaders.

### Questions to Research
* What is the performance cost of calling `DrawRectangleRounded` multiple times per frame on the GPU?
* How does Raylib's double-buffering (`EndDrawing`) swap system work behind the scenes?


## Day 2: July 4, 2026
*Target: Ball Entity, Constructor Initialization List, & Wall Bouncing*

### Tasks Done
1. **Split-Compilation for Ball**: Implemented `entities/ball.h` and `entities/ball.cpp` and linked them inside the `PongArena` target.
2. **Sub-Pixel Precision Physics**: Converted `Cx` and `Cy` coordinate variables from `int` to `float` to enable continuous floating-point physics calculations and prevent integer truncation.
3. **C++ Initializer List Constructor**: Implemented the C++ member initializer list syntax (`: Cx(Cx), Cy(Cy)... {}`) for the `Ball` struct, cleaning up `main.cpp` instantiation to a single line.
4. **Collision Bouncing**: Coded collision checks for all 4 viewport boundaries to invert velocities (`speedX` and `speedY`).
5. **Positional Correction**: Applied boundary adjustments (e.g., `Cy = radius` at top, `Cy = HEIGHT - radius` at bottom) to push the ball out of collision zones upon hit.
6. **Paddle Collisions**: Wired up `CheckCollisionCircleRec` for both paddles inside `main.cpp`'s game loop, correcting X-positions upon hit (`paddle1.x + paddle1.width + ball.radius` and `paddle2.x - ball.radius`) to prevent sticky frames.
7. **Scoring Resets**: Commented out left/right wall bounces in `ball.cpp` and implemented active off-screen resets in `main.cpp` to reset the ball position to screen center when missed.

### Where I Stuck / Issues Faced
1. **Integer Physics deadlocks**: Figured out why using `int` for positions caused physics stuttering and deadlocks at high FPS due to float decimals dropping below `1.0f` and truncating to `0`.
2. **Sticky Collision Jitter Loop**: Debugged the issue where the ball gets trapped on a boundary, vibrating back and forth because it doesn't escape the collision box in a single frame.
   * *Resolution*: Applied positional offsets immediately upon collision detection.
3. **Loop Lifecycle Mismatches**: Realized that declaring collision shape trackers (`Vector2 center`, etc.) outside the main `while` loop prevents them from updating as the entities move. Moved them inside the loop.

### Mistakes Made
1. **Double Inversion**: Initially inverted both `speedX` and `speedY` on paddle collisions, which caused unnatural vertical bounces. Resolved by only inverting `speedX`.
2. **Left Paddle Offset Math**: Initially calculated `paddle1.x - paddle1.width - ball.radius` for the left paddle correction, which pushed the ball off-screen. Resolved to `+` additions.

### Concepts Learned
* **Member Initializer Lists**: Mastered the syntax `: member(param)` and understood how it directly constructs members, bypassing the double-overhead of default constructor + assignment.
* **Double-Bounce Physics (Sticky Boundaries)**: Learned how discrete collision checks create sticky boundaries if position isn't corrected or direction isn't checked.
* **C++ Casting Mechanics**: Used C-style casts `(int)` to bridge floating-point physics coordinates with integer-bound drawing APIs.

### Questions to Research
* What is the difference between C-style casting `(int)var` and C++ style casting `static_cast<int>(var)`? Which one is safer and why?
* How does the compiler treat Member Initializer Lists under the hood in terms of construction order (does declaration order in the header matter)?
* How do you implement circle-to-rectangle collision response (necessary for paddle hits)?


## Day 1: July 3, 2026
*Target: Paddle Entity, Independent 2-Player Controls, & Clamping*

### Tasks Done
1. **Split-Compilation Architecture**: Configured C++ workspace with `entities/paddle.h` (declarations) and `entities/paddle.cpp` (definitions), linking them via `CMakeLists.txt`.
2. **Independent Controls (Option B)**: Bound `upKey` and `downKey` as member variables in the `Paddle` struct to let Player 1 (`W`/`S`) and Player 2 (`UP`/`DOWN` arrows) move independently.
3. **C++ Constructor Implementation**: Created a custom parameter constructor for the `Paddle` struct to eliminate 16 lines of boilerplate initialization in `main.cpp`.
4. **Framerate Independence**: Integrated Delta Time (`GetFrameTime()`) into movement calculations.
5. **Boundary Clamping**: Implemented bounds checking to clamp `y` coordinates between `0` (top) and `HEIGHT - height` (bottom).
6. **Polished Rendering**: Styled drawing with `DrawRectangleRounded` using dynamic color and coordinate structures.
7. **Court Details**: Added center-line division and title text ("MARINO" / "SILICO").

### Where I Stuck / Issues Faced
1. **Single-File Compiler Fallback**: Ran into a compiler error because CLion was executing a raw `g++.exe` command on `main.cpp` instead of using the CMake `PongArena` target which links Raylib.
   * *Resolution*: Switched the target run configuration dropdown in CLion to `PongArena`.
2. **CMake Policy Mismatch**: Hit a compatibility error with Raylib 5.0's script on newer CMake versions (`Compatibility with CMake < 3.5 has been removed`).
   * *Resolution*: Set `CMAKE_POLICY_VERSION_MINIMUM 3.5` in the main `CMakeLists.txt` to bypass the policy check on external dependencies.
3. **Variable Mismatch in `.cpp`**: Got compiler errors in `paddle.cpp` because placeholder variables from syntax examples (`x_val`, `round`, `segments`) were written instead of using the actual struct members and literal values.
   * *Resolution*: Bound members (`x`, `y`, `width`, `height`) to the Raylib `Rectangle` struct, and passed literal floats/ints (`0.4f`, `8`) into `DrawRectangleRounded`.
4. **Symbol Mismatch (Cannot Resolve `downKey`)**: Hit a linting error because of a mixed-up control architecture. Tried to assign keys in the constructor, but forgot to declare `upKey` and `downKey` as members in `paddle.h`.
   * *Resolution*: Fully declared the key fields inside the struct.

### Mistakes Made
1. **Global Variable Instantiation**: Declared `struct Rect { ... } Rect;` in the header, which instantiates a global variable named `Rect` and causes duplicate definition linker errors when included in multiple files.
2. **Uninitialized Speed/Color**: Forgot to set `speed` in `main.cpp`, leaving the memory address to hold random garbage data.
3. **Drawing Sequence Bug**: Called `ClearBackground` *after* drawing the paddle, wiping the paddle off the screen before it could render.
4. **Hardcoded Aesthetics**: Used `BLACK` directly inside `paddle.cpp`'s drawing call instead of utilizing the dynamic `color` variable belonging to the instance.

### Concepts Learned
* **Difference between Declaration and Definition**: Declaring *what* exists in `paddle.h` vs. defining *how* it behaves in `paddle.cpp`.
* **CMake Target Linking**: Understanding why raw files compile single-file and fail without target linkage configurations linking external deps (like Raylib).
* **Boilerplate Reduction via Constructors**: Cleaning up object initialization in `main.cpp` using constructors instead of repetitive member assignments.
* **Delta Time & GetFrameTime()**: The core engine math of multiplying movement vectors by elapsed frame time to decouple physics speed from CPU performance.
* **Header Hygiene**: Using header guards (`#ifndef` / `#define` / `#pragma once`) and managing include directory paths relative to nested targets.
* **Global Struct Instantiation Pitfall**: The bug of instantiating variables directly after class/struct braces in C++ headers, causing linker conflicts.
* **Object Ownership**: Structuring object configurations (like movement keys) inside their respective class layouts rather than managing them globally.

### Questions to Research
* Why are include guards or `#pragma once` strictly necessary in C++ compared to newer languages?
* Why does compiling/defining non-const global variables in headers throw a duplicate symbol/multiple-definition error at the link stage?
* Why is thread-blocking forbidden in real-time callbacks, and what alternatives (like rings/queues) are used to send trigger cues from main thread to audio thread?
