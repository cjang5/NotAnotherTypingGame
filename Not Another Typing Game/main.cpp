//Not Another Typing Game v0.1a
//By: Chris Jang 3/6/14
/*This source code copyrighted by Christopher Jang 2014
and may not be redistributed without written permission.*/

#include <SDL.h>
#include <stdio.h>
#include <string>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdlib.h> //for rand(), srand()
#include <time.h> //for time - rand seed
#include "LTexture.h"

//window dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

////button constants
const int BUTTON_WIDTH = 150;
const int BUTTON_HEIGHT = 50;

//sprites for each respective button state
enum ButtonSprites {
	BUTTON_NORMAL = 0,
	BUTTON_HOVER = 1,
	BUTTON_DOWN = 2,
	BUTTON_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

//LButton class
class LButton {
public:
	//constructor
	LButton();

	//set the (x, y) position of the button
	void setPosition(int x, int y);

	//assign a spritesheet to the button
	void setSpriteSheet(LTexture* sheet);

	//handle different events
	bool handleEvent(SDL_Event* e);

	//render the button at its set position
	void render();

private:
	//(x, y) position
	SDL_Point position;

	//this button's spritesheet
	LTexture* spriteSheet;

	//the current sprite of the button
	ButtonSprites currentSprite;
};

//Monster class
class Monster {
public:
	//constructor
	Monster();

	//TEMP CONSTRUCTOR
	Monster(std::string s);

	//render the monster
	void render();

	//get HP
	int getHP();

	//get monster's name
	std::string getName();

	//deal damage to monster
	void dealDamage(int dmg);

	//is the monster alive?
	bool isAlive();

private:
	//HP
	int HP;

	//name
	std::string name;
	LTexture nameSprite;

	//Monsters sprite
	LTexture sprite;
};

//sdl variables
//main window
SDL_Window* globalWindow = NULL;

//global renderer
SDL_Renderer* globalRenderer = NULL;

//main font (for now)
TTF_Font* globalFont = NULL;
//main font (smaller)
TTF_Font* globalFontSmall = NULL;

//main text to be rendered
LTexture mainText;

//current word to be typed
LTexture currentWordTexture;
std::string currentWord;

//color white
SDL_Color white = { 255, 255, 255 };

//'good job!'
LTexture goodJob;

//Level 1 word bank
std::string level1[] = {"HELLO", "BOY", "WARRIOR", "EDMUND", "MEATBOY", "INDIE", "FEZ", "SPACE", "PIXEL", "TERRAIN", "FUN", "DEVELOP", "ARCADE",
						"BASIC", "RANDOM", "KEY", "AND", "BOOK", "SPELL", "GUN", "RPG", "TWEET", "FLAPPY", "BIRD", "TIME", "WARP", "BUNNY", "MARIO",
						"SUPER", "MEGA", "AWE", "TO", "THE", "WHAT", "NOOB", "OMG"};

//main menu
////the number of buttons on the main menu
const int MAIN_NUM_BUTTONS = 3;

////Sprite clipping Rects
SDL_Rect buttonSpriteClips[BUTTON_SPRITE_TOTAL];

////Spritesheet for main menu buttons
LTexture startButtonSpriteSheet;
LTexture optionsButtonSpriteSheet;
LTexture helpButtonSpriteSheet;

////Main menu button objects
LButton mainButtons[MAIN_NUM_BUTTONS];

//global method forward declarations
////initializer
bool init();
////loads media we will be using
bool loadMedia();
////closes everything
void close();

//LButton class methods
////constructor
LButton::LButton() {
	//initialize button's position
	position.x = 0;
	position.y = 0;

	//initialize its current sprite
	currentSprite = BUTTON_NORMAL;
}

////set the position (x, y)
void LButton::setPosition(int x, int y) {
	position.x = x;
	position.y = y;
}

////set the spritesheet
void LButton::setSpriteSheet(LTexture* sheet) {
	spriteSheet = sheet;
}

////handleEvent
bool LButton::handleEvent(SDL_Event* e) {
	//if a mouse event took place
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {
		//get cursor position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//check if the cursor is inside the button dimensions
		bool inside = true; //initialized to true

		////if cursor is to the left
		if (x < position.x) {
			inside = false;
		}
		////if cursor is to the right
		else if (x > position.x + BUTTON_WIDTH) {
			inside = false;
		}
		////if cursor is above
		else if (y < position.y) {
			inside = false;
		}
		////if cursor is below
		else if (y > position.y + BUTTON_HEIGHT) {
			inside = false;
		}

		//if mouse is outside
		if (!inside) {
			currentSprite = BUTTON_NORMAL;
		}
		//otherwise, if it's outside
		else {
			switch (e->type) {
			case SDL_MOUSEMOTION:
				currentSprite = BUTTON_HOVER;
				break;
			case SDL_MOUSEBUTTONDOWN:
				currentSprite = BUTTON_DOWN;
				break;
			case SDL_MOUSEBUTTONUP:
				currentSprite = BUTTON_UP;
				return true;
				break;
			}
		}
	}

	return false;
}

////render the button's current sprite
void LButton::render() {
	spriteSheet->render(globalRenderer, position.x, position.y, &buttonSpriteClips[currentSprite]);
}

//monster class methods
////constructor
Monster::Monster() {
	//initialize everthang
	name = "Skeleton"; //TEMPORARY
	HP = 20;

	//the LTexture --- later on write a method to assign an image from the bank corresponding to its name i.e: "Wretched Slime" goes with "slime.png" or something
	if (!sprite.loadFromFile(globalRenderer, "assets/red_skeleton.png"))
		printf("Failed to load skeleton sprite!\n");
	if (!nameSprite.loadFromText(globalRenderer, globalFontSmall, name, white))
		printf("Failed to load skeleton's name!\n");
}

////TEMP
Monster::Monster(std::string s) {
	name = s;
	HP = 30;

	if (!sprite.loadFromFile(globalRenderer, "assets/red_skeleton.png"))
		printf("Failed to load red skeleton sprite!\n");
	if (!nameSprite.loadFromText(globalRenderer, globalFontSmall, name, white))
		printf("Failed to load red skeleton's name!\n");
}

////render the monster TODO: Make a spritesheet for each monster, not two separate pngs
void Monster::render() {
	sprite.render(globalRenderer, (SCREEN_WIDTH / 2) - (sprite.getWidth() / 2), 70);
	nameSprite.render(globalRenderer, (SCREEN_WIDTH / 2) - (nameSprite.getWidth() / 2), 40);
}

////get monsters HP
int Monster::getHP() {
	return HP;
}

////deal damage to monster
void Monster::dealDamage(int dmg) {
	HP -= dmg;
}

////determine if the monster is alive or dead
bool Monster::isAlive() {
	if (this->getHP() <= 0) {
		//sprite.loadFromFile("assets/skeleton_dead.png");
		return false;
	}

	return true;
}

////get the monster's name
std::string Monster::getName() {
	return name;
}

//global methods
//initializer
bool init() {
	//initialization flag
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("[init] Failed to initialize SDL!\n");
		success = false;
	}
	else {
		//set texture filtering to 'linear' mode
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("[init] WARNING: Texture filtering is not in 'linear' mode!\n");
		}

		//create global window
		globalWindow = SDL_CreateWindow("Not Another Typing Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (globalWindow == NULL) {
			printf("[init] Failed to create main window!\n");
			success = false;
		}
		else {
			//create renderer
			globalRenderer = SDL_CreateRenderer(globalWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

			if (globalRenderer == NULL) {
				printf("[init] Failed to create global renderer!\n");
				success = false;
			}
			else {
				//set initial draw color
				SDL_SetRenderDrawColor(globalRenderer, 255, 255, 255, 255);

				//initialize SDL_Image (PNG)
				int imgFlags = IMG_INIT_PNG;

				if (!IMG_Init(imgFlags) & imgFlags) {
					printf("[init] Failed to initialize SDL_Image!\n");
					success = false;
				}

				//initialize SDL_ttf
				if (TTF_Init() == -1) {
					printf("[init] Failed to initialize SDL_ttf!\n");
					success = false;
				}
			}
		}
	}

	return success;
}

//load the media that we will be using
bool loadMedia() {
	//initialization flag
	bool success = true;

	//open the font
	globalFont = TTF_OpenFont("fonts/BEBAS.ttf", 48);
	if (globalFont == NULL) {
		printf("[loadMedia] Failed to load 'REVOLUTION' font!\n");
		success = false;
	}
	else {
		//test rendering text
		SDL_Color testColor = { 255, 255, 255 };
		if (!mainText.loadFromText(globalRenderer, globalFont, "REVOLUTION", testColor)) {
			printf("[loadMedia] Test failed for loading text!\n");
			success = false;
		}

		goodJob.loadFromText(globalRenderer, globalFont, "GOOD JOB!", testColor);
	}

	//global font (small)
	globalFontSmall = TTF_OpenFont("fonts/BEBAS.ttf", 24);
	if (globalFontSmall == NULL) {
		printf("[loadMedia] Failed to load 'REVOLUTION' font!\n");
		success = false;
	}
	else {
		//test rendering text
		SDL_Color testColor = { 255, 255, 255 };
		if (!mainText.loadFromText(globalRenderer, globalFontSmall, "REVOLUTION", testColor)) {
			printf("[loadMedia] Test failed for loading text!\n");
			success = false;
		}
	}

	//load main menu button spritesheets
	////start button sprite sheet
	if (!startButtonSpriteSheet.loadFromFile(globalRenderer, "assets/start_button.png")) {
		printf("[loadMedia] Failed to load start button spritesheet!\n");
		success = false;
	}
	
	////options button
	if (!optionsButtonSpriteSheet.loadFromFile(globalRenderer, "assets/options_button.png")) {
		printf("[loadMedia] Failed to load options button spritesheet!\n");
		success = false;
	}

	////help button
	if (!helpButtonSpriteSheet.loadFromFile(globalRenderer, "assets/help_button.png")) {
		printf("[loadMedia] Failed to load help button spritesheet!\n");
		success = false;
	}

	////get Sprite clips
	for (int i = 0; i < BUTTON_SPRITE_TOTAL; i++) {
		buttonSpriteClips[i].x = i * 150;
		buttonSpriteClips[i].y = 0;
		buttonSpriteClips[i].w = BUTTON_WIDTH;
		buttonSpriteClips[i].h = BUTTON_HEIGHT;
	}

	return success;
}

//closing method
void close() {
	//destroy the textures
	mainText.free();
	startButtonSpriteSheet.free();
	optionsButtonSpriteSheet.free();
	helpButtonSpriteSheet.free();

	//close fonts in use
	TTF_CloseFont(globalFont);
	globalFont = NULL;

	//destroy renderer
	SDL_DestroyRenderer(globalRenderer);
	globalRenderer = NULL;

	//destroy global window
	SDL_DestroyWindow(globalWindow);
	globalWindow = NULL;

	//quit
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

//getNewWord get the next word for the user to type
std::string getRandomWord(int level = 1) {
	//initialize the rand seed
	srand(time(NULL));

	if (level == 1) {
		return level1[rand() % (sizeof(level1) / sizeof(level1[0]))];
	}
}

//get a new random monster
Monster getNewMonster() {
	Monster newMonster;
	
	

	return newMonster;
}

//main method
int main(int argc, char* args[]) {
	if (!init()) {
		printf("[main] Failed to initialize!\n");
	}
	else {
		if (!loadMedia()) {
			printf("[main] Failed to load media!\n");
		}
		else {
			//main loop flag
			bool quit = false; 
			bool inMenu = true;

			//SDL Event handler
			SDL_Event e;

			//white color
			SDL_Color white = { 255, 255, 255 };
			//current word that must be typed in order to progress
			currentWord = "WELCOME";
			std::string currentGuess;

			//get a new word?
			bool getNewWord = false;
			bool bGoodJob = false;

			//initialize first word
			currentWordTexture.loadFromText(globalRenderer, globalFont, currentWord, white);

			//initialize menu buttons
			LButton startButton, optionsButton, helpButton;
			////spriteSheets
			startButton.setSpriteSheet(&startButtonSpriteSheet);
			optionsButton.setSpriteSheet(&optionsButtonSpriteSheet);
			helpButton.setSpriteSheet(&helpButtonSpriteSheet);
			////positions
			startButton.setPosition((SCREEN_WIDTH / 2) - (BUTTON_WIDTH / 2), (SCREEN_HEIGHT / 2) + 20);
			optionsButton.setPosition((SCREEN_WIDTH / 2) - (BUTTON_WIDTH / 2), (SCREEN_HEIGHT / 2) + BUTTON_HEIGHT + 28);
			helpButton.setPosition((SCREEN_WIDTH / 2) - (BUTTON_WIDTH / 2), (SCREEN_HEIGHT / 2) + 2 * BUTTON_HEIGHT + 36);

			//current monster
			Monster nextMonster = Monster("red skeleton");
			Monster& currentMonster = nextMonster;

			//main loop
			while (!quit) {
				//if user is in the main menu
				if (inMenu) {
					while (SDL_PollEvent(&e) != 0) {
						//if user clicks x
						if (e.type == SDL_QUIT)
							quit = true;

						if (e.key.keysym.sym == SDLK_ESCAPE)
							quit = true;

						if (e.key.keysym.sym == SDLK_RETURN)
							inMenu = false;

						//handle button events
						if (startButton.handleEvent(&e))
							inMenu = false;
						optionsButton.handleEvent(&e);
						helpButton.handleEvent(&e);
					}

					SDL_RenderClear(globalRenderer);

					//main background
					SDL_Rect backGround = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
					SDL_SetRenderDrawColor(globalRenderer, 80, 90, 100, 255);
					SDL_RenderFillRect(globalRenderer, &backGround);

					//render the buttons
					startButton.render();
					optionsButton.render();
					helpButton.render();

					SDL_RenderPresent(globalRenderer);
				}
				else {
					printf("%i\n", currentMonster.getHP());

					if (!currentMonster.isAlive()) {
						currentMonster = getNewMonster();
					}

					//if current words length is 0 (no word), get a new word
					if (getNewWord) {
						currentWord = getRandomWord(1);
						
						getNewWord = false;
					}

					
					//while there are events on the event queue
					while (SDL_PollEvent(&e) != 0) {
						//if user requests quit
						if (e.type == SDL_QUIT) {
							quit = true;
						}

						//if a key is pressed
						if (e.type == SDL_KEYDOWN) {
							//event based on what key is pressed
							switch (e.key.keysym.sym) {
							case SDLK_ESCAPE:
								quit = true;
								break;
							case SDLK_a:
								currentGuess = "A";
								break;
							case SDLK_b:
								currentGuess = "B";
								break;
							case SDLK_c:
								currentGuess = "C";
								break;
							case SDLK_d:
								currentGuess = "D";
								break;
							case SDLK_e:
								currentGuess = "E";
								break;
							case SDLK_f:
								currentGuess = "F";
								break;
							case SDLK_g:
								currentGuess = "G";
								break;
							case SDLK_h:
								currentGuess = "H";
								break;
							case SDLK_i:
								currentGuess = "I";
								break;
							case SDLK_j:
								currentGuess = "J";
								break;
							case SDLK_k:
								currentGuess = "K";
								break;
							case SDLK_l:
								currentGuess = "L";
								break;
							case SDLK_m:
								currentGuess = "M";
								break;
							case SDLK_n:
								currentGuess = "N";
								break;
							case SDLK_o:
								currentGuess = "O";
								break;
							case SDLK_p:
								currentGuess = "P";
								break;
							case SDLK_q:
								currentGuess = "Q";
								break;
							case SDLK_r:
								currentGuess = "R";
								break;
							case SDLK_s:
								currentGuess = "S";
								break;
							case SDLK_t:
								currentGuess = "T";
								break;
							case SDLK_u:
								currentGuess = "U";
								break;
							case SDLK_v:
								currentGuess = "V";
								break;
							case SDLK_w:
								currentGuess = "W";
								break;
							case SDLK_x:
								currentGuess = "X";
								break;
							case SDLK_y:
								currentGuess = "Y";
								break;
							case SDLK_z:
								currentGuess = "Z";
								break;
							case SDLK_SPACE:
								currentGuess = " ";
								break;
							}
						}
					}

					//draw color
					SDL_SetRenderDrawColor(globalRenderer, 255, 255, 255, 255);

					//clear renderer
					SDL_RenderClear(globalRenderer);

					//main background
					SDL_Rect backGround = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
					SDL_SetRenderDrawColor(globalRenderer, 80, 90, 100, 255);
					SDL_RenderFillRect(globalRenderer, &backGround);
					
					currentMonster.render();

					if (currentWord.length() > 0) {
						if (currentGuess == currentWord.substr(0, 1)) {
							if (currentWord.length() > 1) {
								currentWord = currentWord.substr(1, currentWord.length() - 1);
							}
							else {
								currentWord = " ";
								//bGoodJob = true;
								currentMonster.dealDamage(5);
								getNewWord = true;
							}
						}
					}

					currentWordTexture.loadFromText(globalRenderer, globalFont, currentWord, white);

					/*//if you finished a word, display "good job!" and you have to press spacebar to continue to the next word
					if (bGoodJob) {

						goodJob.render((SCREEN_WIDTH - goodJob.getWidth()) / 2, 340);

						if (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_RETURN) {
							getNewWord = true;
							bGoodJob = false;
						}
					} */

					//render current word
					currentWordTexture.render(globalRenderer, (SCREEN_WIDTH - currentWordTexture.getWidth()) / 2, 340);

					//update screen
					SDL_RenderPresent(globalRenderer);

					//reset current guess
					currentGuess = "";
				}
			}
		}
	}

	close();

	return 0;
}