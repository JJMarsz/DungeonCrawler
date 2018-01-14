#include "LTexture.h"


LTexture buttonSpriteSheetTexture;
LTexture mainMenu;
LTexture spriteSheetTexture;
LTexture tileSST;
LTexture charSST;
LTexture townmenu;
LTexture townButtonSST;
LTexture questboard;
LTexture questSST;
LTexture acceptrejectSST;
LTexture questinfo;
LTexture leftText;
LTexture rightText;
LTexture shop;
LTexture training;
LTexture upgrades;
LTexture goldmenu;
LTexture completed;
LTexture questTitle;
LTexture questInfoText;
LTexture questGold;
LTexture questXP;
LTexture questDiff;
LTexture xp0, xp1, xp2;
LTexture acceptText;
LTexture rejectText;

std::vector<SDL_Rect> spriteClips;
std::vector<SDL_Rect> buttonSpriteClips;
std::vector<SDL_Rect> tileSpriteClips;
std::vector<SDL_Rect> charClips;
std::vector<SDL_Rect> townButtonClips;
std::vector<SDL_Rect> questPageClips;
std::vector<SDL_Rect> acceptrejectClips;
std::vector<LTexture*> texts;

/* Texture class defenitions */
LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 150, 200));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor_, int w) {
	//Get rid of preexisting texture
	free();
	if (w <= 0)
		w = 200;
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(gFont, textureText.c_str(), textColor_, w);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mTexture != NULL;
}

void LTexture::setBlendMode(SDL_BlendMode blending) {

}

void LTexture::setAlpha(Uint8 alpha) {

}

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render(int x, int y, SDL_Rect* clip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

