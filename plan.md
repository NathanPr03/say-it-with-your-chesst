
- [x] Create plan.md
- [x] Create README.md, build instructions etc
- [x] Fix gitignore
- [x] Deal with branched user input code
   - [x] Set build args to switch between good, bad, and branched user input code
- [x] Handle promotion
   - [x] Add queen only promotion
   - [x] Add promotion to other pieces (add them as separate moves which can be scored)
- [ ] Handle en passant
   - [ ] Keep track of the last move
   - [ ] Add en passant as a move
- [ ] Handle castling
- [ ] Integration tests
- [ ] Clean up code
   - [ ] Extract execute and undo logic into command pattern
      - [ ] Create stack of previous moves 
   - [ ] Resolve TODOs
   - [ ] move_picker.c should have a separate move/board score function
   - [ ] move.c needs split up. This will be risky and needs thorough integration tests in place first I think
   - [ ] Move piece structs from board.h to a new pieces.h
   - [ ] Rearrange Colour struct to have NONE first. And replace all usages of Colour = 0 being empty with NONE.
   - [ ] CamelCase or snake_case?
   - [ ] colour vs color
  

