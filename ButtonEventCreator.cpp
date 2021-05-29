#include "ButtonEventCreator.hpp"

ButtonEventCreator::ButtonEventCreator()
{
	for (size_t c = 0; c < BUTTONS_TRACKED; c++)
		newButtons[c] = false;

	setOldButtons(); // here to stop a warning, but filled before use
}

std::vector<Buttons> ButtonEventCreator::GenerateEvents(blit::ButtonState buttons)
{
	setOldButtons();

	setNewButtons(buttons);

	return createPressedEvents();
}

void ButtonEventCreator::setOldButtons()
{
	for (size_t c = 0; c < BUTTONS_TRACKED; c++)
		oldButtons[c] = newButtons[c];
}

void ButtonEventCreator::setNewButtons(blit::ButtonState buttons)
{
	newButtons[(size_t)Buttons::Up] = buttons & blit::Button::DPAD_UP;
	newButtons[(size_t)Buttons::Down] = buttons & blit::Button::DPAD_DOWN;
	newButtons[(size_t)Buttons::Left] = buttons & blit::Button::DPAD_LEFT;
	newButtons[(size_t)Buttons::Right] = buttons & blit::Button::DPAD_RIGHT;
	newButtons[(size_t)Buttons::A] = buttons & blit::Button::A;
	newButtons[(size_t)Buttons::B] = buttons & blit::Button::B;
	newButtons[(size_t)Buttons::X] = buttons & blit::Button::X;
	newButtons[(size_t)Buttons::Y] = buttons & blit::Button::Y;
	newButtons[(size_t)Buttons::Stick] = buttons & blit::Button::JOYSTICK;
}

std::vector<Buttons> ButtonEventCreator::createPressedEvents()
{
	std::vector<Buttons> events;

	for (size_t button = 0; button < BUTTONS_TRACKED; button++)
		if (newButtons[button] && !oldButtons[button])
			events.push_back((Buttons)button);

	return events;
}