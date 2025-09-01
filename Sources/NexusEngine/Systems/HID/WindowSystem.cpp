#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/HID/WindowSystem.h"

#include "NexusEngine/External/Glfw.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(WindowSystem)

	WindowSystem::WindowSystem() :
		OnClose(MemorySystem::GetAllocator(AllocatorType::General)),
		OnFocus(MemorySystem::GetAllocator(AllocatorType::General)),
		OnMove(MemorySystem::GetAllocator(AllocatorType::General)),
		OnResize(MemorySystem::GetAllocator(AllocatorType::General)),
		Monitors(5, MemorySystem::GetAllocator(AllocatorType::General)),
		Target(),
		Pointer(),
		Focused(true)
	{
		Target = Window(Window::Mode::Windowed, 1, NxFr::Vector2i(50), NxFr::Vector2i(1920, 1080), "Nexus", nullptr, true);
		OnFocus += NxFr::Delegate<void(bool)>(this, &WindowSystem::OnFocused);
		OnMove += NxFr::Delegate<void(NxFr::Vector2i)>(this, &WindowSystem::OnMoved);
		OnResize += NxFr::Delegate<void(NxFr::Vector2i)>(this, &WindowSystem::OnResized);
	}

	WindowSystem::~WindowSystem()
	{
		OnFocus -= NxFr::Delegate<void(bool)>(this, &WindowSystem::OnFocused);
		OnMove -= NxFr::Delegate<void(NxFr::Vector2i)>(this, &WindowSystem::OnMoved);
		OnResize -= NxFr::Delegate<void(NxFr::Vector2i)>(this, &WindowSystem::OnResized);
	}

	void WindowSystem::Close()
	{
		Glfw::CloseWindow(Target.Instance);
	}

	void WindowSystem::Minimize()
	{
		Glfw::MinimizeWindow(Target.Instance);
	}

	void WindowSystem::Maximize()
	{
		Glfw::MaximizeWindow(Target.Instance);
	}

	void WindowSystem::Restore()
	{
		Glfw::RestoreWindow(Target.Instance);
	}

	void WindowSystem::Show()
	{
		Glfw::ShowWindow(Target.Instance);
	}

	void WindowSystem::Hide()
	{
		Glfw::HideWindow(Target.Instance);
	}

	void WindowSystem::Focus()
	{
		Glfw::FocusWindow(Target.Instance);
	}

	WindowSystem& WindowSystem::SetWindowVSync(bool VSync)
	{
		Target.VSync = VSync;
		Glfw::SetSwapInterval(VSync);
		return *this;
	}

	WindowSystem& WindowSystem::SetWindowMode(Window::Mode Mode)
	{
		if (Target.WindowMode == Mode)
		{
			return *this;
		}

		Target.WindowMode = Mode;
		if (Target.IsValid())
		{
			DestroyWindow();
			CreateWindow();
		}

		return *this;
	}

	WindowSystem& WindowSystem::SetWindowMonitor(uint8 MonitorIndex)
	{
		if (Target.WindowMode == Window::Mode::Windowed || Target.Monitor == MonitorIndex)
		{
			return *this;
		}

		Target.Monitor = MonitorIndex;
		if (Target.IsValid())
		{
			DestroyWindow();
			CreateWindow();
		}

		return *this;
	}

	WindowSystem& WindowSystem::SetWindowPosition(NxFr::Vector2i Position)
	{
		if (Target.WindowMode != Window::Mode::Windowed)
		{
			return *this;
		}

		Target.Position = Position;
		Glfw::SetWindowPosition(Target.Instance, Position);
		return *this;
	}

	WindowSystem& WindowSystem::SetWindowResolution(NxFr::Vector2i Resolution)
	{
		if (Target.WindowMode != Window::Mode::Windowed)
		{
			return *this;
		}

		Target.Resolution = Resolution;
		Glfw::SetWindowSize(Target.Instance, Resolution);
		return *this;
	}

	WindowSystem& WindowSystem::SetWindowTitle(NxFr::StringView Title)
	{
		Target.Title = Title.ToString();
		Glfw::SetWindowTitle(Target.Instance, Title);
		return *this;
	}

	WindowSystem& WindowSystem::SetWindowIcon(void* Icon)
	{
		Target.Icon = Icon;
		Glfw::SetWindowIcon(Target.Instance, Icon);
		return *this;
	}

	WindowSystem& WindowSystem::SetCursorMode(Cursor::Mode Mode)
	{
		if (Pointer.CursorMode == Mode)
		{
			return *this;
		}

		Pointer.CursorMode = Mode;
		if (Target.IsValid() && Pointer.IsValid())
		{
			UpdateCursor();
		}

		return *this;
	}

	WindowSystem& WindowSystem::SetCursorIcon(Cursor::Icon Icon, void* IconCustom)
	{
		if (Pointer.CursorIcon == Icon && Pointer.IconCustom == IconCustom)
		{
			return *this;
		}

		Pointer.CursorIcon = Icon;
		Pointer.IconCustom = IconCustom;
		if (Target.IsValid() && Pointer.IsValid())
		{
			UpdateCursor();
		}

		return *this;
	}

	void WindowSystem::OnInitialize()
	{
		System::OnInitialize();

		Glfw::Initialize();

		FetchMonitors();
		CreateWindow();
		UpdateCursor();
	}

	void WindowSystem::OnShutdown()
	{
		SetCursorIcon(Cursor::Icon::Default);
		DestroyWindow();

		Glfw::Shutdown();

		System::OnShutdown();
	}

	void WindowSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);
		TickWindow();
	}

	void WindowSystem::OnFocused(bool Focus)
	{
		Focused = Focus;
	}

	void WindowSystem::OnMoved(NxFr::Vector2i Position)
	{
		Target.Position = Position;
	}

	void WindowSystem::OnResized(NxFr::Vector2i Size)
	{
		Target.Resolution = Size;
	}

	void WindowSystem::FetchMonitors()
	{
		NEXUS_PROFILE_FUNCTION();

		NxFr::Array<void*> Instances = Glfw::GetMonitors();

		Monitors = NxFr::Array<Monitor>(Instances.GetCount());
		for (uint64 Index = 0; Index < Instances.GetCount(); ++Index)
		{
			Monitors[Index].Instance = Instances[Index];
			Glfw::GetMonitorSettings(Monitors[Index].Instance, Monitors[Index].Resolution.x, Monitors[Index].Resolution.y, Monitors[Index].RefreshRate);

			NEXUS_LOG(Info, System, "Monitor found with resolution %d-%d", Monitors[Index].Resolution.x, Monitors[Index].Resolution.y);
		}
	}

	void WindowSystem::CreateWindow()
	{
		NEXUS_PROFILE_FUNCTION();

		Target.Instance = Glfw::CreateWindow((uint8)Target.WindowMode, Target.Monitor >= 0 ? Monitors[Target.Monitor].Instance : nullptr, Target.Position, Target.Resolution, Target.Title, Target.VSync);

		NEXUS_LOG(Info, System, "Window created with resolution %d-%d", Target.Resolution.x, Target.Resolution.y);
	}

	void WindowSystem::DestroyWindow()
	{
		NEXUS_PROFILE_FUNCTION();

		Glfw::DestroyWindow(Target.Instance);

		NEXUS_LOG(Info, System, "Window destroyed");
	}

	void WindowSystem::TickWindow()
	{
		NEXUS_PROFILE_FUNCTION();

		Glfw::TickWindow(Target.Instance);
	}

	void WindowSystem::UpdateCursor()
	{
		NEXUS_PROFILE_FUNCTION();

		Pointer.Instance = Glfw::UpdateCursorIcon(Target.Instance, Pointer.Instance, (uint8)Pointer.CursorIcon, Pointer.IconCustom);
		Glfw::SetCursorMode(Target.Instance, (uint32)Pointer.CursorMode);
	}
}
