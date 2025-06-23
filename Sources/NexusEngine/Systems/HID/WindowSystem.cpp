#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/HID/WindowSystem.h"

#include "NexusEngine/External/Glfw.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(WindowSystem)

	WindowSystem::WindowSystem()
		: OnClose(), OnFocus(), OnMove(), OnResize(), Position(50, 50), Resolution(1280, 720), Title("Nexus"), Focused(true), VSync(true), Icon(nullptr), Window(nullptr)
	{
		OnFocus += NxFr::Delegate<void(bool)>(this, &WindowSystem::OnFocused);
		OnMove += NxFr::Delegate<void(NxFr::Vector2i)>(this, &WindowSystem::OnMoved);
		OnResize += NxFr::Delegate<void(NxFr::Vector2i)>(this, &WindowSystem::OnResized);
	}

	void WindowSystem::Close()
	{
		Glfw::CloseWindow(Window);
	}

	void WindowSystem::Minimize()
	{
		Glfw::MinimizeWindow(Window);
	}

	void WindowSystem::Maximize()
	{
		Glfw::MaximizeWindow(Window);
	}

	void WindowSystem::Restore()
	{
		Glfw::RestoreWindow(Window);
	}

	void WindowSystem::Show()
	{
		Glfw::ShowWindow(Window);
	}

	void WindowSystem::Hide()
	{
		Glfw::HideWindow(Window);
	}

	void WindowSystem::Focus()
	{
		Glfw::FocusWindow(Window);
	}

	void WindowSystem::SetPosition(NxFr::Vector2i Position)
	{
		Glfw::SetWindowPosition(Window, Position);
	}

	void WindowSystem::SetResolution(NxFr::Vector2i Size)
	{
		Glfw::SetWindowSize(Window, Size);
	}

	void WindowSystem::SetTitle(NxFr::StringView Title)
	{
		this->Title = Title.ToString();
		Glfw::SetWindowTitle(Window, Title);
	}

	void WindowSystem::SetIcon(void* Icon)
	{
		this->Icon = Icon;
		Glfw::SetWindowIcon(Window, Icon);
	}

	void WindowSystem::SetVSync(bool VSync)
	{
		this->VSync = VSync;
		Glfw::SetSwapInterval(VSync);
	}

	void WindowSystem::OnInitialize()
	{
		System::OnInitialize();

		Window = Glfw::CreateWindow(Position, Resolution, Title.C(), VSync);
	}

	void WindowSystem::OnShutdown()
	{
		Glfw::DestroyWindow(Window);

		System::OnShutdown();
	}

	void WindowSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		Glfw::TickWindow(Window);
	}

	void WindowSystem::OnFocused(bool Focus)
	{
		this->Focused = Focus;
	}

	void WindowSystem::OnMoved(NxFr::Vector2i Position)
	{
		this->Position = Position;
	}

	void WindowSystem::OnResized(NxFr::Vector2i Size)
	{
		this->Resolution = Size;
	}
}
