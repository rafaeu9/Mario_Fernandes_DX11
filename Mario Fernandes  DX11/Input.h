#pragma once
#include <dinput.h>

class Input
{
public:

	HRESULT Initialise(HWND* hWnd, HINSTANCE* hInst);
	void Update();

	bool IsKeyPressed(unsigned char DI_keycode);
	~Input();
private:

	IDirectInput8* m_direct_input;
	IDirectInputDevice8* m_keyboard_device;
	unsigned char m_keyboard_keys_state[256];

	HWND* m_hWnd = NULL;
	HINSTANCE* m_hInst = NULL;
};

