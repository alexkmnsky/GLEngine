/*  
 * Copyright (c) 2022-2023 Alexander Kaminsky and Maxwell Hunt
 *  
 * This file incorporates work covered by the following copyright and permission notice:  
 *  
 *     Copyright (c) 2018 Benny Bobaganoosh
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *     
 *     The above copyright notice and this permission notice shall be included in all
 *     copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *     SOFTWARE.
 */

#pragma once

#include <SDL2/SDL.h>

// This file sucks. I know.

class SDLKeycode
{
public:
	static const unsigned int KEY_0 = SDL_SCANCODE_0;
	static const unsigned int KEY_1 = SDL_SCANCODE_1;
	static const unsigned int KEY_2 = SDL_SCANCODE_2;
	static const unsigned int KEY_3 = SDL_SCANCODE_3;
	static const unsigned int KEY_4 = SDL_SCANCODE_4;
	static const unsigned int KEY_5 = SDL_SCANCODE_5;
	static const unsigned int KEY_6 = SDL_SCANCODE_6;
	static const unsigned int KEY_7 = SDL_SCANCODE_7;
	static const unsigned int KEY_8 = SDL_SCANCODE_8;
	static const unsigned int KEY_9 = SDL_SCANCODE_9;

	static const unsigned int KEY_A = SDL_SCANCODE_A;
	static const unsigned int KEY_B = SDL_SCANCODE_B;
	static const unsigned int KEY_C = SDL_SCANCODE_C;
	static const unsigned int KEY_D = SDL_SCANCODE_D;
	static const unsigned int KEY_E = SDL_SCANCODE_E;
	static const unsigned int KEY_F = SDL_SCANCODE_F;
	static const unsigned int KEY_G = SDL_SCANCODE_G;
	static const unsigned int KEY_H = SDL_SCANCODE_H;
	static const unsigned int KEY_I = SDL_SCANCODE_I;
	static const unsigned int KEY_J = SDL_SCANCODE_J;
	static const unsigned int KEY_K = SDL_SCANCODE_K;
	static const unsigned int KEY_L = SDL_SCANCODE_L;
	static const unsigned int KEY_M = SDL_SCANCODE_M;
	static const unsigned int KEY_N = SDL_SCANCODE_N;
	static const unsigned int KEY_O = SDL_SCANCODE_O;
	static const unsigned int KEY_P = SDL_SCANCODE_P;
	static const unsigned int KEY_Q = SDL_SCANCODE_Q;
	static const unsigned int KEY_R = SDL_SCANCODE_R;
	static const unsigned int KEY_S = SDL_SCANCODE_S;
	static const unsigned int KEY_T = SDL_SCANCODE_T;
	static const unsigned int KEY_U = SDL_SCANCODE_U;
	static const unsigned int KEY_V = SDL_SCANCODE_V;
	static const unsigned int KEY_W = SDL_SCANCODE_W;
	static const unsigned int KEY_X = SDL_SCANCODE_X;
	static const unsigned int KEY_Y = SDL_SCANCODE_Y;
	static const unsigned int KEY_Z = SDL_SCANCODE_Z;

	static const unsigned int KEY_F1 = SDL_SCANCODE_F1;
	static const unsigned int KEY_F2 = SDL_SCANCODE_F2;
	static const unsigned int KEY_F3 = SDL_SCANCODE_F3;
	static const unsigned int KEY_F4 = SDL_SCANCODE_F4;
	static const unsigned int KEY_F5 = SDL_SCANCODE_F5;
	static const unsigned int KEY_F6 = SDL_SCANCODE_F6;
	static const unsigned int KEY_F7 = SDL_SCANCODE_F7;
	static const unsigned int KEY_F8 = SDL_SCANCODE_F8;
	static const unsigned int KEY_F9 = SDL_SCANCODE_F9;
	static const unsigned int KEY_F10 = SDL_SCANCODE_F10;
	static const unsigned int KEY_F11 = SDL_SCANCODE_F11;
	static const unsigned int KEY_F12 = SDL_SCANCODE_F12;
	static const unsigned int KEY_F13 = SDL_SCANCODE_F13;
	static const unsigned int KEY_F14 = SDL_SCANCODE_F14;
	static const unsigned int KEY_F15 = SDL_SCANCODE_F15;
	static const unsigned int KEY_F16 = SDL_SCANCODE_F16;
	static const unsigned int KEY_F17 = SDL_SCANCODE_F17;
	static const unsigned int KEY_F18 = SDL_SCANCODE_F18;
	static const unsigned int KEY_F19 = SDL_SCANCODE_F19;
	static const unsigned int KEY_F20 = SDL_SCANCODE_F20;
	static const unsigned int KEY_F21 = SDL_SCANCODE_F21;
	static const unsigned int KEY_F22 = SDL_SCANCODE_F22;
	static const unsigned int KEY_F23 = SDL_SCANCODE_F23;
	static const unsigned int KEY_F24 = SDL_SCANCODE_F24;

	static const unsigned int KEY_AC_BACK = SDL_SCANCODE_AC_BACK;
	static const unsigned int KEY_AC_BOOKMARKS = SDL_SCANCODE_AC_BOOKMARKS;
	static const unsigned int KEY_AC_HOME = SDL_SCANCODE_AC_HOME;
	static const unsigned int KEY_AC_REFRESH = SDL_SCANCODE_AC_REFRESH;
	static const unsigned int KEY_AC_SEARCH = SDL_SCANCODE_AC_SEARCH;
	static const unsigned int KEY_AC_STOP = SDL_SCANCODE_AC_STOP;
	static const unsigned int KEY_AGAIN = SDL_SCANCODE_AGAIN;
	static const unsigned int KEY_ALTERASE = SDL_SCANCODE_ALTERASE;
	static const unsigned int KEY_APOSTROPHE = SDL_SCANCODE_APOSTROPHE;
	static const unsigned int KEY_APPLICATION = SDL_SCANCODE_APPLICATION;
	static const unsigned int KEY_AUDIOMUTE = SDL_SCANCODE_AUDIOMUTE;
	static const unsigned int KEY_AUDIONEXT = SDL_SCANCODE_AUDIONEXT;
	static const unsigned int KEY_AUDIOPLAY = SDL_SCANCODE_AUDIOPLAY;
	static const unsigned int KEY_AUDIOPREV = SDL_SCANCODE_AUDIOPREV;
	static const unsigned int KEY_AUDIOSTOP = SDL_SCANCODE_AUDIOSTOP;
	static const unsigned int KEY_BACKSLASH = SDL_SCANCODE_BACKSLASH;
	static const unsigned int KEY_BACKSPACE = SDL_SCANCODE_BACKSPACE;
	static const unsigned int KEY_BRIGHTNESSDOWN = SDL_SCANCODE_BRIGHTNESSDOWN;
	static const unsigned int KEY_BRIGHTNESSUP = SDL_SCANCODE_BRIGHTNESSUP;
	static const unsigned int KEY_CALCULATOR = SDL_SCANCODE_CALCULATOR;
	static const unsigned int KEY_CANCEL = SDL_SCANCODE_CANCEL;
	static const unsigned int KEY_CAPSLOCK = SDL_SCANCODE_CAPSLOCK;
	static const unsigned int KEY_CLEAR = SDL_SCANCODE_CLEAR;
	static const unsigned int KEY_CLEARAGAIN = SDL_SCANCODE_CLEARAGAIN;
	static const unsigned int KEY_COMMA = SDL_SCANCODE_COMMA;
	static const unsigned int KEY_COMPUTER = SDL_SCANCODE_COMPUTER;
	static const unsigned int KEY_COPY = SDL_SCANCODE_COPY;
	static const unsigned int KEY_CRSEL = SDL_SCANCODE_CRSEL;
	static const unsigned int KEY_CURRENCYSUBUNIT = SDL_SCANCODE_CURRENCYSUBUNIT;
	static const unsigned int KEY_CURRENCYUNIT = SDL_SCANCODE_CURRENCYUNIT;
	static const unsigned int KEY_CUT = SDL_SCANCODE_CUT;
	static const unsigned int KEY_DECIMALSEPARATOR = SDL_SCANCODE_DECIMALSEPARATOR;
	static const unsigned int KEY_DELETE = SDL_SCANCODE_DELETE;
	static const unsigned int KEY_DISPLAYSWITCH = SDL_SCANCODE_DISPLAYSWITCH;
	static const unsigned int KEY_DOWN = SDL_SCANCODE_DOWN;
	static const unsigned int KEY_EJECT = SDL_SCANCODE_EJECT;
	static const unsigned int KEY_END = SDL_SCANCODE_END;
	static const unsigned int KEY_EQUALS = SDL_SCANCODE_EQUALS;
	static const unsigned int KEY_ESCAPE = SDL_SCANCODE_ESCAPE;
	static const unsigned int KEY_EXECUTE = SDL_SCANCODE_EXECUTE;
	static const unsigned int KEY_EXSEL = SDL_SCANCODE_EXSEL;

	static const unsigned int KEY_FIND = SDL_SCANCODE_FIND;
	static const unsigned int KEY_GRAVE = SDL_SCANCODE_GRAVE;
	static const unsigned int KEY_HELP = SDL_SCANCODE_HELP;
	static const unsigned int KEY_HOME = SDL_SCANCODE_HOME;
	static const unsigned int KEY_INSERT = SDL_SCANCODE_INSERT;
	static const unsigned int KEY_KBDILLUMDOWN = SDL_SCANCODE_KBDILLUMDOWN;
	static const unsigned int KEY_KBDILLUMTOGGLE = SDL_SCANCODE_KBDILLUMTOGGLE;
	static const unsigned int KEY_KBDILLUMUP = SDL_SCANCODE_KBDILLUMUP;
	static const unsigned int KEY_KP_0 = SDL_SCANCODE_KP_0;
	static const unsigned int KEY_KP_00 = SDL_SCANCODE_KP_00;
	static const unsigned int KEY_KP_000 = SDL_SCANCODE_KP_000;
	static const unsigned int KEY_KP_1 = SDL_SCANCODE_KP_1;
	static const unsigned int KEY_KP_2 = SDL_SCANCODE_KP_2;
	static const unsigned int KEY_KP_3 = SDL_SCANCODE_KP_3;
	static const unsigned int KEY_KP_4 = SDL_SCANCODE_KP_4;
	static const unsigned int KEY_KP_5 = SDL_SCANCODE_KP_5;
	static const unsigned int KEY_KP_6 = SDL_SCANCODE_KP_6;
	static const unsigned int KEY_KP_7 = SDL_SCANCODE_KP_7;
	static const unsigned int KEY_KP_8 = SDL_SCANCODE_KP_8;
	static const unsigned int KEY_KP_9 = SDL_SCANCODE_KP_9;
	static const unsigned int KEY_KP_A = SDL_SCANCODE_KP_A;
	static const unsigned int KEY_KP_AMPERSAND = SDL_SCANCODE_KP_AMPERSAND;
	static const unsigned int KEY_KP_AT = SDL_SCANCODE_KP_AT;
	static const unsigned int KEY_KP_B = SDL_SCANCODE_KP_B;
	static const unsigned int KEY_KP_BACKSPACE = SDL_SCANCODE_KP_BACKSPACE;
	static const unsigned int KEY_KP_BINARY = SDL_SCANCODE_KP_BINARY;
	static const unsigned int KEY_KP_C = SDL_SCANCODE_KP_C;
	static const unsigned int KEY_KP_CLEAR = SDL_SCANCODE_KP_CLEAR;
	static const unsigned int KEY_KP_CLEARENTRY = SDL_SCANCODE_KP_CLEARENTRY;
	static const unsigned int KEY_KP_COLON = SDL_SCANCODE_KP_COLON;
	static const unsigned int KEY_KP_COMMA = SDL_SCANCODE_KP_COMMA;
	static const unsigned int KEY_KP_D = SDL_SCANCODE_KP_D;
	static const unsigned int KEY_KP_DBLAMPERSAND = SDL_SCANCODE_KP_DBLAMPERSAND;
	static const unsigned int KEY_KP_DECIMAL = SDL_SCANCODE_KP_DECIMAL;
	static const unsigned int KEY_KP_DIVIDE = SDL_SCANCODE_KP_DIVIDE;
	static const unsigned int KEY_KP_E = SDL_SCANCODE_KP_E;
	static const unsigned int KEY_KP_ENTER = SDL_SCANCODE_KP_ENTER;
	static const unsigned int KEY_KP_EQUALS = SDL_SCANCODE_KP_EQUALS;
	static const unsigned int KEY_KP_EQUALSAS400 = SDL_SCANCODE_KP_EQUALSAS400;
	static const unsigned int KEY_KP_EXCLAM = SDL_SCANCODE_KP_EXCLAM;
	static const unsigned int KEY_KP_F = SDL_SCANCODE_KP_F;
	static const unsigned int KEY_KP_GREATER = SDL_SCANCODE_KP_GREATER;
	static const unsigned int KEY_KP_HASH = SDL_SCANCODE_KP_HASH;
	static const unsigned int KEY_KP_HEXADECIMAL = SDL_SCANCODE_KP_HEXADECIMAL;
	static const unsigned int KEY_KP_LEFTBRACE = SDL_SCANCODE_KP_LEFTBRACE;
	static const unsigned int KEY_KP_LEFTPAREN = SDL_SCANCODE_KP_LEFTPAREN;
	static const unsigned int KEY_KP_LESS = SDL_SCANCODE_KP_LESS;
	static const unsigned int KEY_KP_MEMADD = SDL_SCANCODE_KP_MEMADD;
	static const unsigned int KEY_KP_MEMCLEAR = SDL_SCANCODE_KP_MEMCLEAR;
	static const unsigned int KEY_KP_MEMDIVIDE = SDL_SCANCODE_KP_MEMDIVIDE;
	static const unsigned int KEY_KP_MEMMULTIPLY = SDL_SCANCODE_KP_MEMMULTIPLY;
	static const unsigned int KEY_KP_MEMRECALL = SDL_SCANCODE_KP_MEMRECALL;
	static const unsigned int KEY_KP_MEMSTORE = SDL_SCANCODE_KP_MEMSTORE;
	static const unsigned int KEY_KP_MEMSUBTRACT = SDL_SCANCODE_KP_MEMSUBTRACT;
	static const unsigned int KEY_KP_MINUS = SDL_SCANCODE_KP_MINUS;
	static const unsigned int KEY_KP_MULTIPLY = SDL_SCANCODE_KP_MULTIPLY;
	static const unsigned int KEY_KP_OCTAL = SDL_SCANCODE_KP_OCTAL;
	static const unsigned int KEY_KP_PERCENT = SDL_SCANCODE_KP_PERCENT;
	static const unsigned int KEY_KP_PERIOD = SDL_SCANCODE_KP_PERIOD;
	static const unsigned int KEY_KP_PLUS = SDL_SCANCODE_KP_PLUS;
	static const unsigned int KEY_KP_PLUSMINUS = SDL_SCANCODE_KP_PLUSMINUS;
	static const unsigned int KEY_KP_POWER = SDL_SCANCODE_KP_POWER;
	static const unsigned int KEY_KP_RIGHTBRACE = SDL_SCANCODE_KP_RIGHTBRACE;
	static const unsigned int KEY_KP_RIGHTPAREN = SDL_SCANCODE_KP_RIGHTPAREN;
	static const unsigned int KEY_KP_SPACE = SDL_SCANCODE_KP_SPACE;
	static const unsigned int KEY_KP_TAB = SDL_SCANCODE_KP_TAB;
	static const unsigned int KEY_KP_VERTICALBAR = SDL_SCANCODE_KP_VERTICALBAR;
	static const unsigned int KEY_KP_XOR = SDL_SCANCODE_KP_XOR;
	static const unsigned int KEY_LALT = SDL_SCANCODE_LALT;
	static const unsigned int KEY_LCTRL = SDL_SCANCODE_LCTRL;
	static const unsigned int KEY_LEFT = SDL_SCANCODE_LEFT;
	static const unsigned int KEY_LEFTBRACKET = SDL_SCANCODE_LEFTBRACKET;
	static const unsigned int KEY_LGUI = SDL_SCANCODE_LGUI;
	static const unsigned int KEY_LSHIFT = SDL_SCANCODE_LSHIFT;
	static const unsigned int KEY_MAIL = SDL_SCANCODE_MAIL;
	static const unsigned int KEY_MEDIASELECT = SDL_SCANCODE_MEDIASELECT;
	static const unsigned int KEY_MENU = SDL_SCANCODE_MENU;
	static const unsigned int KEY_MINUS = SDL_SCANCODE_MINUS;
	static const unsigned int KEY_MODE = SDL_SCANCODE_MODE;
	static const unsigned int KEY_MUTE = SDL_SCANCODE_MUTE;
	static const unsigned int KEY_NUMLOCKCLEAR = SDL_SCANCODE_NUMLOCKCLEAR;
	static const unsigned int KEY_OPER = SDL_SCANCODE_OPER;
	static const unsigned int KEY_OUT = SDL_SCANCODE_OUT;
	static const unsigned int KEY_PAGEDOWN = SDL_SCANCODE_PAGEDOWN;
	static const unsigned int KEY_PAGEUP = SDL_SCANCODE_PAGEUP;
	static const unsigned int KEY_PASTE = SDL_SCANCODE_PASTE;
	static const unsigned int KEY_PAUSE = SDL_SCANCODE_PAUSE;
	static const unsigned int KEY_PERIOD = SDL_SCANCODE_PERIOD;
	static const unsigned int KEY_POWER = SDL_SCANCODE_POWER;
	static const unsigned int KEY_PRINTSCREEN = SDL_SCANCODE_PRINTSCREEN;
	static const unsigned int KEY_PRIOR = SDL_SCANCODE_PRIOR;
	static const unsigned int KEY_RALT = SDL_SCANCODE_RALT;
	static const unsigned int KEY_RCTRL = SDL_SCANCODE_RCTRL;
	static const unsigned int KEY_RETURN = SDL_SCANCODE_RETURN;
	static const unsigned int KEY_RETURN2 = SDL_SCANCODE_RETURN2;
	static const unsigned int KEY_RGUI = SDL_SCANCODE_RGUI;
	static const unsigned int KEY_RIGHT = SDL_SCANCODE_RIGHT;
	static const unsigned int KEY_RIGHTBRACKET = SDL_SCANCODE_RIGHTBRACKET;
	static const unsigned int KEY_RSHIFT = SDL_SCANCODE_RSHIFT;
	static const unsigned int KEY_SCROLLLOCK = SDL_SCANCODE_SCROLLLOCK;
	static const unsigned int KEY_SELECT = SDL_SCANCODE_SELECT;
	static const unsigned int KEY_SEMICOLON = SDL_SCANCODE_SEMICOLON;
	static const unsigned int KEY_SEPARATOR = SDL_SCANCODE_SEPARATOR;
	static const unsigned int KEY_SLASH = SDL_SCANCODE_SLASH;
	static const unsigned int KEY_SLEEP = SDL_SCANCODE_SLEEP;
	static const unsigned int KEY_SPACE = SDL_SCANCODE_SPACE;
	static const unsigned int KEY_STOP = SDL_SCANCODE_STOP;
	static const unsigned int KEY_SYSREQ = SDL_SCANCODE_SYSREQ;
	static const unsigned int KEY_TAB = SDL_SCANCODE_TAB;
	static const unsigned int KEY_THOUSANDSSEPARATOR = SDL_SCANCODE_THOUSANDSSEPARATOR;
	static const unsigned int KEY_UNDO = SDL_SCANCODE_UNDO;
	static const unsigned int KEY_UNKNOWN = SDL_SCANCODE_UNKNOWN;
	static const unsigned int KEY_UP = SDL_SCANCODE_UP;
	static const unsigned int KEY_VOLUMEDOWN = SDL_SCANCODE_VOLUMEDOWN;
	static const unsigned int KEY_VOLUMEUP = SDL_SCANCODE_VOLUMEUP;
	static const unsigned int KEY_WWW = SDL_SCANCODE_WWW;
	static const unsigned int KEY_INTERNATIONAL1 = SDL_SCANCODE_INTERNATIONAL1;
	static const unsigned int KEY_INTERNATIONAL2 = SDL_SCANCODE_INTERNATIONAL2;
	static const unsigned int KEY_INTERNATIONAL3 = SDL_SCANCODE_INTERNATIONAL3;
	static const unsigned int KEY_INTERNATIONAL4 = SDL_SCANCODE_INTERNATIONAL4;
	static const unsigned int KEY_INTERNATIONAL5 = SDL_SCANCODE_INTERNATIONAL5;
	static const unsigned int KEY_INTERNATIONAL6 = SDL_SCANCODE_INTERNATIONAL6;
	static const unsigned int KEY_INTERNATIONAL7 = SDL_SCANCODE_INTERNATIONAL7;
	static const unsigned int KEY_INTERNATIONAL8 = SDL_SCANCODE_INTERNATIONAL8;
	static const unsigned int KEY_INTERNATIONAL9 = SDL_SCANCODE_INTERNATIONAL9;
	static const unsigned int KEY_LANG1 = SDL_SCANCODE_LANG1;
	static const unsigned int KEY_LANG2 = SDL_SCANCODE_LANG2;
	static const unsigned int KEY_LANG3 = SDL_SCANCODE_LANG3;
	static const unsigned int KEY_LANG4 = SDL_SCANCODE_LANG4;
	static const unsigned int KEY_LANG5 = SDL_SCANCODE_LANG5;
	static const unsigned int KEY_LANG6 = SDL_SCANCODE_LANG6;
	static const unsigned int KEY_LANG7 = SDL_SCANCODE_LANG7;
	static const unsigned int KEY_LANG8 = SDL_SCANCODE_LANG8;
	static const unsigned int KEY_LANG9 = SDL_SCANCODE_LANG9;
	static const unsigned int KEY_NONUSBACKSLASH = SDL_SCANCODE_NONUSBACKSLASH;
	static const unsigned int KEY_NONUSHASH = SDL_SCANCODE_NONUSHASH;
};
