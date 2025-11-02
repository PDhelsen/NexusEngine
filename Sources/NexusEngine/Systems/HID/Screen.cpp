#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/HID/Screen.h"

#include "NexusEngine/Systems/Resources/Resources/Image.h"

namespace NxEn
{
	Window::Window()
		: WindowMode(Window::Mode::FullScreen), Monitor(-1), Position(0), Resolution(0), Title(), Icon(nullptr), VSync(false), Instance(nullptr)
	{
	}

	Window::Window(Mode WindowMode, int8 Monitor, NxFr::Vector2i Position, NxFr::Vector2i Resolution, const NxFr::String& Title, Image* Icon, bool VSync)
		: WindowMode(WindowMode), Monitor(Monitor), Position(Position), Resolution(Resolution), Title(Title), Icon(Icon), VSync(VSync), Instance(nullptr)
	{
	}

	Window::~Window()
	{
	}


	Monitor::Monitor()
		: Resolution(0), RefreshRate(0), Instance(nullptr)
	{
	}

	Monitor::Monitor(NxFr::Vector2i Resolution, int32 RefreshRate)
		: Resolution(Resolution), RefreshRate(RefreshRate), Instance(nullptr)
	{
	}

	Monitor::~Monitor()
	{
	}

	Cursor::Cursor()
		: CursorMode(Mode::Default), CursorIcon(Icon::Default), IconCustom(nullptr), Instance(nullptr)
	{
	}

	Cursor::Cursor(Mode CursorMode, Icon CursorIcon, void* IconCustom)
		: CursorMode(CursorMode), CursorIcon(CursorIcon), IconCustom(IconCustom), Instance(nullptr)
	{
	}

	Cursor::~Cursor()
	{
	}
}
