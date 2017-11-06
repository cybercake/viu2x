// viu2x.cpp : Defines the entry point for the application.
//

// viu2x.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "../components/viu2xCore/common.h"
#include "../components/viu2xCore/gui.h"

using namespace v2x;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	// This must be the first line to use viu2x.
	App app;

	// Create and show a window
	Window::Shared mainWindow(new Window());
	mainWindow->show();

	// Block until the window is closed.
	app.waitUntilTermination();
}