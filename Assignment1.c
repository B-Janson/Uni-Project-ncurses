#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>

// ADD INCLUDES HERE

// Configuration
#define DELAY (10) /* Millisecond delay between game updates */

#define HERO_WIDTH (5)
#define HERO_HEIGHT (5)
#define ZOMBIE_WIDTH (5)
#define ZOMBIE_HEIGHT (5)

// Game state.
bool game_over = false; /* Set this to true when game is over */
bool update_screen = true; /* Set to false to prevent screen update. */

char * hero_image =
/**/	"H   H"
/**/	"H   H"
/**/	"HHHHH"
/**/	"H   H"
/**/	"H   H";

char * zombie_image =
/**/	"ZZZZZ"
/**/	"   Z "
/**/	"  Z  "
/**/	" Z   "
/**/	"ZZZZZ";

char * msg_image =
/**/	"Goodbye and thank-you for playing ZombieDash Jr."
/**/	"            Press any key to exit...            ";

char border_character = '#';

// // (c) Declare a sprite_id called hero.
// sprite_id hero;

// // (m) Declare a sprite_id called zombie.
// sprite_id zombie;

void draw_help_dialog(void) {
	int height = 14;
	int width = 78;


}

void draw_rectangle(int x, int y, int width, int height, char character) {
	// int height = screen_height();
	// int width = screen_width();

	int left = x;
	int right = width - 1;
	int top = y;
	int bottom = height - 1;

	draw_line(left, top, right, top, character);
	draw_line(right, top, right, bottom, character);
	draw_line(right, bottom, left, bottom, character);
	draw_line(left, bottom, left, top, character);
}

void draw_border(void) {
	draw_rectangle(0, 0, screen_width(), screen_height(), border_character);
}

// Setup game.
void setup(void) {
	draw_border();
}

// Play one turn of game.
void process(void) {
	
}

// Clean up game
void cleanup(void) {
	// STATEMENTS
}

// Program entry point.
int main(void) {
	setup_screen();
	setup();
	show_screen();

	while ( !game_over ) {
		process();

		if ( update_screen ) {
			show_screen();
		}

		timer_pause(DELAY);
	}

	cleanup();

	return 0;
}