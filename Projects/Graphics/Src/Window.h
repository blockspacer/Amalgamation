#pragma once

#include <Windows.h>
// =======================================================================================
//                                      Window
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # Window
/// Detailed description.....
/// Created on: 27-11-2012 
///---------------------------------------------------------------------------------------

static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

class Window
{
public:
	Window(HINSTANCE p_hInstance, int p_width, int p_height, int p_showWindowFlag);
	virtual ~Window();
	HWND getWindowRef();
	void changeWindowRes( int p_width, int p_height );
	void resize(int p_width, int p_height, bool p_setDirty=true );
	bool isDirty();
	int getWidth() {return m_width;}
	int getHeight() {return m_height;}
	void setAutoResize(bool p_mode);
	bool isAutoResizeEnabled() {return m_autoResize;}
	bool shutDownRequested() {return m_shutDownRequest;}
	void requestShutDown() {m_shutDownRequest=true;}
	bool isWindowRegistered() {return m_windowRegistered;}
	static Window* getInstance() {return m_instance;}
	void lockMouse();
	bool m_isFullscreen;
protected:
private:	
	bool m_autoResize;
	int m_width;
	int m_height;
	bool m_sizeIsDirty;
	bool m_shutDownRequest;
	bool m_windowRegistered;
	static Window* m_instance;
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
};