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
#define SHIP_BASE	(SHIP_WIDTH)
#define SHIP_MID	(3)
#define SHIP_TOP	(1)

#define DIAMOND_WIDTH (5)
#define DIAMOND_HEIGHT (5)

#define MISSILE_WIDTH (1)
#define MISSILE_HEIGHT (1)

#define MAX_MISSILES (100)
#define MAX_DIAMONDS (10)

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
/**/	"    Game Over. Would you like to play again?    "
/**/	"                    Press Y/N                  ";

char border_character = '#';

int lives;
int score;
int timePlayed;
int timeStart;
int minutes;
int seconds;
int missileCount;
int diamondsLeft;

sprite_id ship;

//sprite_id diamond;

// For posterity
//sprite_id missile;

sprite_id diamonds[MAX_DIAMONDS];

sprite_id missiles[MAX_MISSILES];

void draw_rectangle(int x, int y, int width, int height, char character);
void draw_help_dialog(void);
void draw_border(void);
void setup(void);
void shootMissile(int x, int y);
bool collided(sprite_id firstSprite, sprite_id secondSprite);
void pauseAndDisplayHelp(void);
bool quitGame(void);
void resetGame(void);
void calculateTimeElapsed(void);
void moveShip(int key);
// void moveDiamond(void);
void moveDiamond(int index);
void moveMissile(void);
//bool missileInFlight(void);
bool canShootMissile(void);
void process(void);
void cleanup(void);

// Method used to draw rectangle from pos top left of (x,y) of width and height 
// and drawn with character specified
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
	// Purge keyboard buffer
	while(get_char() >= 0) {}
	
	// Dimensions as demonstrated in example video
	int width = 50;
	int height = 14;

	// Get box around centre of the screen to accomodate
	int x = (screen_width() - width) / 2;
	int y = (screen_height() - height) / 2;

	const int numStrings = 12;
	// const int maxLength = 40;

	char * infoMessages[numStrings];

	infoMessages[0] = "CAB202 Assignment 1";
	infoMessages[1] = "The Diamonds Of Doom";
	infoMessages[2] = "Brandon Janson";
	infoMessages[3] = "n9494006";
	infoMessages[4] = "";
	infoMessages[5] = "Controls";
	infoMessages[6] = "q\t: quit";
	infoMessages[7] = "h\t: help";
	infoMessages[8] = "   Arrow keys\t: move left/right";
	infoMessages[9] = "Space, z, x, c : shoot bullet    ";
	infoMessages[10] = "";
	infoMessages[11] = "Press a key to play...";
	
	// Draw the outline of the help dialog
	draw_rectangle(x, y, width, height, border_character);

	// Increment so that the strings are below the border
	y++;

	for(int i = 0; i < numStrings; i++) {
		int length = strlen(infoMessages[i]);
		draw_formatted((screen_width() - length) / 2, y + i, infoMessages[i]);
	}

	// Increment by 4 so that y is now equal to the next line break
	y += 4;

	// Draw line break
	draw_line(x, y, x + width - 1, y, border_character);

	// Increment by 6 so that y is now equal to the next line break
	y += 6;

	// Draw line break
	draw_line(x, y, x + width - 1, y, border_character);

	show_screen();

	wait_char();
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

// void draw_diamond(void) {
// 	int seed = time(NULL);
// 	srand(seed);

// 	// Place diamond in random position at top of screen
// 	int diamondX = rand() % (screen_width() - DIAMOND_WIDTH - 2);
// 	diamond = sprite_create(diamondX, 3, DIAMOND_WIDTH, DIAMOND_HEIGHT, diamond_image);

// 	// Give it a downward velocity
// 	sprite_turn_to(diamond, 0, 0.15);
// 	// Choose an angle between 0-16 then move that range to (-8, 8)
// 	int degrees = rand() % 90;
// 	sprite_turn(diamond, (degrees - 45));

// 	sprite_draw(diamond);
// }

void draw_diamonds(void) {
	int seed = time(NULL);
	srand(seed);
	diamondsLeft = MAX_DIAMONDS;

	for (int j = 0; j < MAX_DIAMONDS; j++) {
		// Place diamond in random position at top of screen
		int diamondX = rand() % (screen_width() - DIAMOND_WIDTH - 2);
		diamonds[j] = sprite_create(diamondX, 3, DIAMOND_WIDTH, DIAMOND_HEIGHT, diamond_image);

		// Give it a downward velocity
		sprite_turn_to(diamonds[j], 0, 0.15);
		// Choose an angle between 0-16 then move that range to (-8, 8)
		int degrees = rand() % 90;
		sprite_turn(diamonds[j], (degrees - 45));

		sprite_draw(diamonds[j]);
	}
	
}

void draw_ship(void) {

	ship = sprite_create((screen_width() - SHIP_WIDTH) / 2, screen_height() - 1 - SHIP_HEIGHT, SHIP_WIDTH, SHIP_HEIGHT, ship_image);

	sprite_draw(ship);
}

// Setup game.
void setup(void) {
	// Display the intial help dialog
	draw_help_dialog();

	// Clear the screen ready for the game to start
	clear_screen();

	// Initalise global variables
	lives = 10;
	score = 0;
	timePlayed = 0;
	timeStart = get_current_time();
	diamondsLeft = MAX_DIAMONDS;

	// Draw the border and show the screen
	draw_border();

	draw_ship();

	// draw_diamond();
	draw_diamonds();

	show_screen();

}

// void shootMissile(int x, int y) {
// 	missile = sprite_create(x, y, MISSILE_WIDTH, MISSILE_HEIGHT, missile_image);
// 	sprite_draw(missile);

// 	show_screen();

// 	sprite_turn_to(missile, 0, -0.2);
// 	show_screen();
// }

void shootMissiles(int x, int y) {
	double missileSpeed = -0.2 + diamondsLeft*0.0185;
	for (int i = 0; i < MAX_MISSILES; i++) {
		if(missiles[i] == NULL) {
			missiles[i] = sprite_create(x, y, MISSILE_WIDTH, MISSILE_HEIGHT, missile_image);
			sprite_draw(missiles[i]);
			missileCount++;

			sprite_turn_to(missiles[i], 0, missileSpeed);
			break;
		}
	}
}

bool collided(sprite_id firstSprite, sprite_id secondSprite) {
	// First sprite passed as input
	int first_left		= round(sprite_x(firstSprite));
	int first_right 	= first_left + sprite_width(firstSprite) - 1;
	int first_top 		= round(sprite_y(firstSprite));
	int first_bottom	= first_top + sprite_height(firstSprite) - 1;

	// Second sprite passed as input
	int second_left		= round(sprite_x(secondSprite));
	int second_right 	= second_left + sprite_width(secondSprite) - 1;
	int second_top 		= round(sprite_y(secondSprite));
	int second_bottom	= second_top + sprite_height(secondSprite) - 1;

	if(!(first_right < second_left || first_left > second_right || 
			first_bottom < second_top || first_top > second_bottom)) {
		return true;
	}
	return false;
}

void pauseAndDisplayHelp(void) {
	clear_screen();
	int tempTime = get_current_time();
	draw_help_dialog();
	int elapsedTime = get_current_time() - tempTime;
	timeStart += elapsedTime;
}

// TODO
bool quitGame(void) {
	while(get_char() >= 0) {}
	clear_screen();
	int message_width = strlen(msg_image) / 2;
	sprite_id msg = sprite_create((screen_width() - message_width) / 2, (screen_height() - 2) / 2, message_width, 2, msg_image);
	sprite_draw(msg);
	draw_formatted((screen_width() - 20) / 2, screen_height() - 10, "You scored %d points", score);
	show_screen();
	//game_over = true;
	int key;

	do {
		key = wait_char();
		if(key == 'Y' || key == 'y') {
			return false;
		}

		if(key == 'N' || key == 'n') {
			return true;
		}
	} while(key != 'Y' || key != 'y' || key != 'N' || key != 'n');
 	return false;
}

void resetGame(void) {
	clear_screen();
	cleanup();
	setup();
	show_screen();
}

void calculateTimeElapsed(void) {
	timePlayed = get_current_time() - timeStart;
	minutes = timePlayed / 60;
	seconds = timePlayed % 60;
}

void moveShip(int key) {
	// Get ship's current position
	int ship_x = round(sprite_x(ship));
	int ship_y = round(sprite_y(ship));

	int shipSpeed = 1;
	if(screen_width() > 50) {
		shipSpeed *= 2;
	}

	// If wouldn't be out of bounds and left arrow pressed
	if(key == 260 && ship_x > 1) {
		sprite_move(ship, -shipSpeed, 0); // move left
	}

	// If wouldn't be out of bound and right arrow pressed
	if(key == 261 && ship_x + SHIP_WIDTH < screen_width() - 1) {
		sprite_move(ship, +shipSpeed, 0); // move right
	}

	if((key == ' ' || key == 'x') && canShootMissile()) {
		shootMissiles(ship_x + SHIP_WIDTH / 2, ship_y - 1);
	}

	if(key == 'z' && canShootMissile()) {
		shootMissiles(ship_x, ship_y + 1);
	}

	if(key == 'c' && canShootMissile()) {
		shootMissiles(ship_x + SHIP_WIDTH - 1, ship_y + 1);
	}
}

// void moveDiamond(void) {
// 	// Moving the diamond and keeping in bounds
// 	sprite_step(diamond);

// 	int diamond_x = round(sprite_x(diamond));
// 	int diamond_y = round(sprite_y(diamond));

// 	double diamond_dx = sprite_dx(diamond);
// 	double diamond_dy = sprite_dy(diamond);

// 	if(diamond_x == 0 || diamond_x + DIAMOND_WIDTH == screen_width() - 1) {
// 		diamond_dx = -diamond_dx;
// 	}

// 	if(diamond_y == 2 || diamond_y + DIAMOND_HEIGHT == screen_height()) {
// 		diamond_dy = -diamond_dy;
// 	}

// 	if(diamond_dx != sprite_dx(diamond) || diamond_dy != sprite_dy(diamond)) {
// 		sprite_back(diamond);
// 		sprite_turn_to(diamond, diamond_dx, diamond_dy);
// 	}	
// }

void moveDiamond(int index) {
	// Moving the diamond and keeping in bounds
	sprite_step(diamonds[index]);

	int diamond_x = round(sprite_x(diamonds[index]));
	int diamond_y = round(sprite_y(diamonds[index]));

	double diamond_dx = sprite_dx(diamonds[index]);
	double diamond_dy = sprite_dy(diamonds[index]);

	if(diamond_x == 0 || diamond_x + DIAMOND_WIDTH == screen_width() - 1) {
		diamond_dx = -diamond_dx;
	}

	if(diamond_y == 2 || diamond_y + DIAMOND_HEIGHT == screen_height()) {
		diamond_dy = -diamond_dy;
	}

	if(diamond_dx != sprite_dx(diamonds[index]) || diamond_dy != sprite_dy(diamonds[index])) {
		sprite_back(diamonds[index]);
		sprite_turn_to(diamonds[index], diamond_dx, diamond_dy);
	}	
}

void moveDiamonds() {
	for(int j = 0; j < MAX_DIAMONDS; j++) {
		if(diamonds[j] != NULL) {
			moveDiamond(j);
		}
	}
}

// void moveDiamonds(void) {
// 	for(int i = 0; i < MAX_DIAMONDS; i++) {
// 		if(diamonds[i] != NULL) {
// 			// Moving the diamond and keeping in bounds
// 			sprite_step(diamonds[i]);

// 			int diamond_x = round(sprite_x(diamonds[i]));
// 			int diamond_y = round(sprite_y(diamonds[i]));

// 			double diamond_dx = sprite_dx(diamonds[i]);
// 			double diamond_dy = sprite_dy(diamonds[i]);

// 			if(diamond_x == 0 || diamond_x + DIAMOND_WIDTH == screen_width() - 1) {
// 				diamond_dx = -diamond_dx;
// 			}

// 			if(diamond_y == 2 || diamond_y + DIAMOND_HEIGHT == screen_height()) {
// 				diamond_dy = -diamond_dy;
// 			}

// 			if(diamond_dx != sprite_dx(diamonds[i]) || diamond_dy != sprite_dy(diamonds[i])) {
// 				sprite_back(diamonds[i]);
// 				sprite_turn_to(diamonds[i], diamond_dx, diamond_dy);
// 			}	
// 		}
// 	}
// }

// void moveMissile() {
// 	// int  missile_x = round(sprite_x(missile));
// 	int  missile_y = round(sprite_y(missile));

// 	if(missile_y == 2) {
// 		sprite_destroy(missile);
// 		missile_in_flight = false;
// 	} else if (collided(missile, diamond)) {
// 		sprite_destroy(missile);
// 		missile_in_flight = false;
// 		score++;
// 		sprite_destroy(diamond);
// 		draw_diamond();
// 	} else {
// 		sprite_step(missile);
// 	}
// }

void moveMissiles() {
	for (int i = 0; i < MAX_MISSILES; i++) {
		for(int j = 0; j < MAX_DIAMONDS; j++) {

			if(missiles[i] != NULL && diamonds[j] != NULL) {
				int  missile_y = round(sprite_y(missiles[i]));

				if(missile_y == 2) {
					sprite_back(missiles[i]);
					sprite_draw(missiles[i]);
					sprite_destroy(missiles[i]);
					missiles[i] = NULL;
					missileCount--;

				} else if (collided(missiles[i], diamonds[j])) {

					sprite_destroy(missiles[i]);
					missiles[i] = NULL;
					missileCount--;

					sprite_destroy(diamonds[j]);
					diamonds[j] = NULL;
					diamondsLeft--;

					score++;

					if(diamondsLeft == 0) {
						clear_screen();
						char congratulations[60] = "Congratulations, you cleared all diamonds! +10 points";
						draw_formatted((screen_width() - strlen(congratulations) ) / 2, screen_height()/2, "%s", congratulations);
						score += 10;
						draw_border();
						show_screen();
						timer_pause(1000);
						for(int i = 3; i > 0; i--) {
							clear_screen();
							draw_formatted(screen_width() / 2, screen_height()/2, "%d", i);
							show_screen();
							timer_pause(1000);
						}
						draw_diamonds();
						draw_ship();
					}


				} else {
					sprite_step(missiles[i]);
				}
			}
		}	
	}
}

// bool missileInFlight(void) {
// 	return missile_in_flight;
// }

bool canShootMissile(void) {
	return missileCount < MAX_MISSILES;
}

bool missilesInFlight(void) {
	return missileCount > 0;
}

// Play one turn of game.
void process(void) {
	// Get the next key pressed
	int key = get_char();

	// If they press q then ask if quit game 
	if(key == 'q') {
		if(quitGame()) {
			game_over = true;
		} else {
			resetGame();
		}
		return;
	}

	// If they press h then pause and display help information
	if(key == 'h') {
		pauseAndDisplayHelp();
		return;
	}

	// Work out how long the game has gone for and format
	calculateTimeElapsed();

	// Move the ship left and right depending on input
	moveShip(key);

	// Move the diamond sprite and check collisions
	//moveDiamonds();	

	moveDiamonds();

	// If there is currently a missile in flight
	if(missilesInFlight()) {
		moveMissiles();
	}

	for(int j = 0; j < MAX_DIAMONDS; j++) {
		if(diamonds[j] != NULL) {
			if(collided(ship, diamonds[j])) {
				lives--;
				if(lives <= 0) {
					if(quitGame()) {
						game_over = true;
					} else {
						resetGame();
					}
				} else {
					draw_diamonds();
					draw_ship();
				}
			}
		}
		
	}

	// Remove everything for re-draw
	clear_screen();

	draw_border();

	sprite_draw(ship);

	for(int j = 0; j < MAX_DIAMONDS; j++) {
		if(diamonds[j] != NULL) {
			sprite_draw(diamonds[j]);
		}
	}


	// for(int i = 0; i < MAX_DIAMONDS; i++) {
	// 	if(diamonds[i] != NULL) {
	// 		sprite_draw(diamonds[i]);
	// 	}
	// }

	if(missilesInFlight()) {
		//sprite_draw(missile);
		for(int i = 0; i < MAX_MISSILES; i++) {
			if(missiles[i] != NULL) {
				sprite_draw(missiles[i]);
			}
		}
	}

	// draw_formatted(5, 5, "%d", missileCount);
}

// Clean up game
void cleanup(void) {
	// Reset all variables to initial values
	game_over = false;
	update_screen = true;
	missile_in_flight = false;

	lives = 10;
	score = 0;
	timePlayed = 0;
	timeStart = 0;
	minutes = 0;
	seconds = 0;
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