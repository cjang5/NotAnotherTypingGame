#include "LTexture.h"
#include <SDL_image.h>

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
bool LTexture::loadFromFile(SDL_Renderer* renderer, std::string path) {
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
		finalTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

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
bool LTexture::loadFromText(SDL_Renderer* renderer, TTF_Font* font, std::string text, SDL_Color color) {
	//free any pre-existing hardTexture
	free();

	//final texture that will be assigned to 'hardTexture
	SDL_Texture* finalTexture = NULL;

	//the image created from the text that is inputted
	SDL_Surface* loadedText = TTF_RenderText_Solid(font, text.c_str(), color);

	if (loadedText == NULL) {
		printf("[LTexture::loadFromText] Failed to load text into image!\n");
	}
	else {
		//create image from the text
		finalTexture = SDL_CreateTextureFromSurface(renderer, loadedText);

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
void LTexture::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	//set render space
	SDL_Rect renderSpace = { x, y, imageWidth, imageHeight };

	//if clipping, then reset the render space
	if (clip != NULL) {
		renderSpace.w = clip->w;
		renderSpace.h = clip->h;
	}

	//render the image
	SDL_RenderCopyEx(renderer, hardTexture, clip, &renderSpace, angle, center, flip);
}

//get image width
int LTexture::getWidth() {
	return imageWidth;
}

//get image height
int LTexture::getHeight() {
	return imageHeight;
}