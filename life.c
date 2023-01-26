#include "life.h"
#include "config.h"

#include <sys/random.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <png.h>

struct Game *Game_Init(int width, int height) {
    int cellcount = width * height;
    struct Game *game = malloc(sizeof(struct Game));
    game->generation = 0;
    game->len = sizeof(bool) * cellcount;
    game->grid = malloc(game->len);
    game->past_grid = malloc(game->len);

    for (int i = 0; i < cellcount; i++) {
        game->grid[i] = false;
        game->past_grid[i] = false;
    }

    game->width = width;
    game->height = height;

    return game;
}

void Game_Random(struct Game *game) {
    unsigned int seed;
    if (0 >= getrandom(&seed, sizeof(seed), 0)) {
        perror(NULL);
        exit(EXIT_FAILURE);
    }
    srand(seed);
    for (int i = 0; i < game->width * game->height; i++) {
        game->grid[i] = (rand() % 2) ? false : true;
    }
}

void Game_Render(struct Game *game) {

    static char fname[20];
    static png_byte *buffer = NULL;
    static png_image *imagep = NULL;
    int buffer_size = game->width * game->height * SCALE * SCALE * sizeof(png_byte);

    // allocate the buffer once
    if (buffer == NULL) {
        buffer = malloc(buffer_size);
        if (buffer == NULL) {
            fprintf(stderr, "Error: malloc failed\n");
            exit(EXIT_FAILURE);
        }
    }
    memset(buffer, 0, buffer_size);

    if (imagep == NULL) {
        imagep = malloc(sizeof(png_image));
        if (imagep == NULL) {
            fprintf(stderr, "Error: malloc failed\n");
            exit(EXIT_FAILURE);
        }
    }

    memset(imagep, 0, sizeof(png_image));
    imagep->version = PNG_IMAGE_VERSION;
    imagep->opaque = NULL;
    imagep->width = game->width * SCALE;
    imagep->height = game->height * SCALE;
    imagep->format = PNG_FORMAT_GRAY;
    imagep->flags = 0;
    imagep->colormap_entries = 0;

    sprintf(fname, "life_%05lu.png", game->generation);
    for (int h = 0; h < game->height; h++) {
        for (int sh = 0; sh < SCALE; sh++)  //scale for height
            for (int w = 0; w < game->width; w++) {
                for (int s = 0; s < SCALE; s++) {
                    int index = w + h * game->width;
                    int buffer_index = (w * SCALE) + s + (((h * SCALE) + sh) * game->width * SCALE); // h * scale * game->w + sh * game->w
                    buffer[buffer_index] = (game->grid[index]) ? 0 : 255;
                }
            }
    }

    png_image_write_to_file(imagep, fname, 0, buffer, 0, 0);
}


/* check the rules to see whether the cell at (x, y)
 * should be alive next generation or not */
bool check_rules(struct Game *game, int x, int y) {
    bool *grid = game->past_grid;
    int c = 0; /* number of neighbours */
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) /* we don't wanna count the cell itself */
                continue;
            int nx = x + i;
            int ny = y + j;
            if (nx >= game->width || nx < 0 || ny >= game->height || ny < 0) {
                continue;
            }
            if (grid[nx + ny * game->width]) {
                c++;
            }
        }
    }

    if (grid[x + game->width * y]) { // cell is alive
        if (c == 2 || c == 3) {
            return true;
        } else {
            return false;
        }
    } else { // cell is dead
        if (c == 3) {
            return true;
        } else {
            return false;
        }
    }
}


void Game_Tick(struct Game *game) {

    game->generation++;
    memcpy(game->past_grid, game->grid, game->len);

    for (int i = 0; i < game->width; i++) {
        for (int j = 0; j < game->height; j++) {
            game->grid[i + game->width * j] = check_rules(game, i, j);
        }
    }
}

struct Game *parse_init_file(const char *name) {
    errno = 0;
    FILE *file = fopen(name, "r");
    if (errno != 0) {
        perror(NULL);
        exit(EXIT_FAILURE);
    }

    // count the 0 and 1s in the first line to get width
    int c;
    int width = 0;
    int height = 0;
    do {
        c = fgetc(file);
        if (c == '0' || c == '1') {
            width += 1;
        }
    } while (c != '\n' && c != EOF);

    // handle the last line either having or not having a \n
    bool seen = false;
    c = '1';
    rewind(file);
    while (c != EOF) {
        c = fgetc(file);
        if (c == '0' || c == '1') {
            seen = true;
        } else if (c == '\n' || c == EOF) {
            if (seen) {
                height += 1;
                seen = false;
            }
        }
    }

    struct Game *game = Game_Init(width, height);

    rewind(file);
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            int c;
            do {
                c = fgetc(file);
            } while (c != '0' && c != '1');
            game->grid[w + (width * h)] = (c == '1');
        }
    }
    return game;
}


void print_help(const char *name) {
    printf(
        "Usage: %s gen_from gen_to [init_file]\n"
        "    gen_from:     Produce output from this generation onward\n"
        "    gen_to:       Stop generating output after this generation\n"
        "    init_file:    The file containing generation 0,\n"
        "                  or omit it to use a random initial state\n",
        name);
}


int main(int argc, char *argv[]) {

    int gen_from;
    int gen_to;
    struct Game *game;

    if (argc == 4) {
        game = parse_init_file(argv[3]);

    } else if (argc == 3) {
        game = Game_Init(WIDTH, HEIGHT);
        Game_Random(game);

    } else {
        print_help(argv[0]);
        exit(EXIT_FAILURE);
    }

    errno = 0;
    gen_from = strtol(argv[1], NULL, 10);
    gen_to = strtol(argv[2], NULL, 10);
    if (errno != 0) {
        perror(NULL);
        exit(EXIT_FAILURE);
    }

    // Don't render unwanted generations
    for (int i = 0; i < gen_from; i++) {
        Game_Tick(game);
    }

    for (int i = gen_from; i <= gen_to; i++) {
        Game_Render(game);
        Game_Tick(game);
    }

    return EXIT_SUCCESS;
}
