#include <stdio.h>
#include "game/game.h"


int main(int argc, char *argv[]) {
    printf("Hello, World!\n");

    const char *game_mode = "good-bot";
    if (argc > 1) {
        game_mode = argv[1];
    }

    play_game(game_mode);
}