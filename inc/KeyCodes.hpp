#pragma once

enum class KeyCodes {
    KEY_SPACE = 0,
    KEY_APOSTROPHE,
    KEY_COMMA,
    KEY_MINUS,
    KEY_PERIOD,
    KEY_SLASH,
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_SEMICOLON,
    KEY_EQUAL,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_LEFT_BRACKET,
    KEY_BACKSLASH,
    KEY_GRAVE_ACCENT,
    KEY_ESCAPE,
    KEY_ENTER,
    KEY_TAB,
    KEY_BACKSPACE,
    KEY_INSERT,
    KEY_DELETE,
    KEY_RIGHT,
    KEY_LEFT,
    KEY_DOWN,
    KEY_UP,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    KEY_HOME,
    KEY_END,
    KEY_CAPS_LOCK,
    KEY_SCROLL_LOCK,
    KEY_NUM_LOCK,
    KEY_PRINT_SCREEN,
    KEY_PAUSE,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_KP_0,
    KEY_KP_1,
    KEY_KP_2,
    KEY_KP_3,
    KEY_KP_4,
    KEY_KP_5,
    KEY_KP_6,
    KEY_KP_7,
    KEY_KP_8,
    KEY_KP_9,
    KEY_KP_DECIMAL,
    KEY_KP_DIVIDE,
    KEY_KP_MULTIPLY,
    KEY_KP_SUBTRACT,
    KEY_KP_ADD,
    KEY_KP_ENTER,
    KEY_KP_EQUAL,
    KEY_LEFT_SHIFT,
    KEY_LEFT_CONTROL,
    KEY_LEFT_ALT,
    KEY_LEFT_SUPER,
    KEY_RIGHT_SHIFT,
    KEY_RIGHT_CONTROL,
    KEY_RIGHT_ALT,
    KEY_RIGHT_SUPER,
    KEY_MENU,

    KEY_COUNT
};

inline int mapKeyCodeToGLFWKey(const KeyCodes keyCode)
{
    switch(keyCode) {
    case KeyCodes::KEY_SPACE: return GLFW_KEY_SPACE;
    case KeyCodes::KEY_APOSTROPHE: return GLFW_KEY_APOSTROPHE;
    case KeyCodes::KEY_COMMA: return GLFW_KEY_COMMA;
    case KeyCodes::KEY_MINUS: return GLFW_KEY_MINUS;
    case KeyCodes::KEY_PERIOD: return GLFW_KEY_PERIOD;
    case KeyCodes::KEY_SLASH: return GLFW_KEY_SLASH;
    case KeyCodes::KEY_0: return GLFW_KEY_0;
    case KeyCodes::KEY_1: return GLFW_KEY_1;
    case KeyCodes::KEY_2: return GLFW_KEY_2;
    case KeyCodes::KEY_3: return GLFW_KEY_3;
    case KeyCodes::KEY_4: return GLFW_KEY_4;
    case KeyCodes::KEY_5: return GLFW_KEY_5;
    case KeyCodes::KEY_6: return GLFW_KEY_6;
    case KeyCodes::KEY_7: return GLFW_KEY_7;
    case KeyCodes::KEY_8: return GLFW_KEY_8;
    case KeyCodes::KEY_9: return GLFW_KEY_9;
    case KeyCodes::KEY_SEMICOLON: return GLFW_KEY_SEMICOLON;
    case KeyCodes::KEY_A: return GLFW_KEY_A;
    case KeyCodes::KEY_B: return GLFW_KEY_B;
    case KeyCodes::KEY_C: return GLFW_KEY_C;
    case KeyCodes::KEY_D: return GLFW_KEY_D;
    case KeyCodes::KEY_E: return GLFW_KEY_E;
    case KeyCodes::KEY_F: return GLFW_KEY_F;
    case KeyCodes::KEY_G: return GLFW_KEY_G;
    case KeyCodes::KEY_H: return GLFW_KEY_H;
    case KeyCodes::KEY_I: return GLFW_KEY_I;
    case KeyCodes::KEY_J: return GLFW_KEY_J;
    case KeyCodes::KEY_K: return GLFW_KEY_K;
    case KeyCodes::KEY_L: return GLFW_KEY_L;
    case KeyCodes::KEY_M: return GLFW_KEY_M;
    case KeyCodes::KEY_N: return GLFW_KEY_N;
    case KeyCodes::KEY_O: return GLFW_KEY_O;
    case KeyCodes::KEY_P: return GLFW_KEY_P;
    case KeyCodes::KEY_Q: return GLFW_KEY_Q;
    case KeyCodes::KEY_R: return GLFW_KEY_R;
    case KeyCodes::KEY_S: return GLFW_KEY_S;
    case KeyCodes::KEY_T: return GLFW_KEY_T;
    case KeyCodes::KEY_U: return GLFW_KEY_U;
    case KeyCodes::KEY_V: return GLFW_KEY_V;
    case KeyCodes::KEY_W: return GLFW_KEY_W;
    case KeyCodes::KEY_X: return GLFW_KEY_X;
    case KeyCodes::KEY_Y: return GLFW_KEY_Y;
    case KeyCodes::KEY_Z: return GLFW_KEY_Z;
    case KeyCodes::KEY_LEFT_BRACKET: return GLFW_KEY_LEFT_BRACKET;
    case KeyCodes::KEY_BACKSLASH: return GLFW_KEY_BACKSLASH;
    case KeyCodes::KEY_GRAVE_ACCENT: return GLFW_KEY_GRAVE_ACCENT;
    case KeyCodes::KEY_ESCAPE: return GLFW_KEY_ESCAPE;
    case KeyCodes::KEY_ENTER: return GLFW_KEY_ENTER;
    case KeyCodes::KEY_TAB: return GLFW_KEY_TAB;
    case KeyCodes::KEY_BACKSPACE: return GLFW_KEY_BACKSPACE;
    case KeyCodes::KEY_INSERT: return GLFW_KEY_INSERT;
    case KeyCodes::KEY_DELETE: return GLFW_KEY_DELETE;
    case KeyCodes::KEY_RIGHT: return GLFW_KEY_RIGHT;
    case KeyCodes::KEY_LEFT: return GLFW_KEY_LEFT;
    case KeyCodes::KEY_DOWN: return GLFW_KEY_DOWN;
    case KeyCodes::KEY_UP: return GLFW_KEY_UP;
    case KeyCodes::KEY_PAGE_UP: return GLFW_KEY_PAGE_UP;
    case KeyCodes::KEY_PAGE_DOWN: return GLFW_KEY_PAGE_DOWN;
    case KeyCodes::KEY_HOME: return GLFW_KEY_HOME;
    case KeyCodes::KEY_END: return GLFW_KEY_END;
    case KeyCodes::KEY_CAPS_LOCK: return GLFW_KEY_CAPS_LOCK;
    case KeyCodes::KEY_SCROLL_LOCK: return GLFW_KEY_SCROLL_LOCK;
    case KeyCodes::KEY_NUM_LOCK: return GLFW_KEY_NUM_LOCK;
    case KeyCodes::KEY_PRINT_SCREEN: return GLFW_KEY_PRINT_SCREEN;
    case KeyCodes::KEY_PAUSE: return GLFW_KEY_PAUSE;
    case KeyCodes::KEY_F1: return GLFW_KEY_F1;
    case KeyCodes::KEY_F2: return GLFW_KEY_F2;
    case KeyCodes::KEY_F3: return GLFW_KEY_F3;
    case KeyCodes::KEY_F4: return GLFW_KEY_F4;
    case KeyCodes::KEY_F5: return GLFW_KEY_F5;
    case KeyCodes::KEY_F6: return GLFW_KEY_F6;
    case KeyCodes::KEY_F7: return GLFW_KEY_F7;
    case KeyCodes::KEY_F8: return GLFW_KEY_F8;
    case KeyCodes::KEY_F9: return GLFW_KEY_F9;
    case KeyCodes::KEY_F10: return GLFW_KEY_F10;
    case KeyCodes::KEY_F11: return GLFW_KEY_F11;
    case KeyCodes::KEY_F12: return GLFW_KEY_F12;
    case KeyCodes::KEY_KP_0: return GLFW_KEY_KP_0;
    case KeyCodes::KEY_KP_1: return GLFW_KEY_KP_1;
    case KeyCodes::KEY_KP_2: return GLFW_KEY_KP_2;
    case KeyCodes::KEY_KP_3: return GLFW_KEY_KP_3;
    case KeyCodes::KEY_KP_4: return GLFW_KEY_KP_4;
    case KeyCodes::KEY_KP_5: return GLFW_KEY_KP_5;
    case KeyCodes::KEY_KP_6: return GLFW_KEY_KP_6;
    case KeyCodes::KEY_KP_7: return GLFW_KEY_KP_7;
    case KeyCodes::KEY_KP_8: return GLFW_KEY_KP_8;
    case KeyCodes::KEY_KP_9: return GLFW_KEY_KP_9;
    case KeyCodes::KEY_KP_DECIMAL: return GLFW_KEY_KP_DECIMAL;
    case KeyCodes::KEY_KP_DIVIDE: return GLFW_KEY_KP_DIVIDE;
    case KeyCodes::KEY_KP_MULTIPLY: return GLFW_KEY_KP_MULTIPLY;
    case KeyCodes::KEY_KP_SUBTRACT: return GLFW_KEY_KP_SUBTRACT;
    case KeyCodes::KEY_KP_ADD: return GLFW_KEY_KP_ADD;
    case KeyCodes::KEY_KP_ENTER: return GLFW_KEY_KP_ENTER;
    case KeyCodes::KEY_KP_EQUAL: return GLFW_KEY_KP_EQUAL;
    case KeyCodes::KEY_LEFT_SHIFT: return GLFW_KEY_LEFT_SHIFT;
    case KeyCodes::KEY_LEFT_CONTROL: return GLFW_KEY_LEFT_CONTROL;
    case KeyCodes::KEY_LEFT_ALT: return GLFW_KEY_LEFT_ALT;
    case KeyCodes::KEY_LEFT_SUPER: return GLFW_KEY_LEFT_SUPER;
    case KeyCodes::KEY_RIGHT_SHIFT: return GLFW_KEY_RIGHT_SHIFT;
    case KeyCodes::KEY_RIGHT_CONTROL: return GLFW_KEY_RIGHT_CONTROL;
    case KeyCodes::KEY_RIGHT_ALT: return GLFW_KEY_RIGHT_ALT;
    case KeyCodes::KEY_RIGHT_SUPER: return GLFW_KEY_RIGHT_SUPER;
    case KeyCodes::KEY_MENU: return GLFW_KEY_MENU;
    case KeyCodes::KEY_EQUAL: return GLFW_KEY_EQUAL;
    default: return GLFW_KEY_UNKNOWN;
    }
}
