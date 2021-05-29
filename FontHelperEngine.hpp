#pragma once
#include "ButtonEventCreator.hpp"
#include "ExtraFontInfo.hpp" // todo: rename this file
#include "Bitmap.hpp"

class FontHelperEngine
{
public:
	void Start();
	void Render();
	void Update();
private:
	const char* HEXIT_MAP = "0123456789abcdef";
	const std::string FONT_NAMES[4] = { "Minimal    ", "Fat        ", "Outline    ", "Fixed Width" };
	static const int NUMBER_OF_HEXITS = Bitmap::COLUMNS * 2;
	blit::Font fonts[4] = { blit::minimal_font, blit::fat_font, blit::outline_font, ExtraFontInfo::FIXED_WIDTH_FONT };
	blit::Pen white, black, gray, green;
	enum class FontEnum
	{
		Minimal,
		Fat,
		Outline,
		FixedWidth,
		FirstFont = Minimal,
		LastFont = FixedWidth
	} currentFont;
	int hexits[NUMBER_OF_HEXITS];
	char templateChar = 'A';
	enum class ModeEnum
	{
		DrawMode,
		HexMode,
		HelpMode
	} currentMode = ModeEnum::DrawMode;
	size_t hexitIndex = 0;
	const int CHAR_BITMAP_X = 5, CHAR_BITMAP_Y = 15;
	const int TEMPLATE_CHAR_X = 129, TEMPLATE_CHAR_Y = 0;
	const int CHAR_TYPE_X = 139, CHAR_TYPE_Y = 0;
	const int TEMPLATE_BITMAP_X1 = 45, TEMPLATE_BITMAP_Y1 = 15;
	const int TEMPLATE_BITMAP_X2 = 5, TEMPLATE_BITMAP_Y2 = 65;
	const int PREVIEW_X = 10, PREVIEW_Y = 110;
	const int HEXIT_POINTER_STRING_X = 3, HEXIT_POINTER_STRING_Y = 7;
	const int FIXED_CHAR_HEIGHT = 8, FIXED_CHAR_WIDTH = 7;
	const int SCREEN_WIDTH = 160;
	const bool DRAW_PREVIEW = true;
	const int LINES_PER_PAGE = 13;
	const int NOT_FOUND = -1;
	const int WIDTH_OF_HEXIT_POINTER_STRING = 19;
	int drawRow = 0, drawColumn = 0;
	std::string remainingHelpText;
	Bitmap charBitmap, templateBitmap, clipboardBitmap;
	size_t clipboardHeight = 0, clipboardWidth = 0;
	ButtonEventCreator buttonEvents;
	void setupSDLGraphicsSystem();
	void setupColors();
	const uint8_t* getCharColumns(char charToLookup, FontEnum font);
	std::string charToHex(char charToLookup);
	Bitmap charToBitmap(char charToLookup, FontEnum font);
	void processHexModeEvents(std::vector<Buttons>);
	void processDrawModeEvents(std::vector<Buttons>);
	void processHelpModeEvents(std::vector<Buttons>);
	void openHelpMode();
	void closeHelpMode();
	void incrementValue();
	void decrementValue();
	void nextHexit();
	void prevHexit();
	void swapMode();
	void changeFont();
	void refreshTemplate();
	void refreshHexits();
	void refreshHexitPointer();
	void refreshFont();
	void refreshMode();
	void refreshPreview();
	std::string padding(int amount);
	void incrementHexit();
	void decrementHexit();
	void incrementTemplate();
	void decrementTemplate();
	std::string hexitsToString();
	void print(std::string stringToPrint, int x, int y, FontEnum font = FontEnum::FixedWidth);
	void print(Bitmap bitmap, int x, int y, bool selection = false);
	void drawPixelBackground(int x, int y);
	void drawPixelCursor(int x, int y);
	void updatePixelsFromHexits();
	void copyTemplate();
	void copyPixelsToHexits();
	std::string getCharType(char x);
	void cls();
	void redrawScreen();
	void handleHexModeEvent(Buttons button);
	void handleDrawModeEvent(Buttons button);
	void moveCursorUp();
	void moveCursorDown();
	void moveCursorLeft();
	void moveCursorRight();
	void togglePixel();
	std::string buildHelpText();
	std::string paginateText(std::string text, blit::Font font);
	void writeHelpText();
	void pageDownHelpText();
	std::string buildHexitPointerString();
	void printHexModeInstructions();
	void printDrawModeInstructions();
};