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

#define SHIP_WIDTH (7)
#define SHIP_HEIGHT (3)
#define DIAMOND_WIDTH (5)
#define DIAMOND_HEIGHT (5)
#define MISSILE_WIDTH (1)
#define MISSILE_HEIGHT (1)

// Game state.
bool game_over = false; /* Set this to true when game is over */
bool update_screen = true; /* Set to false to prevent screen update. */
bool missile_in_flight = false; /* Determines if missile already been shot */

char * ship_image =
/**/	"   |   "
/**/	"  /o\\  "
/**/	"|/___\\|";

char * diamond_image =
/**/	"  C  "
/**/	" CCC "
/**/	"CCCCC"
/**/	" CCC "
/**/	"  C  ";

char * missile_image =
/**/	"!";

char * msg_image =
/**/	"Goodbye and thank-you for playing ZombieDash Jr."
/**/	"            Press any key to exit...            ";

char border_character = '#';
//char missile = '!';

int lives;
int score;
int timePlayed;
int timeStart;
int minutes;
int seconds;

sprite_id ship;

sprite_id diamond;

sprite_id missile;

void draw_rectangle(int x, int y, int width, int height, char character) {
	int left = x;
	int right = x + width - 1;
	int top = y;
	int bottom = y + height - 1;

	// Top border
	draw_line(left, top, right, top, character);
	// Right border
	draw_line(right, top, right, bottom, character);
	// Bottom border
	draw_line(right, bottom, left, bottom, character);
	// Left border
	draw_line(left, bottom, left, top, character);
}

void draw_help_dialog(void) {
	
	// Dimensions as demonstrated in example video
	int width = 50;
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
	// Draw border around outside
	draw_rectangle(0, 0, screen_width(), screen_height(), border_character);
	// Draw line break between info and game window
	draw_line(0, 2, screen_width() - 1, 2, border_character);

	int middle = screen_width()/2;
	draw_formatted(2, 1, "Lives = %i", lives);

	draw_formatted(middle, 1, "# Score = %i", score);

	draw_formatted(screen_width() - 20, 1, "# Time = %i:%02d", minutes, seconds);
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

	// Initalise global variables
	lives = 10;
	score = 0;
	timePlayed = 0;
	timeStart = get_current_time();

	// Draw the border and show the screen
	draw_border();
	show_screen();

	// Setup game bits
	ship = sprite_create((screen_width() - SHIP_WIDTH) / 2, screen_height() - 1 - SHIP_HEIGHT, SHIP_WIDTH, SHIP_HEIGHT, ship_image);

	sprite_draw(ship);

	show_screen();

	int seed = time(NULL);
	srand(seed);

	// Place diamond in random position at top of screen
	int diamondX = rand() % (screen_width() - DIAMOND_WIDTH - 2);
	diamond = sprite_create(diamondX, 3, DIAMOND_WIDTH, DIAMOND_HEIGHT, diamond_image);

	sprite_draw(diamond);

	show_screen();

	// Give it a downward velocity
	sprite_turn_to(diamond, 0, 0.15);
	// Choose an angle between 0-16 then move that range to (-8, 8)
	int degrees = rand() % 16;
	sprite_turn(diamond, (degrees - 8)*9);

	show_screen();

}

void shootMissile(int x, int y) {
	missile = sprite_create(x, y, MISSILE_WIDTH, MISSILE_HEIGHT, missile_image);
	sprite_draw(missile);

	show_screen();

	//update_screen = false;

	sprite_turn_to(missile, 0, -0.2);
	show_screen();
}

// Play one turn of game.
void process(void) {

	// Get the next key pressed
	int key = get_char();

	// If they press q then quit the game
	if(key == 'q') {
		game_over = true;
		return;
	}

	// Work out how long the game has gone for and format
	timePlayed = get_current_time() - timeStart;
	minutes = timePlayed / 60;
	seconds = timePlayed % 60;

	// Get ship's current position
	int ship_x = round(sprite_x(ship));
	int ship_y = round(sprite_y(ship));

	// If wouldn't be out of bounds and left arrow pressed
	if(key == 260 && ship_x > 1) {
		sprite_move(ship, -1, 0); // move left
	}

	// If wouldn't be out of bound and right arrow pressed
	if(key == 261 && ship_x + SHIP_WIDTH < screen_width() - 1) {
		sprite_move(ship, +1, 0); // move right
	}

	if((key == ' ' || key == 'z' || key == 'c' || key == 'x') && !missile_in_flight) {
		shootMissile(ship_x + SHIP_WIDTH / 2, ship_y - 1);
		missile_in_flight = true;
	}

	// Moving the diamond and keeping in bounds
	sprite_step(diamond);

	int diamond_x = round(sprite_x(diamond));
	int diamond_y = round(sprite_y(diamond));

	double diamond_dx = sprite_dx(diamond);
	double diamond_dy = sprite_dy(diamond);

	if(diamond_x == 0 || diamond_x + DIAMOND_WIDTH == screen_width() - 1) {
		diamond_dx = -diamond_dx;
	}

	if(diamond_y == 2 || diamond_y + DIAMOND_HEIGHT == screen_height()) {
		diamond_dy = -diamond_dy;
	}

	if(diamond_dx != sprite_dx(diamond) || diamond_dy != sprite_dy(diamond)) {
		sprite_back(diamond);
		sprite_turn_to(diamond, diamond_dx, diamond_dy);
	}	

	// Remove everything for re-draw
	clear_screen();

	draw_border();

	sprite_draw(ship);

	sprite_draw(diamond);

	if(missile_in_flight) {
		int  missile_x = round(sprite_x(missile));
		int  missile_y = round(sprite_y(missile));
		if(missile_y == 2) {
			sprite_destroy(missile);
			missile_in_flight = false;
		} else {
			sprite_step(missile);
			sprite_draw(missile);
		}

	}

	
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