#ifndef LBUTTON_H
#define LBUTTON_H
#include "LTexture.h"

//sprites for each respective button state
enum ButtonSprites {
	BUTTON_NORMAL,
	BUTTON_HOVER,
	BUTTON_DOWN,
	BUTTON_UP,
	BUTTON_SPRITE_TOTAL 
};

//TEMP VARIABLES
////button constants
const int BUTTON_WIDTH = 150;
const int BUTTON_HEIGHT = 50;

////Spritesheet for main menu buttons
LTexture startButtonSpriteSheet;
LTexture optionsButtonSpriteSheet;
LTexture helpButtonSpriteSheet;

//return the number of buttons there are
int numButtons();

//load the media for LButton
bool LoadLButtonMedia(SDL_Renderer* renderer);

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
	void render(SDL_Renderer* renderer);

	//get width/height
	int getWidth();
	int getHeight();

private:
	//(x, y) position
	SDL_Point position;

	int buttonWidth;
	int buttonHeight;

	//this button's spritesheet
	LTexture* spriteSheet;

	//the current sprite of the button
	ButtonSprites currentSprite;
};

#endif