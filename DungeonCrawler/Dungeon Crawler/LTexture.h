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
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor_, int w);

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Deallocates texture
	void free();

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL);

	//font setter
	void setFont(TTF_Font* font_);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture * mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;

	TTF_Font* font;
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
extern LTexture questinfo;
extern LTexture acceptrejectSST;
extern LTexture shop;
extern LTexture training;
extern LTexture upgrades;
extern LTexture dungeonmenu;
extern LTexture healthboxSST;
extern LTexture dungeonButtonSST;
extern LTexture dungText1, dungText2, dungText3;
extern LTexture newgametext, loadgametext, tutorialtext, creditstext;
extern LTexture messageBox;
extern LTexture multiplierSST;
extern LTexture choicemenu;
extern LTexture choiceButtonSST;
extern LTexture no;
extern LTexture dungeonroom;
extern LTexture roomTilesSST;
extern LTexture background;
extern LTexture endTurn;
extern LTexture endTurnText;
extern LTexture mobSST;
extern LTexture mobNum;
extern LTexture abIconSST;

extern std::vector<SDL_Rect> spriteClips;
extern std::vector<SDL_Rect> tileSpriteClips;
extern std::vector<SDL_Rect> charClips;
extern std::vector<SDL_Rect> healthBoxClips;
extern std::vector<SDL_Rect> multiplyClips;
extern std::vector<SDL_Rect> roomTileClips;
extern std::vector<SDL_Rect> mobNumClips;
extern std::vector<SDL_Rect> mobClips;
extern std::vector<SDL_Rect> abClips;

extern std::vector<LTexture*> texts;

/* Texts */
extern LTexture leftText;
extern LTexture rightText;
extern LTexture goldmenu;
extern LTexture xp0, xp1, xp2;
extern LTexture completed;
extern LTexture questTitle;
extern LTexture questInfoText;
extern LTexture questGold;
extern LTexture questXP;
extern LTexture questDiff;
extern LTexture acceptText;
extern LTexture rejectText;
extern LTexture hp0, hp1, hp2;
#endif

