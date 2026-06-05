#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/HID/Screen.h"

#include "NexusEngine/Systems/Resources/Resources/Image.h"

namespace NxEn
{
	Window::Window()
		: WindowMode(Window::Mode::FullScreen), Monitor(-1), Position(0), Resolution(0), Title(), Icon(nullptr), VSync(false), Instance(nullptr)
	{
	}

	Window::~Window()
	{
	}


	Monitor::Monitor()
		: Resolution(0), RefreshRate(0), Instance(nullptr)
	{
	}

	Monitor::~Monitor()
	{
	}

	Cursor::Cursor()
		: CursorMode(Mode::Default), CursorIcon(Icon::Default), IconCustom(nullptr), Instance(nullptr)
	{
	}

	Cursor::~Cursor()
	{
	}
}
