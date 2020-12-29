#include "Input.h"

HRESULT Input::Initialise(HWND* hWnd, HINSTANCE* hInst)
{
	m_hWnd = hWnd;
	m_hInst = hInst;
	if (m_hWnd == NULL || m_hInst == NULL) return E_FAIL;

	HRESULT hr = S_OK;
	ZeroMemory(m_keyboard_keys_state, sizeof(m_keyboard_keys_state));

	hr = DirectInput8Create(*m_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_direct_input, NULL);
	if (FAILED(hr)) return hr;

	hr = m_direct_input->CreateDevice(GUID_SysKeyboard, &m_keyboard_device, NULL);
	if (FAILED(hr)) return hr;

	hr = m_keyboard_device->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) return hr;

	hr = m_keyboard_device->SetCooperativeLevel(*m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr)) return hr;

	hr = m_keyboard_device->Acquire();
	if (FAILED(hr)) return hr;

	return hr;

}

void Input::Update()
{	

	HRESULT hr;
	hr = m_keyboard_device->GetDeviceState(sizeof(m_keyboard_keys_state), (LPVOID)&m_keyboard_keys_state);
	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_keyboard_device->Acquire();
		}
	}
	if (IsKeyPressed(DIK_ESCAPE)) DestroyWindow(*m_hWnd);

}

bool Input::IsKeyPressed(unsigned char DI_keycode)
{
	return m_keyboard_keys_state[DI_keycode] & 0x80;
}

Input::~Input()
{
	//if (m_keyboard_device)
	//{
	//	//m_keyboard_device->Unacquire();
	//	m_keyboard_device->Release();
	//}
	//if (m_direct_input) m_direct_input->Release();	


}


