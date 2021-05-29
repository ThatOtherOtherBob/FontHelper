#pragma once
#include "32blit.hpp"
#include<vector>
#include "Buttons.hpp"

class ButtonEventCreator
{
public:
	std::vector<Buttons> GenerateEvents(blit::ButtonState buttons);
	ButtonEventCreator();
private:
	static const int BUTTONS_TRACKED = 9;
	bool oldButtons[BUTTONS_TRACKED], newButtons[BUTTONS_TRACKED];
	void setOldButtons();
	void setNewButtons(blit::ButtonState buttons);
	std::vector<Buttons> createPressedEvents();
};