#include "Keyboard.h"
#include <SDL2/SDL_events.h>
#include "Private/Convert.h"

#include <iostream>

using namespace pure;

bool pure::isKeyPressed(Key key)
{
	SDL_PumpEvents();
	const uint8_t* keyState = SDL_GetKeyboardState(nullptr);
	SDL_Scancode sc = toSDLScancode(key);
	bool result = keyState[toSDLScancode(key)];
	return result;

}
