# say-it-with-your-chesst

## Description
Chess engine written in pure C. 
The engine is able to play chess games with a human player over the command line or with another engine.

## Design
The engine uses the [minimax](https://en.wikipedia.org/wiki/Minimax) algorithm with [alpha-beta pruning](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning) to search for the best move.

This generates a tree of possible moves, speculatively executes them and evaluates the board position at the end of each branch. Rolling back the speculative moves made. 
The engine then selects the best move based on the evaluation of the board position.
The depth of the search can be controlled by passing a parameter to the `minimax` function. 
Currently going deeper than 5 gets prohibitively slow ~20 seconds for some moves.

### Score Eval
The engine also uses a simple evaluation function to evaluate the board position.
This currently does:
- Material score
- Naive positional score

## Build instructions
This is only designed to work on MacOS.

### IDE build instructions
Built through the IDE by hitting run on `say_it_with_your_chesst` target. 

⚠️ **Note**: This line in the CMakeLists.txt file is brittle and may break if the SDK version changes.:
```cmake
set(CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX15.2.sdk) # Without this it tries to use the 15.1 SDK
```

### Cmd line build instructions
```bash
cd build
cmake ..
make
./say_it_with_your_chesst
```

### Build Args
You can switch between the black pieces being controlled by:
- A good bot (the same as white) 
- A bad bot (looks one move deep and picks the best one)
- User input (you play as black)

To do this you pass in three args at runtime, this looks like:
```bash
./say_it_with_your_chesst good-bot
./say_it_with_your_chesst bad-bot
./say_it_with_your_chesst user
```

This can also be done through the IDE by editing the `Run Configuration -> Arguments` field.

## Tests 
The tests use the CUnit test framework.

When adding new tests, register them to the `CMakeLists.txt` file in the root dir. This file will then need to be refreshed. 

### Running All Tests
The tests can be built and run through the IDE with the `All CTest` target.

There are also a single target that runs all the tests:
```bash
cd build
cmake ..
make tests
```

### Running Individual Test
The tests can be run individually through the IDE by hitting run on the target for a given test, such as `move_test`.

Or through the command line:
```bash
cd build
cmake ..
make move_test
./move_test
```

### Full Game Test
The `full_game_test` is slightly different to the other tests. It runs a full game between the engine and another engine.

#### Against Bad Bot
This test runs a full game between the engine and a naive version of itself, which always executes the first of all available moves.
This means the engine should beat it in exactly 9 moves. As the eval function of the engine changes, this will likely need updated.

#### Against Good Bot
This test runs a full game between the engine and itself. 
As there is no three-fold repetition or fifty-move rule (yet), the game will continue indefinitely, so a timeout has been added.

#### Random Test
One of the tests in `full_game_test.c` generates both black and whites moves randomly. This is to test the engine can handle a wide range of moves and board states. 
For reproducibility, the moves that the test executes are written to a file `moves.txt`. These can then be replayed with the  `replay_random_moves` test, which is currently commented out.
