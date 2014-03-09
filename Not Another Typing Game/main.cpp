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

//window dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//LTexture class **TODO: Move to a separate .cpp file
class LTexture {
public:
	//constructor
	LTexture();

	//destructor
	~LTexture();

	//deallocator
	void free();

	//load image from file
	bool loadFromFile(std::string path);

	//load from inputted text
	bool loadFromText(std::string text, SDL_Color color);

	//set blending mode
	void setBlendingMode(SDL_BlendMode blend);

	//set alpha
	void setAlpha(Uint8 a);

	//set color
	void setColor(Uint8 r, Uint8 g, Uint8 b);

	//render the image
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//get image width
	int getWidth();

	//get image height
	int getHeight();

private:
	//actual image texture
	SDL_Texture* hardTexture;

	//image width
	int imageWidth;

	//image height
	int imageHeight;
};

//sprites for each respective button state
enum ButtonSprites {
	BUTTON_NORMAL,
	BUTTON_HOVER,
	BUTTON_DOWN,
	BUTTON_UP,
	BUTTON_SPRITE_TOTAL
};

//LButton class
class LButton {
public:
	//constructor
	LButton();

	//set the (x, y) position of the button
	void setPosition(int x, int y);

	//handle different events
	void handleEvent(SDL_Event* e);

	//render the button at its set position
	void render();

private:
	//(x, y) position
	SDL_Point position;

	//this button's spritesheet
	LTexture spriteSheet;

	//the current sprite of the button
	ButtonSprites currentSprite;
};

//sdl variables
//main window
SDL_Window* globalWindow = NULL;

//global renderer
SDL_Renderer* globalRenderer = NULL;

//main font (for now)
TTF_Font* globalFont = NULL;

//main text to be rendered
LTexture mainText;

//current word to be typed
LTexture currentWordTexture;
std::string currentWord;

//'good job!'
LTexture goodJob;

//Level 1 word bank
std::string level1[] = {"HELLO", "BOY", "WARRIOR", "EDMUND", "MEATBOY", "INDIE", "FEZ", "SPACE", "PIXEL", "TERRAIN", "FUN", "DEVELOP", "ARCADE",
						"BASIC", "RANDOM", "KEY", "AND", "BOOK", "SPELL", "GUN", "RPG", "TWEET", "FLAPPY", "BIRD", "TIME", "WARP", "BUNNY", "MARIO",
						"SUPER", "MEGA", "AWE", "TO", "THE", "WHAT", "NOOB", "OMG"};

//main menu
////the number of buttons on the main menu
const int MAIN_NUM_BUTTONS = 3;

////button constants
const int BUTTON_WIDTH = 150;
const int BUTTON_HEIGHT = 50;

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

//LTexture class methods
//constructor
LTexture::LTexture() {
	//initialize everything
	hardTexture = NULL;
	imageWidth = 0;
	imageHeight = 0;
}

//destructor
LTexture::~LTexture() {
	//deallocate everything
	free();
}

//deallocator
void LTexture::free() {
	//if the hardTexture exists, destroy it
	if (hardTexture != NULL) {
		SDL_DestroyTexture(hardTexture);
		hardTexture = NULL;

		//reinitialize dimensions
		imageWidth = 0;
		imageHeight = 0;
	}
}

//load an image from a file path
bool LTexture::loadFromFile(std::string path) {
	//free any existing textures
	free();

	//final texture that will be assigned to 'hardTexture'
	SDL_Texture* finalTexture = NULL;

	//where the image will come from
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == NULL) {
		printf("[LTexture::loadFromFile] Failed to load surface!\n");
	}
	else {
		//color key the image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 0, 255)); //colorkey color: magenta?

		//create texture from the surface
		finalTexture = SDL_CreateTextureFromSurface(globalRenderer, loadedSurface);

		if (finalTexture == NULL) {
			printf("[LTexture::loadFromFile] Failed to create texture from surface!\n");
		}
		else {
			//get image's dimensions
			imageWidth = loadedSurface->w;
			imageHeight = loadedSurface->h;
		}
	}

	//assign the new hardTexture
	hardTexture = finalTexture;

	//return if the method was successful
	return hardTexture != NULL;
}

//load image from typed text
bool LTexture::loadFromText(std::string text, SDL_Color color) {
	//free any pre-existing hardTexture
	free();

	//final texture that will be assigned to 'hardTexture
	SDL_Texture* finalTexture = NULL;

	//the image created from the text that is inputted
	SDL_Surface* loadedText = TTF_RenderText_Solid(globalFont, text.c_str(), color);

	if (loadedText == NULL) {
		printf("[LTexture::loadFromText] Failed to load text into image!\n");
	}
	else {
		//create image from the text
		finalTexture = SDL_CreateTextureFromSurface(globalRenderer, loadedText);

		if (finalTexture == NULL) {
			printf("[LTexture::loadFromText] Failed to create texture from given text!\n");
		}
		else {
			//get image dimensions
			imageWidth = loadedText->w;
			imageHeight = loadedText->h;
		}
	}

	//assign the text image to hardTexture
	hardTexture = finalTexture;

	//return if loading the text was successful
	return hardTexture != NULL;
}

//set the blending mode of the texture
void LTexture::setBlendingMode(SDL_BlendMode blend) {
	//set the blending mode
	SDL_SetTextureBlendMode(hardTexture, blend);
}

//set the alpha of the hardTexture
void LTexture::setAlpha(Uint8 a) {
	//set the alpha
	SDL_SetTextureAlphaMod(hardTexture, a);
}

//set the color of the hardTexture
void LTexture::setColor(Uint8 r, Uint8 g, Uint8 b) {
	//set the color
	SDL_SetTextureColorMod(hardTexture, r, g, b);
}

//render method - given coordinates, angle, point, flip status, clipping dimensions
void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	//set render space
	SDL_Rect renderSpace = { x, y, imageWidth, imageHeight };

	//if clipping, then reset the render space
	if (clip != NULL) {
		renderSpace.w = clip->w;
		renderSpace.h = clip->h;
	}

	//render the image
	SDL_RenderCopyEx(globalRenderer, hardTexture, clip, &renderSpace, angle, center, flip);
}

//get image width
int LTexture::getWidth() {
	return imageWidth;
}

//get image height
int LTexture::getHeight() {
	return imageHeight;
}

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

void LButton::handleEvent(SDL_Event* e) {
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
				break;
			}
		}
	}
}

////render the button's current sprite
void LButton::render() {
	spriteSheet.render(position.x, position.y, &buttonSpriteClips[currentSprite]);
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
		if (!mainText.loadFromText("REVOLUTION", testColor)) {
			printf("[loadMedia] Test failed for loading text!\n");
			success = false;
		}

		goodJob.loadFromText("GOOD JOB!", testColor);
	}

	//load main menu button spritesheets

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
			//TODO: MAKE QUIT = true 
			//main loop flag
			bool quit = false; //true so it wont run while you're in the menu

			//Menu is displayed before the main game loop executes
			

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
			currentWordTexture.loadFromText(currentWord, white);

			//main loop
			while (!quit) {
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
				SDL_SetRenderDrawColor(globalRenderer, 77, 89, 98, 255);
				SDL_RenderFillRect(globalRenderer, &backGround);

				if (currentWord.length() > 0) {
					if (currentGuess == currentWord.substr(0, 1)) {
						if (currentWord.length() > 1) {
							currentWord = currentWord.substr(1, currentWord.length() - 1);
						}
						else {
							currentWord = " ";
							bGoodJob = true;
						}
					}
				}

				currentWordTexture.loadFromText(currentWord, white);

				//if you finished a word, display "good job!" and you have to press spacebar to continue to the next word
				if (bGoodJob) {
					goodJob.render((SCREEN_WIDTH - goodJob.getWidth()) / 2, (SCREEN_HEIGHT - goodJob.getHeight()) / 2);

					if (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_RETURN) {
						getNewWord = true;
						bGoodJob = false;
					}
				}

				//render current word
				currentWordTexture.render((SCREEN_WIDTH - currentWordTexture.getWidth()) / 2, (SCREEN_HEIGHT - currentWordTexture.getHeight()) / 2);

				/*//test for timer bar
				SDL_SetRenderDrawColor(globalRenderer, 255, 0, 0, 255);
				SDL_Rect timer = { 0, 0, 150, 30 };
				SDL_RenderFillRect(globalRenderer, &timer); */

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