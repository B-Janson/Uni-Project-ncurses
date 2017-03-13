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

// (c) Declare a sprite_id called hero.
sprite_id hero;

// (m) Declare a sprite_id called zombie.
sprite_id zombie;

void draw_border() {
	int h = screen_height();
	int w = screen_width();

	int left = 0;
	int right = w - 1;
	int top = 0;
	int bottom = h - 1;

	draw_line(left, top, right, top, '*');
	draw_line(right, top, right, bottom, '*');
	draw_line(right, bottom, left, bottom, '*');
	draw_line(left, bottom, left, top, '*');
}

// Setup game.
void setup(void) {
	// (a) Draw the border (setup).
	draw_border();

	// Keep the next line intact.
	show_screen();

	// (d) Set up the hero at the centre of the screen.
	int hw = HERO_WIDTH, hh = HERO_HEIGHT;
	hero = sprite_create((screen_width() - hw) / 2, (screen_height() - hh) / 2, hw, hh, hero_image);

	// (e) Draw the hero.
	sprite_draw(hero);

	// Keep the next line intact.
	show_screen();

	// (n)	Set up the zombie at a random location on the screen.
	int seed = time(NULL);
	srand(seed);

	int zx = rand() % (screen_width() - hw - 2);
	int zy = rand() % (screen_height() - hh - 2);
	zombie = sprite_create(zx, zy, ZOMBIE_WIDTH, ZOMBIE_HEIGHT, zombie_image);

	// (o) Draw the zombie.
	sprite_draw(zombie);

	// (p) Set the zombie in motion.
	sprite_turn_to(zombie, 0.2, 0);
	int degrees = rand() % 360;
	sprite_turn(zombie, degrees);

	// Keep the next line intact.
	show_screen();
}

// Play one turn of game.
void process(void) {
	// return;

	// (f) Get a character code from standard input without waiting.
	int key = get_char();
	
	// (y) Test for end of game.

	// (g)	Get the current screen coordinates of the hero in integer variables 
	//		by rounding the actual coordinates.
	int hx = round(sprite_x(hero));
	int hy = round(sprite_y(hero));

	// (h) Move hero left according to specification.
	if(key == 'a' && hx > 1) {
		sprite_move(hero, -1, 0);
	}

	// (i) Move hero right according to specification.
	if(key == 'd' && hx + HERO_WIDTH < screen_width() - 1) {
		sprite_move(hero, 1, 0);
	}

	// (j) Move hero up according to specification.
	if(key == 'w' && hy > 1) {
		sprite_move(hero, 0, -1);
	}

	// (k) Move hero down according to specification.
	if(key == 's' && hy + HERO_HEIGHT < screen_height() - 1) {
		sprite_move(hero, 0, 1);
	}

	// (q.a) Test to move the zombie if key is 'z' or negative.
	if(key == 'z' || key < 0) {
		// (r) Zombie takes one step.
		sprite_step(zombie);

		// (s) Get screen location of zombie.
		int zx = round(sprite_x(zombie));
		int zy = round(sprite_y(zombie));

		// (t) Get the displacement vector of the zombie.
		double dx = sprite_dx(zombie);
		double dy = sprite_dy(zombie);

		// (u) Test to see if the zombie hit the left or right border.
		if(zx == 0 || zx + ZOMBIE_WIDTH == screen_width() - 1) {
			dx = -dx;
		}

		// (v) Test to see if the zombie hit the top or bottom border.
		if(zy == 0 || zy + ZOMBIE_HEIGHT == screen_height()) {
			dy = -dy;
		}

		// (w) Test to see if the zombie needs to step back and change direction.
		if(dx != sprite_dx(zombie) || dy != sprite_dy(zombie)) {
			sprite_back(zombie);
			sprite_turn_to(zombie, dx, dy);
		}

	// (q.b) End else-if test to move the zombie if key is 'z' or negative.
	}

	// Leave next line intact
	clear_screen();

	// (b) Draw the border (process).
	draw_border();

	// (l)	Draw the hero.
	sprite_draw(hero);

	// (x)	Draw the zombie.
	sprite_draw(zombie);
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