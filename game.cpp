#include "game.hpp"
#include "FontHelperEngine.hpp"

FontHelperEngine FontHelperEngine;

///////////////////////////////////////////////////////////////////////////
//
// init()
//
// setup your game here
//
void init()
{
    FontHelperEngine.Start();
}

///////////////////////////////////////////////////////////////////////////
//
// render(time)
//
// This function is called to perform rendering of the game. time is the 
// amount if milliseconds elapsed since the start of your game
//
void render(uint32_t time)
{
    FontHelperEngine.Render();
}

///////////////////////////////////////////////////////////////////////////
//
// update(time)
//
// This is called to update your game state. time is the 
// amount if milliseconds elapsed since the start of your game
//
void update(uint32_t time)
{
    FontHelperEngine.Update();
}
