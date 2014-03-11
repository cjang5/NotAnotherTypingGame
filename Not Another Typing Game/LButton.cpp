#include "LButton.h"

//numButton
int numButtons() {
	return BUTTON_SPRITE_TOTAL;
}

////Sprite clipping Rects
SDL_Rect buttonSpriteClips[BUTTON_SPRITE_TOTAL];

//load LButton media
bool loadLButtonMedia(SDL_Renderer* renderer) {
	//initialization flag
	bool success = true;

	//load main menu button spritesheets
	////start button sprite sheet
	if (!startButtonSpriteSheet.loadFromFile(renderer, "assets/start_button.png")) {
		printf("[loadMedia] Failed to load start button spritesheet!\n");
		success = false;
	}

	////options button
	if (!optionsButtonSpriteSheet.loadFromFile(renderer, "assets/options_button.png")) {
		printf("[loadMedia] Failed to load options button spritesheet!\n");
		success = false;
	}

	////help button
	if (!helpButtonSpriteSheet.loadFromFile(renderer, "assets/help_button.png")) {
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
void LButton::render(SDL_Renderer* renderer) {
	spriteSheet->render(renderer, position.x, position.y, &buttonSpriteClips[currentSprite]);
}