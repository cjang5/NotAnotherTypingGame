#ifndef LTEXTURE_H
#define LTEXTURE_H
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

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
	bool loadFromFile(SDL_Renderer* renderer, std::string path);

	//load from inputted text
	bool loadFromText(SDL_Renderer* renderer, TTF_Font* font, std::string text, SDL_Color color);

	//set blending mode
	void setBlendingMode(SDL_BlendMode blend);

	//set alpha
	void setAlpha(Uint8 a);

	//set color
	void setColor(Uint8 r, Uint8 g, Uint8 b);

	//render the image
	void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

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

#endif