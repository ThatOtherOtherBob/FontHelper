# Font Helper

This is a utility for drawing characters for new fonts.

Font Helper v1.0.0 by LBOB

To print text in C++, use "blit::screen.text(...)".
Available fonts are: Minimal, Fat and Outline.
These are stored in "32blit-sdk-master/32blit/graphics/font.cpp".
This program also uses a 4th, custom font called "FixedWidth".
FixedWidth exists to add lower-case letters to the minimal font, tweak positioning and make certain characters more distinct.
(e.g. #:1, L:l, U:I and S:|)
This is not a built-in font, and is mostly ripped off from the minimal font. ";
Each character is saved as a series of 6 bytes, shown as nybbles/hexits. (e.g. "{0x00,0x00,0x2e,0x00,0x00,0x00}, // !")
This program lets you determine what to set these bytes to.

Note: fonts only represent characters from space/dec32/hex20 to ~/dec126/hex7e.

The upper-left grid is the edit window.
The right and bottom grids are the template character.
Below the bottom grid is a preview of the edit window.
Above the edit window is the list of nybbles representing what you've drawn.
To the right of the nybbles is the template character shown normal size.
To the right of the template is a symbol indicating where in the character set you are.  (U)ppercase, (L)owercase, (S)ymbol/Space or (#)Number.
Note: some fonts repeat the upper-case letters in the lower-case area.

You start in pixel mode.
Move with the d-pad.
Toggle pixels with A.
Swap modes with B.
Press in the stick to copy the template into the edit window.

When you are happy with the letter:
1. Make a copy of an existing font's data.  (i.e. "minimal_font_data", "minimal_font_width" and "minimal_font")
2. Replace the letter of your choice in *_font_data with the bytes at the top of the screen.
3. You may need to edit the *_font_width array.

In hex mode, you can use up/down to change the bytes and left/right to move to a different nybble/hexit.
Use up/down while the template is selected to move to the next/previous character.
Use A to swap which font's characters are being used as templates.
Swap modes with B.
Press in the stick to copy the template into the edit window.