#include "Convert.h"

using namespace pure;

SDL_Scancode toSDLScancode(Key key)
{
	switch (key)
	{
		case Key::UNKNOWN: return SDL_SCANCODE_UNKNOWN;
		case Key::A: return SDL_SCANCODE_A;
		case Key::C: return SDL_SCANCODE_C;
		case Key::D: return SDL_SCANCODE_D;
		case Key::E: return SDL_SCANCODE_E;
		case Key::F: return SDL_SCANCODE_F;
		case Key::B: return SDL_SCANCODE_B;
		case Key::G: return SDL_SCANCODE_G;
		case Key::H: return SDL_SCANCODE_H;
		case Key::I: return SDL_SCANCODE_I;
		case Key::J: return SDL_SCANCODE_J;
		case Key::K: return SDL_SCANCODE_K;
		case Key::L: return SDL_SCANCODE_L;
		case Key::M: return SDL_SCANCODE_M;
		case Key::N: return SDL_SCANCODE_N;
		case Key::O: return SDL_SCANCODE_O;
		case Key::P: return SDL_SCANCODE_P;
		case Key::Q: return SDL_SCANCODE_Q;
		case Key::R: return SDL_SCANCODE_R;
		case Key::S: return SDL_SCANCODE_S;
		case Key::T: return SDL_SCANCODE_T;
		case Key::U: return SDL_SCANCODE_U;
		case Key::V: return SDL_SCANCODE_V;
		case Key::W: return SDL_SCANCODE_W;
		case Key::X: return SDL_SCANCODE_X;
		case Key::Y: return SDL_SCANCODE_Y;
		case Key::Z: return SDL_SCANCODE_Z;
		case Key::N1: return SDL_SCANCODE_1;
		case Key::N2: return SDL_SCANCODE_2;
		case Key::N3: return SDL_SCANCODE_3;
		case Key::N4: return SDL_SCANCODE_4;
		case Key::N5: return SDL_SCANCODE_5;
		case Key::N6: return SDL_SCANCODE_6;
		case Key::N7: return SDL_SCANCODE_7;
		case Key::N8: return SDL_SCANCODE_8;
		case Key::N9: return SDL_SCANCODE_9;
		case Key::N0: return SDL_SCANCODE_0;
		case Key::RETURN: return SDL_SCANCODE_RETURN;
		case Key::ESCAPE: return SDL_SCANCODE_ESCAPE;
		case Key::BACKSPACE: return SDL_SCANCODE_BACKSPACE;
		case Key::TAB: return SDL_SCANCODE_TAB;
		case Key::SPACE: return SDL_SCANCODE_SPACE;
		case Key::MINUS: return SDL_SCANCODE_MINUS;
		case Key::EQUALS: return SDL_SCANCODE_EQUALS;
		case Key::LEFTBRACKET: return SDL_SCANCODE_LEFTBRACKET;
		case Key::RIGHTBRACKET: return SDL_SCANCODE_RIGHTBRACKET;
		case Key::BACKSLASH: return SDL_SCANCODE_BACKSLASH;
		case Key::NONUSHASH: return SDL_SCANCODE_NONUSHASH;
		case Key::SEMICOLON: return SDL_SCANCODE_SEMICOLON;
		case Key::APOSTROPHE: return SDL_SCANCODE_APOSTROPHE;
		case Key::GRAVE: return SDL_SCANCODE_GRAVE;
		case Key::COMMA: return SDL_SCANCODE_COMMA;
		case Key::PERIOD: return SDL_SCANCODE_PERIOD;
		case Key::SLASH: return SDL_SCANCODE_SLASH;
		case Key::CAPSLOCK: return SDL_SCANCODE_CAPSLOCK;
		case Key::F1: return SDL_SCANCODE_F1;
		case Key::F2: return SDL_SCANCODE_F2;
		case Key::F3: return SDL_SCANCODE_F3;
		case Key::F4: return SDL_SCANCODE_F4;
		case Key::F5: return SDL_SCANCODE_F5;
		case Key::F6: return SDL_SCANCODE_F6;
		case Key::F7: return SDL_SCANCODE_F7;
		case Key::F8: return SDL_SCANCODE_F8;
		case Key::F9: return SDL_SCANCODE_F9;
		case Key::F10: return SDL_SCANCODE_F10;
		case Key::F11: return SDL_SCANCODE_F11;
		case Key::F12: return SDL_SCANCODE_F12;
		case Key::PRINTSCREEN: return SDL_SCANCODE_PRINTSCREEN;
		case Key::SCROLLLOCK: return SDL_SCANCODE_SCROLLLOCK;
		case Key::PAUSE: return SDL_SCANCODE_PAUSE;
		case Key::INSERT: return SDL_SCANCODE_INSERT;
		case Key::HOME: return SDL_SCANCODE_HOME;
		case Key::PAGEUP: return SDL_SCANCODE_PAGEUP;
		case Key::K_DELETE: return SDL_SCANCODE_DELETE;
		case Key::END: return SDL_SCANCODE_END;
		case Key::PAGEDOWN: return SDL_SCANCODE_PAGEDOWN;
		case Key::RIGHT: return SDL_SCANCODE_RIGHT;
		case Key::LEFT: return SDL_SCANCODE_LEFT;
		case Key::DOWN: return SDL_SCANCODE_DOWN;
		case Key::UP: return SDL_SCANCODE_UP;
		case Key::NUMLOCKCLEAR: return SDL_SCANCODE_NUMLOCKCLEAR;
		case Key::KP_DIVIDE: return SDL_SCANCODE_KP_DIVIDE;
		case Key::KP_MULTIPLY: return SDL_SCANCODE_KP_MULTIPLY;
		case Key::KP_MINUS: return SDL_SCANCODE_KP_MINUS;
		case Key::KP_PLUS: return SDL_SCANCODE_KP_PLUS;
		case Key::KP_ENTER: return SDL_SCANCODE_KP_ENTER;
		case Key::KP_1: return SDL_SCANCODE_KP_1;
		case Key::KP_2: return SDL_SCANCODE_KP_2;
		case Key::KP_3: return SDL_SCANCODE_KP_3;
		case Key::KP_4: return SDL_SCANCODE_KP_4;
		case Key::KP_5: return SDL_SCANCODE_KP_5;
		case Key::KP_6: return SDL_SCANCODE_KP_6;
		case Key::KP_7: return SDL_SCANCODE_KP_7;
		case Key::KP_8: return SDL_SCANCODE_KP_8;
		case Key::KP_9: return SDL_SCANCODE_KP_9;
		case Key::KP_0: return SDL_SCANCODE_KP_0;
		case Key::KP_PERIOD: return SDL_SCANCODE_KP_PERIOD;
		case Key::NONUSBACKSLASH: return SDL_SCANCODE_NONUSBACKSLASH;
		case Key::APPLICATION: return SDL_SCANCODE_APPLICATION;
		case Key::POWER: return SDL_SCANCODE_POWER;
		case Key::KP_EQUALS: return SDL_SCANCODE_KP_EQUALS;
		case Key::F13: return SDL_SCANCODE_F13;
		case Key::F14: return SDL_SCANCODE_F14;
		case Key::F15: return SDL_SCANCODE_F15;
		case Key::F16: return SDL_SCANCODE_F16;
		case Key::F17: return SDL_SCANCODE_F17;
		case Key::F18: return SDL_SCANCODE_F18;
		case Key::F19: return SDL_SCANCODE_F19;
		case Key::F20: return SDL_SCANCODE_F20;
		case Key::F21: return SDL_SCANCODE_F21;
		case Key::F22: return SDL_SCANCODE_F22;
		case Key::F23: return SDL_SCANCODE_F23;
		case Key::F24: return SDL_SCANCODE_F24;
		case Key::EXECUTE: return SDL_SCANCODE_EXECUTE;
		case Key::HELP: return SDL_SCANCODE_HELP;
		case Key::MENU: return SDL_SCANCODE_MENU;
		case Key::SELECT: return SDL_SCANCODE_SELECT;
		case Key::STOP: return SDL_SCANCODE_STOP;
		case Key::AGAIN: return SDL_SCANCODE_AGAIN;
		case Key::UNDO: return SDL_SCANCODE_UNDO;
		case Key::CUT: return SDL_SCANCODE_CUT;
		case Key::COPY: return SDL_SCANCODE_COPY;
		case Key::PASTE: return SDL_SCANCODE_PASTE;
		case Key::FIND: return SDL_SCANCODE_FIND;
		case Key::MUTE: return SDL_SCANCODE_MUTE;
		case Key::VOLUMEUP: return SDL_SCANCODE_VOLUMEUP;
		case Key::VOLUMEDOWN: return SDL_SCANCODE_VOLUMEDOWN;
		case Key::KP_COMMA: return SDL_SCANCODE_KP_COMMA;
		case Key::KP_EQUALSAS400: return SDL_SCANCODE_KP_EQUALSAS400;
		case Key::INTERNATIONAL1: return SDL_SCANCODE_INTERNATIONAL1;
		case Key::INTERNATIONAL2: return SDL_SCANCODE_INTERNATIONAL2;
		case Key::INTERNATIONAL3: return SDL_SCANCODE_INTERNATIONAL3;
		case Key::INTERNATIONAL4: return SDL_SCANCODE_INTERNATIONAL4;
		case Key::INTERNATIONAL5: return SDL_SCANCODE_INTERNATIONAL5;
		case Key::INTERNATIONAL6: return SDL_SCANCODE_INTERNATIONAL6;
		case Key::INTERNATIONAL7: return SDL_SCANCODE_INTERNATIONAL7;
		case Key::INTERNATIONAL8: return SDL_SCANCODE_INTERNATIONAL8;
		case Key::INTERNATIONAL9: return SDL_SCANCODE_INTERNATIONAL9;
		case Key::LANG1: return SDL_SCANCODE_LANG1;
		case Key::LANG2: return SDL_SCANCODE_LANG2;
		case Key::LANG3: return SDL_SCANCODE_LANG3;
		case Key::LANG4: return SDL_SCANCODE_LANG4;
		case Key::LANG5: return SDL_SCANCODE_LANG5;
		case Key::LANG6: return SDL_SCANCODE_LANG6;
		case Key::LANG7: return SDL_SCANCODE_LANG7;
		case Key::LANG8: return SDL_SCANCODE_LANG8;
		case Key::LANG9: return SDL_SCANCODE_LANG9;
		case Key::ALTERASE: return SDL_SCANCODE_ALTERASE;
		case Key::SYSREQ: return SDL_SCANCODE_SYSREQ;
		case Key::CANCEL: return SDL_SCANCODE_CANCEL;
		case Key::CLEAR: return SDL_SCANCODE_CLEAR;
		case Key::PRIOR: return SDL_SCANCODE_PRIOR;
		case Key::RETURN2: return SDL_SCANCODE_RETURN2;
		case Key::SEPARATOR: return SDL_SCANCODE_SEPARATOR;
		case Key::K_OUT: return SDL_SCANCODE_OUT;
		case Key::OPER: return SDL_SCANCODE_OPER;
		case Key::CLEARAGAIN: return SDL_SCANCODE_CLEARAGAIN;
		case Key::CRSEL: return SDL_SCANCODE_CRSEL;
		case Key::EXSEL: return SDL_SCANCODE_EXSEL;
		case Key::KP_00: return SDL_SCANCODE_KP_00;
		case Key::KP_000: return SDL_SCANCODE_KP_000;
		case Key::THOUSANDSSEPARATOR: return SDL_SCANCODE_THOUSANDSSEPARATOR;
		case Key::DECIMALSEPARATOR: return SDL_SCANCODE_DECIMALSEPARATOR;
		case Key::CURRENCYUNIT: return SDL_SCANCODE_CURRENCYUNIT;
		case Key::CURRENCYSUBUNIT: return SDL_SCANCODE_CURRENCYSUBUNIT;
		case Key::KP_LEFTPAREN: return SDL_SCANCODE_KP_LEFTPAREN;
		case Key::KP_RIGHTPAREN: return SDL_SCANCODE_KP_RIGHTPAREN;
		case Key::KP_LEFTBRACE: return SDL_SCANCODE_KP_LEFTBRACE;
		case Key::KP_RIGHTBRACE: return SDL_SCANCODE_KP_RIGHTBRACE;
		case Key::KP_TAB: return SDL_SCANCODE_KP_TAB;
		case Key::KP_BACKSPACE: return SDL_SCANCODE_KP_BACKSPACE;
		case Key::KP_A: return SDL_SCANCODE_KP_A;
		case Key::KP_B: return SDL_SCANCODE_KP_B;
		case Key::KP_C: return SDL_SCANCODE_KP_C;
		case Key::KP_D: return SDL_SCANCODE_KP_D;
		case Key::KP_E: return SDL_SCANCODE_KP_E;
		case Key::KP_F: return SDL_SCANCODE_KP_F;
		case Key::KP_XOR: return SDL_SCANCODE_KP_XOR;
		case Key::KP_POWER: return SDL_SCANCODE_KP_POWER;
		case Key::KP_PERCENT: return SDL_SCANCODE_KP_PERCENT;
		case Key::KP_LESS: return SDL_SCANCODE_KP_LESS;
		case Key::KP_GREATER: return SDL_SCANCODE_KP_GREATER;
		case Key::KP_AMPERSAND: return SDL_SCANCODE_KP_AMPERSAND;
		case Key::KP_DBLAMPERSAND: return SDL_SCANCODE_KP_DBLAMPERSAND;
		case Key::KP_VERTICALBAR: return SDL_SCANCODE_KP_VERTICALBAR;
		case Key::KP_DBLVERTICALBAR: return SDL_SCANCODE_KP_DBLVERTICALBAR;
		case Key::KP_COLON: return SDL_SCANCODE_KP_COLON;
		case Key::KP_HASH: return SDL_SCANCODE_KP_HASH;
		case Key::KP_SPACE: return SDL_SCANCODE_KP_SPACE;
		case Key::KP_AT: return SDL_SCANCODE_KP_AT;
		case Key::KP_EXCLAM: return SDL_SCANCODE_KP_EXCLAM;
		case Key::KP_MEMSTORE: return SDL_SCANCODE_KP_MEMSTORE;
		case Key::KP_MEMRECALL: return SDL_SCANCODE_KP_MEMRECALL;
		case Key::KP_MEMCLEAR: return SDL_SCANCODE_KP_MEMCLEAR;
		case Key::KP_MEMADD: return SDL_SCANCODE_KP_MEMADD;
		case Key::KP_MEMSUBTRACT: return SDL_SCANCODE_KP_MEMSUBTRACT;
		case Key::KP_MEMMULTIPLY: return SDL_SCANCODE_KP_MEMMULTIPLY;
		case Key::KP_MEMDIVIDE: return SDL_SCANCODE_KP_MEMDIVIDE;
		case Key::KP_PLUSMINUS: return SDL_SCANCODE_KP_PLUSMINUS;
		case Key::KP_CLEAR: return SDL_SCANCODE_KP_CLEAR;
		case Key::KP_CLEARENTRY: return SDL_SCANCODE_KP_CLEARENTRY;
		case Key::KP_BINARY: return SDL_SCANCODE_KP_BINARY;
		case Key::KP_OCTAL: return SDL_SCANCODE_KP_OCTAL;
		case Key::KP_DECIMAL: return SDL_SCANCODE_KP_DECIMAL;
		case Key::KP_HEXADECIMAL: return SDL_SCANCODE_KP_HEXADECIMAL;
		case Key::LCTRL: return SDL_SCANCODE_LCTRL;
		case Key::LSHIFT: return SDL_SCANCODE_LSHIFT;
		case Key::LALT: return SDL_SCANCODE_LALT;
		case Key::LGUI: return SDL_SCANCODE_LGUI;
		case Key::RCTRL: return SDL_SCANCODE_RCTRL;
		case Key::RSHIFT: return SDL_SCANCODE_RSHIFT;
		case Key::RALT: return SDL_SCANCODE_RALT;
		case Key::RGUI: return SDL_SCANCODE_RGUI;
		case Key::MODE: return SDL_SCANCODE_MODE;
		case Key::AUDIONEXT: return SDL_SCANCODE_AUDIONEXT;
		case Key::AUDIOPREV: return SDL_SCANCODE_AUDIOPREV;
		case Key::AUDIOSTOP: return SDL_SCANCODE_AUDIOSTOP;
		case Key::AUDIOPLAY: return SDL_SCANCODE_AUDIOPLAY;
		case Key::AUDIOMUTE: return SDL_SCANCODE_AUDIOMUTE;
		case Key::MEDIASELECT: return SDL_SCANCODE_MEDIASELECT;
		case Key::WWW: return SDL_SCANCODE_WWW;
		case Key::MAIL: return SDL_SCANCODE_MAIL;
		case Key::CALCULATOR: return SDL_SCANCODE_CALCULATOR;
		case Key::COMPUTER: return SDL_SCANCODE_COMPUTER;
		case Key::AC_SEARCH: return SDL_SCANCODE_AC_SEARCH;
		case Key::AC_HOME: return SDL_SCANCODE_AC_HOME;
		case Key::AC_BACK: return SDL_SCANCODE_AC_BACK;
		case Key::AC_FORWARD: return SDL_SCANCODE_AC_FORWARD;
		case Key::AC_STOP: return SDL_SCANCODE_AC_STOP;
		case Key::AC_REFRESH: return SDL_SCANCODE_AC_REFRESH;
		case Key::AC_BOOKMARKS: return SDL_SCANCODE_AC_BOOKMARKS;
		case Key::BRIGHTNESSDOWN: return SDL_SCANCODE_BRIGHTNESSDOWN;
		case Key::BRIGHTNESSUP: return SDL_SCANCODE_BRIGHTNESSUP;
		case Key::DISPLAYSWITCH: return SDL_SCANCODE_DISPLAYSWITCH;
		case Key::KBDILLUMTOGGLE: return SDL_SCANCODE_KBDILLUMTOGGLE;
		case Key::KBDILLUMDOWN: return SDL_SCANCODE_KBDILLUMDOWN;
		case Key::KBDILLUMUP: return SDL_SCANCODE_KBDILLUMUP;
		case Key::EJECT: return SDL_SCANCODE_EJECT;
		case Key::SLEEP: return SDL_SCANCODE_SLEEP;
		case Key::APP1: return SDL_SCANCODE_APP1;
		case Key::APP2: return SDL_SCANCODE_APP2;
		default: return SDL_SCANCODE_UNKNOWN;
	}
}

pure::Key toPureKey(SDL_Scancode code)
{
	switch (code)
	{
		case SDL_SCANCODE_UNKNOWN: return Key::UNKNOWN;
		case SDL_SCANCODE_A: return Key::A;
		case SDL_SCANCODE_C: return Key::C;
		case SDL_SCANCODE_D: return Key::D;
		case SDL_SCANCODE_E: return Key::E;
		case SDL_SCANCODE_F: return Key::F;
		case SDL_SCANCODE_B: return Key::B;
		case SDL_SCANCODE_G: return Key::G;
		case SDL_SCANCODE_H: return Key::H;
		case SDL_SCANCODE_I: return Key::I;
		case SDL_SCANCODE_J: return Key::J;
		case SDL_SCANCODE_K: return Key::K;
		case SDL_SCANCODE_L: return Key::L;
		case SDL_SCANCODE_M: return Key::M;
		case SDL_SCANCODE_N: return Key::N;
		case SDL_SCANCODE_O: return Key::O;
		case SDL_SCANCODE_P: return Key::P;
		case SDL_SCANCODE_Q: return Key::Q;
		case SDL_SCANCODE_R: return Key::R;
		case SDL_SCANCODE_S: return Key::S;
		case SDL_SCANCODE_T: return Key::T;
		case SDL_SCANCODE_U: return Key::U;
		case SDL_SCANCODE_V: return Key::V;
		case SDL_SCANCODE_W: return Key::W;
		case SDL_SCANCODE_X: return Key::X;
		case SDL_SCANCODE_Y: return Key::Y;
		case SDL_SCANCODE_Z: return Key::Z;
		case SDL_SCANCODE_1: return Key::N1;
		case SDL_SCANCODE_2: return Key::N2;
		case SDL_SCANCODE_3: return Key::N3;
		case SDL_SCANCODE_4: return Key::N4;
		case SDL_SCANCODE_5: return Key::N5;
		case SDL_SCANCODE_6: return Key::N6;
		case SDL_SCANCODE_7: return Key::N7;
		case SDL_SCANCODE_8: return Key::N8;
		case SDL_SCANCODE_9: return Key::N9;
		case SDL_SCANCODE_0: return Key::N0;
		case SDL_SCANCODE_RETURN: return Key::RETURN;
		case SDL_SCANCODE_ESCAPE: return Key::ESCAPE;
		case SDL_SCANCODE_BACKSPACE: return Key::BACKSPACE;
		case SDL_SCANCODE_TAB: return Key::TAB;
		case SDL_SCANCODE_SPACE: return Key::SPACE;
		case SDL_SCANCODE_MINUS: return Key::MINUS;
		case SDL_SCANCODE_EQUALS: return Key::EQUALS;
		case SDL_SCANCODE_LEFTBRACKET: return Key::LEFTBRACKET;
		case SDL_SCANCODE_RIGHTBRACKET: return Key::RIGHTBRACKET;
		case SDL_SCANCODE_BACKSLASH: return Key::BACKSLASH;
		case SDL_SCANCODE_NONUSHASH: return Key::NONUSHASH;
		case SDL_SCANCODE_SEMICOLON: return Key::SEMICOLON;
		case SDL_SCANCODE_APOSTROPHE: return Key::APOSTROPHE;
		case SDL_SCANCODE_GRAVE: return Key::GRAVE;
		case SDL_SCANCODE_COMMA: return Key::COMMA;
		case SDL_SCANCODE_PERIOD: return Key::PERIOD;
		case SDL_SCANCODE_SLASH: return Key::SLASH;
		case SDL_SCANCODE_CAPSLOCK: return Key::CAPSLOCK;
		case SDL_SCANCODE_F1: return Key::F1;
		case SDL_SCANCODE_F2: return Key::F2;
		case SDL_SCANCODE_F3: return Key::F3;
		case SDL_SCANCODE_F4: return Key::F4;
		case SDL_SCANCODE_F5: return Key::F5;
		case SDL_SCANCODE_F6: return Key::F6;
		case SDL_SCANCODE_F7: return Key::F7;
		case SDL_SCANCODE_F8: return Key::F8;
		case SDL_SCANCODE_F9: return Key::F9;
		case SDL_SCANCODE_F10: return Key::F10;
		case SDL_SCANCODE_F11: return Key::F11;
		case SDL_SCANCODE_F12: return Key::F12;
		case SDL_SCANCODE_PRINTSCREEN: return Key::PRINTSCREEN;
		case SDL_SCANCODE_SCROLLLOCK: return Key::SCROLLLOCK;
		case SDL_SCANCODE_PAUSE: return Key::PAUSE;
		case SDL_SCANCODE_INSERT: return Key::INSERT;
		case SDL_SCANCODE_HOME: return Key::HOME;
		case SDL_SCANCODE_PAGEUP: return Key::PAGEUP;
		case SDL_SCANCODE_DELETE: return Key::K_DELETE;
		case SDL_SCANCODE_END: return Key::END;
		case SDL_SCANCODE_PAGEDOWN: return Key::PAGEDOWN;
		case SDL_SCANCODE_RIGHT: return Key::RIGHT;
		case SDL_SCANCODE_LEFT: return Key::LEFT;
		case SDL_SCANCODE_DOWN: return Key::DOWN;
		case SDL_SCANCODE_UP: return Key::UP;
		case SDL_SCANCODE_NUMLOCKCLEAR: return Key::NUMLOCKCLEAR;
		case SDL_SCANCODE_KP_DIVIDE: return Key::KP_DIVIDE;
		case SDL_SCANCODE_KP_MULTIPLY: return Key::KP_MULTIPLY;
		case SDL_SCANCODE_KP_MINUS: return Key::KP_MINUS;
		case SDL_SCANCODE_KP_PLUS: return Key::KP_PLUS;
		case SDL_SCANCODE_KP_ENTER: return Key::KP_ENTER;
		case SDL_SCANCODE_KP_1: return Key::KP_1;
		case SDL_SCANCODE_KP_2: return Key::KP_2;
		case SDL_SCANCODE_KP_3: return Key::KP_3;
		case SDL_SCANCODE_KP_4: return Key::KP_4;
		case SDL_SCANCODE_KP_5: return Key::KP_5;
		case SDL_SCANCODE_KP_6: return Key::KP_6;
		case SDL_SCANCODE_KP_7: return Key::KP_7;
		case SDL_SCANCODE_KP_8: return Key::KP_8;
		case SDL_SCANCODE_KP_9: return Key::KP_9;
		case SDL_SCANCODE_KP_0: return Key::KP_0;
		case SDL_SCANCODE_KP_PERIOD: return Key::KP_PERIOD;
		case SDL_SCANCODE_NONUSBACKSLASH: return Key::NONUSBACKSLASH;
		case SDL_SCANCODE_APPLICATION: return Key::APPLICATION;
		case SDL_SCANCODE_POWER: return Key::POWER;
		case SDL_SCANCODE_KP_EQUALS: return Key::KP_EQUALS;
		case SDL_SCANCODE_F13: return Key::F13;
		case SDL_SCANCODE_F14: return Key::F14;
		case SDL_SCANCODE_F15: return Key::F15;
		case SDL_SCANCODE_F16: return Key::F16;
		case SDL_SCANCODE_F17: return Key::F17;
		case SDL_SCANCODE_F18: return Key::F18;
		case SDL_SCANCODE_F19: return Key::F19;
		case SDL_SCANCODE_F20: return Key::F20;
		case SDL_SCANCODE_F21: return Key::F21;
		case SDL_SCANCODE_F22: return Key::F22;
		case SDL_SCANCODE_F23: return Key::F23;
		case SDL_SCANCODE_F24: return Key::F24;
		case SDL_SCANCODE_EXECUTE: return Key::EXECUTE;
		case SDL_SCANCODE_HELP: return Key::HELP;
		case SDL_SCANCODE_MENU: return Key::MENU;
		case SDL_SCANCODE_SELECT: return Key::SELECT;
		case SDL_SCANCODE_STOP: return Key::STOP;
		case SDL_SCANCODE_AGAIN: return Key::AGAIN;
		case SDL_SCANCODE_UNDO: return Key::UNDO;
		case SDL_SCANCODE_CUT: return Key::CUT;
		case SDL_SCANCODE_COPY: return Key::COPY;
		case SDL_SCANCODE_PASTE: return Key::PASTE;
		case SDL_SCANCODE_FIND: return Key::FIND;
		case SDL_SCANCODE_MUTE: return Key::MUTE;
		case SDL_SCANCODE_VOLUMEUP: return Key::VOLUMEUP;
		case SDL_SCANCODE_VOLUMEDOWN: return Key::VOLUMEDOWN;
		case SDL_SCANCODE_KP_COMMA: return Key::KP_COMMA;
		case SDL_SCANCODE_KP_EQUALSAS400: return Key::KP_EQUALSAS400;
		case SDL_SCANCODE_INTERNATIONAL1: return Key::INTERNATIONAL1;
		case SDL_SCANCODE_INTERNATIONAL2: return Key::INTERNATIONAL2;
		case SDL_SCANCODE_INTERNATIONAL3: return Key::INTERNATIONAL3;
		case SDL_SCANCODE_INTERNATIONAL4: return Key::INTERNATIONAL4;
		case SDL_SCANCODE_INTERNATIONAL5: return Key::INTERNATIONAL5;
		case SDL_SCANCODE_INTERNATIONAL6: return Key::INTERNATIONAL6;
		case SDL_SCANCODE_INTERNATIONAL7: return Key::INTERNATIONAL7;
		case SDL_SCANCODE_INTERNATIONAL8: return Key::INTERNATIONAL8;
		case SDL_SCANCODE_INTERNATIONAL9: return Key::INTERNATIONAL9;
		case SDL_SCANCODE_LANG1: return Key::LANG1;
		case SDL_SCANCODE_LANG2: return Key::LANG2;
		case SDL_SCANCODE_LANG3: return Key::LANG3;
		case SDL_SCANCODE_LANG4: return Key::LANG4;
		case SDL_SCANCODE_LANG5: return Key::LANG5;
		case SDL_SCANCODE_LANG6: return Key::LANG6;
		case SDL_SCANCODE_LANG7: return Key::LANG7;
		case SDL_SCANCODE_LANG8: return Key::LANG8;
		case SDL_SCANCODE_LANG9: return Key::LANG9;
		case SDL_SCANCODE_ALTERASE: return Key::ALTERASE;
		case SDL_SCANCODE_SYSREQ: return Key::SYSREQ;
		case SDL_SCANCODE_CANCEL: return Key::CANCEL;
		case SDL_SCANCODE_CLEAR: return Key::CLEAR;
		case SDL_SCANCODE_PRIOR: return Key::PRIOR;
		case SDL_SCANCODE_RETURN2: return Key::RETURN2;
		case SDL_SCANCODE_SEPARATOR: return Key::SEPARATOR;
		case SDL_SCANCODE_OUT: return Key::K_OUT;
		case SDL_SCANCODE_OPER: return Key::OPER;
		case SDL_SCANCODE_CLEARAGAIN: return Key::CLEARAGAIN;
		case SDL_SCANCODE_CRSEL: return Key::CRSEL;
		case SDL_SCANCODE_EXSEL: return Key::EXSEL;
		case SDL_SCANCODE_KP_00: return Key::KP_00;
		case SDL_SCANCODE_KP_000: return Key::KP_000;
		case SDL_SCANCODE_THOUSANDSSEPARATOR: return Key::THOUSANDSSEPARATOR;
		case SDL_SCANCODE_DECIMALSEPARATOR: return Key::DECIMALSEPARATOR;
		case SDL_SCANCODE_CURRENCYUNIT: return Key::CURRENCYUNIT;
		case SDL_SCANCODE_CURRENCYSUBUNIT: return Key::CURRENCYSUBUNIT;
		case SDL_SCANCODE_KP_LEFTPAREN: return Key::KP_LEFTPAREN;
		case SDL_SCANCODE_KP_RIGHTPAREN: return Key::KP_RIGHTPAREN;
		case SDL_SCANCODE_KP_LEFTBRACE: return Key::KP_LEFTBRACE;
		case SDL_SCANCODE_KP_RIGHTBRACE: return Key::KP_RIGHTBRACE;
		case SDL_SCANCODE_KP_TAB: return Key::KP_TAB;
		case SDL_SCANCODE_KP_BACKSPACE: return Key::KP_BACKSPACE;
		case SDL_SCANCODE_KP_A: return Key::KP_A;
		case SDL_SCANCODE_KP_B: return Key::KP_B;
		case SDL_SCANCODE_KP_C: return Key::KP_C;
		case SDL_SCANCODE_KP_D: return Key::KP_D;
		case SDL_SCANCODE_KP_E: return Key::KP_E;
		case SDL_SCANCODE_KP_F: return Key::KP_F;
		case SDL_SCANCODE_KP_XOR: return Key::KP_XOR;
		case SDL_SCANCODE_KP_POWER: return Key::KP_POWER;
		case SDL_SCANCODE_KP_PERCENT: return Key::KP_PERCENT;
		case SDL_SCANCODE_KP_LESS: return Key::KP_LESS;
		case SDL_SCANCODE_KP_GREATER: return Key::KP_GREATER;
		case SDL_SCANCODE_KP_AMPERSAND: return Key::KP_AMPERSAND;
		case SDL_SCANCODE_KP_DBLAMPERSAND: return Key::KP_DBLAMPERSAND;
		case SDL_SCANCODE_KP_VERTICALBAR: return Key::KP_VERTICALBAR;
		case SDL_SCANCODE_KP_DBLVERTICALBAR: return Key::KP_DBLVERTICALBAR;
		case SDL_SCANCODE_KP_COLON: return Key::KP_COLON;
		case SDL_SCANCODE_KP_HASH: return Key::KP_HASH;
		case SDL_SCANCODE_KP_SPACE: return Key::KP_SPACE;
		case SDL_SCANCODE_KP_AT: return Key::KP_AT;
		case SDL_SCANCODE_KP_EXCLAM: return Key::KP_EXCLAM;
		case SDL_SCANCODE_KP_MEMSTORE: return Key::KP_MEMSTORE;
		case SDL_SCANCODE_KP_MEMRECALL: return Key::KP_MEMRECALL;
		case SDL_SCANCODE_KP_MEMCLEAR: return Key::KP_MEMCLEAR;
		case SDL_SCANCODE_KP_MEMADD: return Key::KP_MEMADD;
		case SDL_SCANCODE_KP_MEMSUBTRACT: return Key::KP_MEMSUBTRACT;
		case SDL_SCANCODE_KP_MEMMULTIPLY: return Key::KP_MEMMULTIPLY;
		case SDL_SCANCODE_KP_MEMDIVIDE: return Key::KP_MEMDIVIDE;
		case SDL_SCANCODE_KP_PLUSMINUS: return Key::KP_PLUSMINUS;
		case SDL_SCANCODE_KP_CLEAR: return Key::KP_CLEAR;
		case SDL_SCANCODE_KP_CLEARENTRY: return Key::KP_CLEARENTRY;
		case SDL_SCANCODE_KP_BINARY: return Key::KP_BINARY;
		case SDL_SCANCODE_KP_OCTAL: return Key::KP_OCTAL;
		case SDL_SCANCODE_KP_DECIMAL: return Key::KP_DECIMAL;
		case SDL_SCANCODE_KP_HEXADECIMAL: return Key::KP_HEXADECIMAL;
		case SDL_SCANCODE_LCTRL: return Key::LCTRL;
		case SDL_SCANCODE_LSHIFT: return Key::LSHIFT;
		case SDL_SCANCODE_LALT: return Key::LALT;
		case SDL_SCANCODE_LGUI: return Key::LGUI;
		case SDL_SCANCODE_RCTRL: return Key::RCTRL;
		case SDL_SCANCODE_RSHIFT: return Key::RSHIFT;
		case SDL_SCANCODE_RALT: return Key::RALT;
		case SDL_SCANCODE_RGUI: return Key::RGUI;
		case SDL_SCANCODE_MODE: return Key::MODE;
		case SDL_SCANCODE_AUDIONEXT: return Key::AUDIONEXT;
		case SDL_SCANCODE_AUDIOPREV: return Key::AUDIOPREV;
		case SDL_SCANCODE_AUDIOSTOP: return Key::AUDIOSTOP;
		case SDL_SCANCODE_AUDIOPLAY: return Key::AUDIOPLAY;
		case SDL_SCANCODE_AUDIOMUTE: return Key::AUDIOMUTE;
		case SDL_SCANCODE_MEDIASELECT: return Key::MEDIASELECT;
		case SDL_SCANCODE_WWW: return Key::WWW;
		case SDL_SCANCODE_MAIL: return Key::MAIL;
		case SDL_SCANCODE_CALCULATOR: return Key::CALCULATOR;
		case SDL_SCANCODE_COMPUTER: return Key::COMPUTER;
		case SDL_SCANCODE_AC_SEARCH: return Key::AC_SEARCH;
		case SDL_SCANCODE_AC_HOME: return Key::AC_HOME;
		case SDL_SCANCODE_AC_BACK: return Key::AC_BACK;
		case SDL_SCANCODE_AC_FORWARD: return Key::AC_FORWARD;
		case SDL_SCANCODE_AC_STOP: return Key::AC_STOP;
		case SDL_SCANCODE_AC_REFRESH: return Key::AC_REFRESH;
		case SDL_SCANCODE_AC_BOOKMARKS: return Key::AC_BOOKMARKS;
		case SDL_SCANCODE_BRIGHTNESSDOWN: return Key::BRIGHTNESSDOWN;
		case SDL_SCANCODE_BRIGHTNESSUP: return Key::BRIGHTNESSUP;
		case SDL_SCANCODE_DISPLAYSWITCH: return Key::DISPLAYSWITCH;
		case SDL_SCANCODE_KBDILLUMTOGGLE: return Key::KBDILLUMTOGGLE;
		case SDL_SCANCODE_KBDILLUMDOWN: return Key::KBDILLUMDOWN;
		case SDL_SCANCODE_KBDILLUMUP: return Key::KBDILLUMUP;
		case SDL_SCANCODE_EJECT: return Key::EJECT;
		case SDL_SCANCODE_SLEEP: return Key::SLEEP;
		case SDL_SCANCODE_APP1: return Key::APP1;
		case SDL_SCANCODE_APP2: return Key::APP2;
		default: return Key::UNKNOWN;
	}


}

static constexpr GLenum glDataType[] = {
	GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT,
	GL_INT, GL_UNSIGNED_INT, GL_FLOAT
};

static constexpr GLenum glUsage[] = {
	GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW
};

static constexpr GLenum glPrim[] = {
	GL_POINTS, GL_LINES, GL_LINE_LOOP, GL_LINE_STRIP, GL_TRIANGLES, 
	GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS, GL_QUAD_STRIP, 
	GL_POLYGON
};

static constexpr GLenum glBuffAccess[] = {
	GL_READ_ONLY, GL_WRITE_ONLY, GL_READ_WRITE
};

static constexpr GLenum glTexFormat[] = {
	GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB,
	GL_RGBA, GL_R8
};

GLenum toGlTexFormat(pure::Texture::Format fmt)
{
	return glTexFormat[static_cast<int>(fmt)];
}

GLenum toGlBuffAccess(pure::BufferAccess access)
{
	return glBuffAccess[static_cast<int>(access)];
}


GLenum toGlPrim(pure::DrawPrimitive prim)
{
	return glPrim[static_cast<int>(prim)];
}

GLenum toGlUsage(pure::DrawUsage usage)
{
	return glUsage[static_cast<int>(usage)];
}

GLenum toGlDataType(pure::DataType dt)
{
	return glDataType[static_cast<int>(dt)];
}

