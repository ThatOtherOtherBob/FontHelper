#include "FontHelperEngine.hpp"
#include "ExtraFontInfo.hpp"
#include "32blit.hpp"

void FontHelperEngine::Start()
{
    setupSDLGraphicsSystem();

    setupColors();

    cls();

    refreshHexitPointer();
    refreshTemplate();
    copyTemplate();
    refreshFont();
    refreshMode();
    refreshPreview();
}

void FontHelperEngine::cls()
{
    blit::screen.pen = black;
    blit::screen.clear();
}

void FontHelperEngine::setupSDLGraphicsSystem()
{
    blit::set_screen_mode(blit::ScreenMode::lores);
    blit::screen.alpha = 255;
    blit::screen.mask = nullptr;
}

void FontHelperEngine::setupColors()
{
    white = blit::Pen(255, 255, 255);
    gray = blit::Pen(60, 60, 60);
    black = blit::Pen(0, 0, 0);
    green = blit::Pen(0, 255, 0);
}

void FontHelperEngine::Render()
{
}

void FontHelperEngine::Update()
{
    std::vector<Buttons> events = buttonEvents.GenerateEvents(blit::buttons);

    if (currentMode == ModeEnum::HexMode)
        processHexModeEvents(events);
    else if (currentMode == ModeEnum::DrawMode)
        processDrawModeEvents(events);
    else if (currentMode == ModeEnum::HelpMode)
        processHelpModeEvents(events);
}

// todo: add selection to drag, add copy/paste?
void FontHelperEngine::processHexModeEvents(std::vector<Buttons> events)
{
    for (Buttons event : events)
        handleHexModeEvent(event);
}

void FontHelperEngine::handleHexModeEvent(Buttons button)
{
    if (button == Buttons::Up)
        incrementValue();
    else if (button == Buttons::Down)
        decrementValue();
    else if (button == Buttons::Left)
        prevHexit();
    else if (button == Buttons::Right)
        nextHexit();
    else if (button == Buttons::A)
        changeFont();
    else if (button == Buttons::B)
        swapMode();
    else if (button == Buttons::X || button == Buttons::Y)
        openHelpMode();
    else if (button == Buttons::Stick)
        copyTemplate();
}

void FontHelperEngine::processDrawModeEvents(std::vector<Buttons> events)
{
    for (Buttons event : events)
        handleDrawModeEvent(event);
}

void FontHelperEngine::handleDrawModeEvent(Buttons button)
{
    if (button == Buttons::Up)
        moveCursorUp();
    else if (button == Buttons::Down)
        moveCursorDown();
    else if (button == Buttons::Left)
        moveCursorLeft();
    else if (button == Buttons::Right)
        moveCursorRight();
    else if (button == Buttons::A)
        togglePixel();
    else if (button == Buttons::B)
        swapMode();
    else if (button == Buttons::X || button == Buttons::Y)
        openHelpMode();
    else if (button == Buttons::Stick)
        copyTemplate();
}

void FontHelperEngine::moveCursorUp()
{
    if (drawRow == 0)
        return;

    drawRow--;

    print(charBitmap, CHAR_BITMAP_X, CHAR_BITMAP_Y, DRAW_PREVIEW);
}

void FontHelperEngine::moveCursorDown()
{
    if (drawRow + 1 == Bitmap::ROWS)
        return;

    drawRow++;
    
    print(charBitmap, CHAR_BITMAP_X, CHAR_BITMAP_Y, DRAW_PREVIEW);
}

void FontHelperEngine::moveCursorLeft()
{
    if (drawColumn == 0)
        return;

    drawColumn--;
    print(charBitmap, CHAR_BITMAP_X, CHAR_BITMAP_Y, DRAW_PREVIEW);
}

void FontHelperEngine::moveCursorRight()
{
    if (drawColumn + 1 == Bitmap::COLUMNS)
        return;

    drawColumn++;
    print(charBitmap, CHAR_BITMAP_X, CHAR_BITMAP_Y, DRAW_PREVIEW);
}

void FontHelperEngine::togglePixel()
{
    charBitmap.Pixels[drawRow][drawColumn] = !charBitmap.Pixels[drawRow][drawColumn];
    copyPixelsToHexits();
    refreshPreview();
}

void FontHelperEngine::refreshPreview()
{
    for (int r = 0; r < Bitmap::ROWS; r++)
        for (int c = 0; c < Bitmap::COLUMNS; c++)
        {
            blit::screen.pen = charBitmap.Pixels[r][c] ? white : black;
            blit::screen.pixel(blit::Point(c + PREVIEW_X, r + PREVIEW_Y));
        }
}

void FontHelperEngine::openHelpMode()
{
    currentMode = ModeEnum::HelpMode;

    remainingHelpText = paginateText(buildHelpText(), ExtraFontInfo::FIXED_WIDTH_FONT);

    writeHelpText();
}

void FontHelperEngine::closeHelpMode()
{
    cls();

    currentMode = ModeEnum::DrawMode;

    refreshFont();
    refreshHexits();
    refreshMode();
    refreshTemplate();
    refreshPreview();
}

std::string FontHelperEngine::buildHelpText()
{
    std::string text = "Font Helper v1.0.0 by LBOB\n\n";

    text += "To print text in C++, use \"blit::screen.text(...)\". ";
    text += "Available fonts are: Minimal, Fat and Outline. ";
    text += "These are stored in \"32blit-sdk-master/32blit/graphics/font.cpp\". ";
    text += "This program also uses a 4th, custom font called \"FixedWidth\". ";
    text += "FixedWidth exists to add lower-case letters to the minimal font, tweak positioning and make certain characters more distinct. ";
    text += "(e.g. #:1, L:l, U:I and S:|) ";
    text += "This is not a built-in font, and is mostly ripped off from the minimal font. ";
    text += "Each character is saved as a series of 6 bytes, shown as nybbles/hexits. (e.g. \"{0x00,0x00,0x2e,0x00,0x00,0x00}, // !\") ";
    text += "This program lets you determine what to set these bytes to.\n\n";
    text += "Note: fonts only represent characters from space/dec32/hex20 to ~/dec126/hex7e.\n\n";

    text += "The upper-left grid is the edit window. ";
    text += "The right and bottom grids are the template character. ";
    text += "Below the bottom grid is a preview of the edit window. ";
    text += "Above the edit window is the list of nybbles representing what you've drawn. ";
    text += "To the right of the nybbles is the template character shown normal size. ";
    text += "To the right of the template is a symbol indicating where in the character set you are.  (U)ppercase, (L)owercase, (S)ymbol/Space or (#)Number.\n";
    text += "Note: some fonts repeat the upper-case letters in the lower-case area.\n\n";

    text += "You start in pixel mode.  ";
    text += "Move with the d-pad. ";
    text += "Toggle pixels with A. ";
    text += "Swap modes with B. ";
    text += "Press in the stick to copy the template into the edit window.\n\n";

    text += "When you are happy with the letter:\n";
    text += "1. Make a copy of an existing font's data.  (i.e. \"minimal_font_data\", \"minimal_font_width\" and \"minimal_font\")\n";
    text += "2. Replace the letter of your choice in *_font_data with the bytes at the top of the screen.\n";
    text += "3. You may need to edit the *_font_width array.\n\n";

    text += "In hex mode, you can use up/down to change the bytes and left/right to move to a different nybble/hexit. ";
    text += "Use up/down while the template is selected to move to the next/previous character. ";
    text += "Use A to swap which font's characters are being used as templates. ";
    text += "Swap modes with B. ";
    text += "Press in the stick to copy the template into the edit window. ";

    return text;
}

std::string FontHelperEngine::paginateText(std::string text, blit::Font font)
{
    const bool FIXED_WIDTH = false;
    const bool SPLIT_BETWEEN_WORDS = true;

    return blit::screen.sprites->wrap_text(text, SCREEN_WIDTH, font, FIXED_WIDTH, SPLIT_BETWEEN_WORDS);
}

void FontHelperEngine::writeHelpText()
{
    const bool FIXED_WIDTH = false;

    cls();

    blit::screen.pen = white;
    blit::screen.text(remainingHelpText, fonts[(size_t)FontEnum::FixedWidth], blit::Point(0, 0), FIXED_WIDTH);
}

void FontHelperEngine::processHelpModeEvents(std::vector<Buttons> events)
{
    if (events.size() == 0)
        return;
  
    pageDownHelpText();

    if (remainingHelpText.size() == 0)
        closeHelpMode();
    else
        writeHelpText();
}

void FontHelperEngine::pageDownHelpText()
{
    int index = -1;

    for (int c = 0; c < LINES_PER_PAGE; c++)
    {
        index = remainingHelpText.find("\n", index + 1);
        if (index == NOT_FOUND)
        {
            remainingHelpText = "";
            return;
        }
    }

    remainingHelpText = remainingHelpText.substr(index + 1);
}

void FontHelperEngine::changeFont()
{
    if (currentFont == FontEnum::LastFont)
        currentFont = FontEnum::FirstFont;
    else
        currentFont = (FontEnum)((int)currentFont + 1);

    refreshFont();
}

void FontHelperEngine::refreshFont()
{
    print(FONT_NAMES[(int)currentFont], 80, 30);

    templateBitmap = charToBitmap(templateChar, currentFont);

    refreshTemplate();
}

void FontHelperEngine::refreshTemplate()
{
    std::string toPrint(1, templateChar);
    
    print(toPrint, TEMPLATE_CHAR_X, TEMPLATE_CHAR_Y, currentFont);

    print("(" + getCharType(templateChar) + ")", CHAR_TYPE_X, CHAR_TYPE_Y, FontEnum::FixedWidth);

    const uint8_t* columns = getCharColumns(templateChar, currentFont);

    for (int c = 0; c < Bitmap::COLUMNS; c++)
        templateBitmap.UpdateColumn(c, columns[c]);

    print(templateBitmap, TEMPLATE_BITMAP_X1, TEMPLATE_BITMAP_Y1);
    print(templateBitmap, TEMPLATE_BITMAP_X2, TEMPLATE_BITMAP_Y2);
}

std::string FontHelperEngine::getCharType(char x)
{
    if (x >= 'A' && x <= 'Z')
        return "U";
    
    if (x >= 'a' && x <= 'z')
        return "L";

    if (x >= '0' && x <= '9')
        return "#";

    return "S";
}

void FontHelperEngine::swapMode()
{
    if (currentMode == ModeEnum::DrawMode)
        currentMode = ModeEnum::HexMode;
    else if (currentMode == ModeEnum::HexMode || currentMode == ModeEnum::HelpMode)
        currentMode = ModeEnum::DrawMode;

    refreshHexits();
    refreshHexitPointer();
    refreshMode();
}

const uint8_t* FontHelperEngine::getCharColumns(char charToLookup, FontEnum font)
{
    const size_t SPACE = 32;
    size_t charNumber = (size_t)charToLookup - SPACE;

    if (font == FontEnum::Minimal)
        return ExtraFontInfo::MINIMAL_FONT_DATA[charNumber];
    else if (font == FontEnum::Fat)
        return ExtraFontInfo::FAT_FONT_DATA[charNumber];
    else if (font == FontEnum::Outline)
        return ExtraFontInfo::OUTLINE_FONT_DATA[charNumber];
    else if (font == FontEnum::FixedWidth)
        return ExtraFontInfo::FIXED_WIDTH_FONT_DATA[charNumber];
}

std::string FontHelperEngine::charToHex(char charToLookup)
{
    std::string hex = "..";

    hex[0] = HEXIT_MAP[charToLookup >> 4];
    hex[1] = HEXIT_MAP[charToLookup % 16];

    return hex;
}

Bitmap FontHelperEngine::charToBitmap(char charToLookup, FontEnum font)
{
    const uint8_t* columns = getCharColumns(charToLookup, font);
    Bitmap bitmap;

    for (int c = 0; c < 6; c++)
        bitmap.UpdateColumn(c, columns[c]);
    
    return bitmap;
}

void FontHelperEngine::incrementValue()
{
    if (hexitIndex < NUMBER_OF_HEXITS)
        incrementHexit();
    else
        incrementTemplate();
}

void FontHelperEngine::decrementValue()
{
    if (hexitIndex < NUMBER_OF_HEXITS)
        decrementHexit();
    else
        decrementTemplate();
}

void FontHelperEngine::incrementTemplate()
{
    if (templateChar < '~')
        templateChar++;
    else
        templateChar = ' ';

    refreshTemplate();
}

void FontHelperEngine::decrementTemplate()
{
    if (templateChar > ' ')
        templateChar--;
    else
        templateChar = '~';

    refreshTemplate();
}

void FontHelperEngine::incrementHexit()
{
    if (hexits[hexitIndex] == 15)
        hexits[hexitIndex] = 0;
    else
        hexits[hexitIndex]++;

    updatePixelsFromHexits();

    refreshHexits();
    refreshPreview();
}

void FontHelperEngine::decrementHexit()
{
    if (hexits[hexitIndex] == 0)
        hexits[hexitIndex] = 15;
    else
        hexits[hexitIndex]--;

    updatePixelsFromHexits();

    refreshHexits();
    refreshPreview();
}

void FontHelperEngine::updatePixelsFromHexits()
{
    size_t mostSignificantNibbleIndex = hexitIndex - (hexitIndex % 2);
    size_t leastSignificantNibbleIndex = mostSignificantNibbleIndex + 1;
    size_t column = hexitIndex / 2;

    charBitmap.UpdateColumn(column, (hexits[mostSignificantNibbleIndex] << 4) + hexits[leastSignificantNibbleIndex]);
}

void FontHelperEngine::nextHexit()
{
    if (hexitIndex < NUMBER_OF_HEXITS)
        hexitIndex++;
    else
        hexitIndex = 0;

    refreshHexitPointer();
}

void FontHelperEngine::prevHexit()
{
    if (hexitIndex > 0)
        hexitIndex--;
    else
        hexitIndex = 12;

    refreshHexitPointer();
}

void FontHelperEngine::refreshHexits()
{
    print(hexitsToString(), 3, 0);
    print(charBitmap, CHAR_BITMAP_X, CHAR_BITMAP_Y, true);
}

void FontHelperEngine::refreshHexitPointer()
{
    if (currentMode == ModeEnum::DrawMode)
    {
        print(padding(WIDTH_OF_HEXIT_POINTER_STRING), 3, 7, FontEnum::FixedWidth);
        return;
    }

    blit::screen.pen = white;

    print(buildHexitPointerString(), HEXIT_POINTER_STRING_X, HEXIT_POINTER_STRING_Y, FontEnum::FixedWidth);
}

std::string FontHelperEngine::buildHexitPointerString()
{
    const int WIDTH_OF_HEXIT_POINTER_STRING = 19;
    const int PADDING_MAP[NUMBER_OF_HEXITS + 1] = { 0, 1, 3, 4, 6, 7, 9, 10, 12, 13, 15, 16, 18 };
    int prepadding = PADDING_MAP[hexitIndex];
    int postpadding = WIDTH_OF_HEXIT_POINTER_STRING - prepadding - 1;

    return padding(prepadding) + '^' + padding(postpadding);
}

void FontHelperEngine::refreshMode()
{
    if (currentMode == ModeEnum::HexMode)
    {
        printHexModeInstructions();
        
        refreshFont();
    }
    else if (currentMode == ModeEnum::DrawMode)
        printDrawModeInstructions();
}

void FontHelperEngine::printHexModeInstructions()
{
    print("(A) Font:", 80, 20);
    print("(B) Mode", 80, 40);
    print("[L][R]     ", 80, 50);
    print("Move Cursor", 80, 60);
    print("[U] Raise", 80, 70);
    print("[D] Lower", 80, 80);
    print("[Stick] Use", 80, 90);
    print("Template", 80, 100);
    print("(X)(Y) Help", 80, 110);
}

void FontHelperEngine::printDrawModeInstructions()
{
    print("(A) Pixel", 80, 20);
    print("(B) Mode   ", 80, 30);
    print("[D-Pad] ", 80, 40);
    print("Move Cursor", 80, 50);
    print("           ", 80, 60);
    print("         ", 80, 70);
    print("         ", 80, 80);
    print("[Stick] Use", 80, 90);
    print("Template", 80, 100);
    print("(X)(Y) Help", 80, 110);
}

std::string FontHelperEngine::padding(int amount)
{
    std::string spaces(amount, ' ');

    return spaces;
}

std::string FontHelperEngine::hexitsToString()
{
    std::string theString;

    for (size_t c = 0; c < NUMBER_OF_HEXITS; c += 2)
    {
        theString += HEXIT_MAP[hexits[c]];
        theString += HEXIT_MAP[hexits[c + 1]];
        theString += ' ';
    }

    theString.pop_back();

    return theString;
}

void FontHelperEngine::print(std::string stringToPrint, int x, int y, FontEnum font /*= FontEnum::FixedWidth*/)
{
    blit::screen.pen = black;
    blit::screen.rectangle(blit::Rect(x, y, stringToPrint.size() * FIXED_CHAR_WIDTH, FIXED_CHAR_HEIGHT));

    blit::screen.pen = white;
    blit::screen.text(stringToPrint, fonts[(int)font], blit::Point(x, y));
}

void FontHelperEngine::print(Bitmap bitmap, int x, int y, bool selection /*= false*/)
{
    drawPixelBackground(x, y);

    if (selection)
        drawPixelCursor(x, y);

    x++; y++;

    for (int r = 0; r < Bitmap::ROWS; r++)
        for (int c = 0; c < Bitmap::COLUMNS; c++)
        {
            blit::screen.pen = bitmap.Pixels[r][c] ? white : black;
            blit::screen.rectangle(blit::Rect(5 * c + x, 5 * r + y, 4, 4));
        }
}

void FontHelperEngine::drawPixelBackground(int x, int y)
{
    blit::screen.pen = gray;
    blit::screen.rectangle(blit::Rect(x, y, 5 * Bitmap::COLUMNS + 1, 5 * Bitmap::ROWS + 1));
}

void FontHelperEngine::drawPixelCursor(int x, int y)
{
    if (currentMode != ModeEnum::DrawMode)
        return;

    blit::screen.pen = green;
    blit::screen.rectangle(blit::Rect(5 * drawColumn + CHAR_BITMAP_X, 5 * drawRow + CHAR_BITMAP_Y, 6, 6));
}

void FontHelperEngine::copyTemplate()
{
    for (size_t r = 0; r < Bitmap::ROWS; r++)
        for (size_t c = 0; c < Bitmap::COLUMNS; c++)
            charBitmap.Pixels[r][c] = templateBitmap.Pixels[r][c];

    copyPixelsToHexits();

    refreshPreview();
}

void FontHelperEngine::copyPixelsToHexits()
{
    for (size_t h = 0; h < NUMBER_OF_HEXITS; h++)
        hexits[h] = 0;
        
    for (size_t r = 0; r < Bitmap::ROWS; r++)
        for (size_t c = 0; c < Bitmap::COLUMNS; c++)
            hexits[2 * c + (r > 3 ? 0 : 1)] += charBitmap.Pixels[r][c] << (r % 4);

    refreshHexits();
}
