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

//Monster class
class Monster {
public:
	//default constructor
	Monster();

	//constructor; name
	Monster(std::string s);

	//initialize
	void initialize(SDL_Renderer* renderer);

	//enum for monster states
	enum MonsterStates {
		ALIVE,
		HIT,
		DEAD, 
		TOTAL
	};

	//render the monster
	void render(SDL_Renderer* renderer);

	//get HP
	int getHP();

	//get monster's name
	std::string getName();

	//set texture
	//void setTexture(SDL_Renderer* renderer, std::string path);

	//deal damage to monster
	void dealDamage(int dmg);

	//change monsters state
	void changeState(int m);

	//handle events
	void handleEvent();

	//is the monster alive?
	bool isAlive();

private:
	//HP
	int HP;

	//name
	std::string name;
	LTexture nameSprite;

	//Monsters sprite
	LTexture spriteSheet;

	MonsterStates state;

	SDL_Rect clip[TOTAL];
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

//global method forward declarations
////initializer
bool init();
////loads media we will be using
bool loadMedia();
////closes everything
void close();

//monster class methods
////default constructor
Monster::Monster() {
	//initialize everthang
	name = "Skeleton"; //TEMPORARY
	HP = 20;

	state = ALIVE;
	//the LTexture --- later on write a method to assign an image from the bank corresponding to its name i.e: "Wretched Slime" goes with "slime.png" or something
}

////TEMP
Monster::Monster(std::string s) {
	name = s;
	HP = 30;

	state = ALIVE;
}

//initialize
void Monster::initialize(SDL_Renderer* renderer) {
	if (name == "red skeleton") {
		spriteSheet.loadFromFile(renderer, "assets/red_skeleton.png");
		nameSprite.loadFromText(renderer, globalFontSmall, name, white);
		for (int i = 0; i < TOTAL; i++) {
			clip[i].x = i * 200;
			clip[i].y = 0;
			clip[i].w = 200;
			clip[i].h = 250;
		}
	}
	else if (name == "boring skeleton") {
		spriteSheet.loadFromFile(renderer, "assets/white_skeleton.png");
		nameSprite.loadFromText(renderer, globalFontSmall, name, white);
		for (int i = 0; i < TOTAL; i++) {
			clip[i].x = i * 200;
			clip[i].y = 0;
			clip[i].w = 200;
			clip[i].h = 250;
		}
	}
	else if (name == "green skeleton") {
		spriteSheet.loadFromFile(renderer, "assets/green_skeleton.png");
		nameSprite.loadFromText(renderer, globalFontSmall, name, white);
		for (int i = 0; i < TOTAL; i++) {
			clip[i].x = i * 200;
			clip[i].y = 0;
			clip[i].w = 200;
			clip[i].h = 250;
		}
	}
}

////render the monster TODO: Make a spritesheet for each monster, not two separate pngs
void Monster::render(SDL_Renderer* renderer) {
	spriteSheet.render(renderer, (SCREEN_WIDTH / 2) - (spriteSheet.getWidth() / 6), 70, &clip[state]);
	nameSprite.render(renderer, (SCREEN_WIDTH / 2) - (nameSprite.getWidth() / 2), 40);
}

////get monsters HP
int Monster::getHP() {
	return HP;
}

////deal damage to monster
void Monster::dealDamage(int dmg) {
	HP -= dmg;
	state = HIT;
}

////change monster's state
void Monster::changeState(int m) {
	switch (m) {
	case 0:
		state = ALIVE;
		break;
	case 1:
		state = HIT;
		break;
	case 2:
		state = DEAD;
		break;
	default:
		state = ALIVE;
		break;
	}
}

////determine if the monster is alive or dead
bool Monster::isAlive() {
	if (HP <= 0) {
		return false;
	}

	return true;
}

////handle event
void Monster::handleEvent() {
	if (!isAlive()) {
		//sprite.loadFromFile(globalRenderer, "assets/skeleton_dead.png");
		state = DEAD;
	}
	else {
		state = ALIVE;
	}
}

////set texture
/*void Monster::setTexture(SDL_Renderer* renderer, std::string path) {
	sprite.loadFromFile(renderer, path.c_str());
	nameSprite.loadFromText(renderer, globalFontSmall, name, white);
}*/

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

	return success;
}

//closing method
void close() {
	//destroy the textures
	mainText.free();

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
Monster getNewMonster(int n) {
	Monster newMonster;

	if (n == 1)
		newMonster = Monster("boring skeleton");
	else if (n == 2)
		newMonster = Monster("red skeleton");
	else
		newMonster = Monster("green skeleton");

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

			int n;
			srand(time(NULL));
			Monster monsters[5];
			for (int i = 0; i < 5; i++) {
				n = rand() % 3;
				monsters[i] = getNewMonster(n);
				monsters[i].initialize(globalRenderer);
			}

			//current ticks
			Uint32 currentTicks = 0;
			int hurtFrame = 0;

			int current = 0;
			//current monster
			Monster *currentMonster = &monsters[current];

			bool getNew = false;
			//TEMP
			printf("%i\n", currentMonster->getHP());
			//main loop
			while (!quit) {
				if (getNew) {
					currentMonster = &monsters[current];
					getNew = false;
				}
				//if current words length is 0 (no word), get a new word
				if (getNewWord) {
					currentMonster->handleEvent();
					printf("%i\n", currentMonster->getHP());
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

				if (currentWord.length() > 0) {
					//printf("H"); 
					if (currentGuess == currentWord.substr(0, 1)) {
						if (currentWord.length() > 1) {
							currentWord = currentWord.substr(1, currentWord.length() - 1);
						}
						else {

							//get next monster
							if (!currentMonster->isAlive()) {
								current++;
								if (current > 4)
									quit = true;
								getNew = true;
							}

							currentWord = "";
							//bGoodJob = true;
							currentMonster->dealDamage(5);

							currentTicks = SDL_GetTicks();
						}
					}
				}
				else {
					//hurt animation
					switch (hurtFrame) {
					case 0:
						if (SDL_GetTicks() - currentTicks > 50) {
							currentMonster->changeState(0);
							currentTicks = SDL_GetTicks();
							hurtFrame++;
						}
						break;
					case 1:
						if (SDL_GetTicks() - currentTicks > 50) {
							currentMonster->changeState(1);
							currentTicks = SDL_GetTicks();
							hurtFrame++;
						}
						break;
					case 2:
						if (SDL_GetTicks() - currentTicks > 50) {
							currentMonster->changeState(0);
							currentTicks = SDL_GetTicks();
							hurtFrame++;
						}
						break;
					}	

					//if hurt animation is over
					if (hurtFrame == 3) {
						hurtFrame = 0;
						getNewWord = true;
					}
				}

				currentMonster->render(globalRenderer);

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
	
	close();

	return 0;
}