#ifndef LTEXTURE_H
#define LTEXTURE_H

#include "lib.h"
#include "SDL.h"


class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

	//Deallocates texture
	void free();

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture * mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};


//Rendered texts
//extern LTexture buttonOneText;
//extern LTexture buttonTwoText;
//extern LTexture buttonThreeText;

//Character spritesheet along with clips
extern LTexture spriteSheetTexture;

//button spritesheet along with clips
extern LTexture buttonSpriteSheetTexture;
extern LTexture mainMenu;
extern LTexture tileSST;
extern LTexture charSST;
extern LTexture townmenu;
extern LTexture townButtonSST;
extern LTexture questboard;
extern LTexture questSST;

extern std::vector<SDL_Rect> spriteClips;
extern std::vector<SDL_Rect> buttonSpriteClips;
extern std::vector<SDL_Rect> tileSpriteClips;
extern std::vector<SDL_Rect> charClips;
extern std::vector<SDL_Rect> questPageClips;
extern std::vector<SDL_Rect> townButtonClips;

extern std::vector<LTexture*> texts;

#endif

