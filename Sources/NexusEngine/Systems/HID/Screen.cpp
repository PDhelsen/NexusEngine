#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/HID/Screen.h"

#include "NexusEngine/Systems/Resources/Resources/Image.h"

namespace NxEn
{
	Monitor::Monitor()
		: Resolution(0), RefreshRate(0), Instance(nullptr)
	{
	}

	Monitor::~Monitor()
	{
	}

	Window::Window()
		: WindowMode(Window::Mode::Windowed), Monitor(-1), RefreshRate(0), VSync(false), Position(0), Resolution(0), Title(), Icon(nullptr), Instance(nullptr)
	{
	}

	Window::~Window()
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
