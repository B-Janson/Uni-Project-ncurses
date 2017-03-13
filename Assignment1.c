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

char * ship_image =
/**/	"   |    "
/**/	"  /o\\  "
/**/	"|/___\\|";

char * diamond_image =
/**/	"  C  "
/**/	" CCC "
/**/	"CCCCC"
/**/	"CCCCC"
/**/	" CCC "
/**/	"  C  ";

char * msg_image =
/**/	"Goodbye and thank-you for playing ZombieDash Jr."
/**/	"            Press any key to exit...            ";

char border_character = '#';

int lives;
int score;
int timePlayed;

sprite_id ship;

sprite_id diamond;

void draw_rectangle(int x, int y, int width, int height, char character) {
	int left = x;
	int right = x + width - 1;
	int top = y;
	int bottom = y + height - 1;

	draw_line(left, top, right, top, character);
	draw_line(right, top, right, bottom, character);
	draw_line(right, bottom, left, bottom, character);
	draw_line(left, bottom, left, top, character);
}

void draw_help_dialog(void) {
	
	// Dimensions as demonstrated in example video
	int width = 64;
	int height = 14;

	// Get box around centre of the screen to accomodate
	int x = (screen_width() - width) / 2;
	int y = (screen_height() - height) / 2;

	const int numStrings = 12;
	const int maxLength = 40;

	// Initalise string array to loop through easier
	char strings[numStrings][maxLength] = {
			"CAB202 Assignment 1",
			"The Diamonds Of Doom",
			"Brandon Janson",
			"n9494006",
			"",
			"Controls",
			"q\t: quit",
			"h\t: help",
			"   Arrow keys\t: move left/right",
			"Space, z, x, c : shoot bullet    ",
			"",
			"Press a key to play..."
		};
	
	// Draw the outline of the help dialog
	draw_rectangle(x, y, width, height, border_character);

	// Increment so that the strings are below the border
	y++;

	// Loop through the first four strings and display them on screen
	for(int i = 0; i < numStrings; i++) {
		int length = strlen(strings[i]);
		draw_formatted((screen_width() - length) / 2, y + i, strings[i]);
	}

	// Increment by 4 so that y is now equal to the next line break
	y += 4;

	// Draw line break
	draw_line(x, y, x + width - 1, y, border_character);

	// Increment by 6 so that y is now equal to the next line break
	y += 6;

	// Draw line break
	draw_line(x, y, x + width - 1, y, border_character);
}

void draw_border(void) {
	draw_rectangle(0, 0, screen_width(), screen_height(), border_character);
	draw_line(0, 2, screen_width() - 1, 2, border_character);

	int middle = screen_width()/2;
	draw_formatted(2, 1, "Lives = %i", lives);

	draw_formatted(middle, 1, "# Score = %i", score);

	draw_formatted(screen_width() - 20, 1, "# Time = %i", timePlayed);
}

// Setup game.
void setup(void) {
	// Display the intial help dialog
	draw_help_dialog();
	show_screen();

	// Wait for key input to start the game
	int key = wait_char();

	// Clear the screen ready for the game to start
	clear_screen();

	lives = 10;
	score = 0;
	timePlayed = 0;

	// Draw the border and show the screen
	draw_border();
	show_screen();
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