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
This project has sparse, incomplete, and likely flakey integration tests. There are no, and will be no unit tests.

The tests use the CUnit test framework.

They can be run through the IDE by hitting run on the `tests` target.

Or through the command line:
```bash
cd build
cmake ..
make tests
./tests
```

There are also separate targets for each test file:
```bash
cd build
cmake ..
make tests
./move_test
./promotion_test
```