#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/HID/WindowSystem.h"

#include "NexusEngine/External/Glfw/Glfw.h"
#include "NexusEngine/Systems/Resources/Resources/Image.h"

namespace NxEn
{
	static SettingVar<NxFr::Vector2i>* SettingResolution = SettingVar<NxFr::Vector2i>::Create("Settings", "WindowResolution", NxFr::Vector2i(1920, 1080));
	static SettingVar<float>* SettingMode = SettingVar<float>::Create("Settings", "WindowMode", 1);
	static SettingVar<float>* SettingMonitor = SettingVar<float>::Create("Settings", "WindowMonitor", 1);
	static SettingVar<bool>* SettingVSync = SettingVar<bool>::Create("Settings", "WindowVSync", true);

	WindowSystem::WindowSystem()
		: OnClose(), OnMove(), OnResize(), OnFocus(), Monitors(), Target(), Pointer(), Focused(true)
	{
		Target.WindowMode = Window::Mode::Windowed;
		Target.Monitor = -1;
		Target.RefreshRate = 0;
		Target.VSync = true;
		Target.Position = NxFr::Vector2i(50);
		Target.Resolution = NxFr::Vector2i(1920, 1080);
		Target.Title = "Nexus";
		Target.Icon = nullptr;

		Pointer.CursorMode = Cursor::Mode::Default;
		Pointer.CursorIcon = Cursor::Icon::Default;

		OnFocus += { this, & WindowSystem::OnFocused };
		OnMove += { this, & WindowSystem::OnMoved };
		OnResize += { this, & WindowSystem::OnResized };
	}

	WindowSystem::~WindowSystem()
	{
		OnFocus.Clear();
		OnMove.Clear();
		OnResize.Clear();
	}

	void WindowSystem::Close()
	{
		if (Target.Instance)
		{
			Glfw::CloseWindow(Target.Instance);
		}
	}

	void WindowSystem::Minimize()
	{
		if (Target.Instance)
		{
			Glfw::MinimizeWindow(Target.Instance);
		}
	}

	void WindowSystem::Maximize()
	{
		if (Target.Instance)
		{
			Glfw::MaximizeWindow(Target.Instance);
		}
	}

	void WindowSystem::Restore()
	{
		if (Target.Instance)
		{
			Glfw::RestoreWindow(Target.Instance);
		}
	}

	void WindowSystem::Show()
	{
		if (Target.Instance)
		{
			Glfw::ShowWindow(Target.Instance);
		}
	}

	void WindowSystem::Hide()
	{
		if (Target.Instance)
		{
			Glfw::HideWindow(Target.Instance);
		}
	}

	void WindowSystem::Focus()
	{
		if (Target.Instance)
		{
			Glfw::FocusWindow(Target.Instance);
		}
	}

	WindowSystem& WindowSystem::SetWindowMode(Window::Mode Mode)
	{
#if NX_EDITOR
		NX_LOG(Warning, System, "In Editor SetWindowMode won't have an impact");
		return *this;
#endif

		Target.WindowMode = Mode;
		Target.Monitor = Target.WindowMode != Window::Mode::Windowed ? NxFr::Math::Max(Target.Monitor, (int8)0) : -1;
		if (Target.Instance)
		{
			Glfw::SetWindowMode(Target.Instance,
				(uint8)Target.WindowMode, Target.Monitor >= 0 ? Monitors[Target.Monitor].Instance : nullptr,
				Target.Position, Target.Resolution, Target.RefreshRate);
		}

		return *this;
	}

	WindowSystem& WindowSystem::SetWindowMonitor(int8 Index)
	{
#if NX_EDITOR
		NX_LOG(Warning, System, "In Editor SetWindowMonitor won't have an impact");
		return *this;
#endif

		if (Target.WindowMode == Window::Mode::Windowed)
		{
			NX_LOG(Warning, System, "Window is in windowed mode, SetWindowMonitor won't have an impact");
			return *this;
		}

		if (!Monitors.IsValidIndex(Index))
		{
			NX_LOG(Error, System, "Index %d is not a valid monitor index", Index);
			return *this;
		}

		Target.Monitor = NxFr::Math::Max(Index, (int8)0);
		if (Target.Instance)
		{
			Glfw::SetWindowMode(Target.Instance,
				(uint8)Target.WindowMode, Target.Monitor >= 0 ? Monitors[Target.Monitor].Instance : nullptr,
				Target.Position, Target.Resolution, Target.RefreshRate);
		}

		return *this;
	}

	WindowSystem& WindowSystem::SetWindowRefreshRate(uint64 RefreshRate)
	{
#if NX_EDITOR
		NX_LOG(Warning, System, "In Editor SetWindowRefreshRate won't have an impact");
		return *this;
#endif

		if (Target.WindowMode != Window::Mode::FullScreen)
		{
			NX_LOG(Warning, System, "Window is not in fullscreen mode, SetWindowRefreshRate won't have an impact");
			return *this;
		}

		Target.RefreshRate = RefreshRate;
		if (Target.Instance)
		{
			Glfw::SetWindowMode(Target.Instance,
				(uint8)Target.WindowMode, Target.Monitor >= 0 ? Monitors[Target.Monitor].Instance : nullptr,
				Target.Position, Target.Resolution, Target.RefreshRate);
		}

		return *this;
	}

	WindowSystem& WindowSystem::SetWindowVSync(bool VSync)
	{
		Target.VSync = VSync;
		if (Target.Instance)
		{
			Glfw::SetSwapInterval(VSync);
		}

		return *this;
	}

	WindowSystem& WindowSystem::SetWindowPosition(NxFr::Vector2i Position)
	{
		if (Target.WindowMode != Window::Mode::Windowed)
		{
			NX_LOG(Warning, System, "Window is not in windowed mode, SetWindowPosition won't have an impact");
			return *this;
		}

		Target.Position = Position;
		if (Target.Instance)
		{
			Glfw::SetWindowPosition(Target.Instance, Position);
		}

		return *this;
	}

	WindowSystem& WindowSystem::SetWindowResolution(NxFr::Vector2i Resolution)
	{
		Target.Resolution = Resolution;
		if (Target.Instance)
		{
			Glfw::SetWindowSize(Target.Instance, Resolution);
		}

		return *this;
	}

	WindowSystem& WindowSystem::SetWindowTitle(NxFr::StringView Title)
	{
		Target.Title = Title;
		if (Target.Instance)
		{
			Glfw::SetWindowTitle(Target.Instance, Title);
		}

		return *this;
	}

	WindowSystem& WindowSystem::SetWindowIcon(const Image* Icon)
	{
		Target.Icon = Icon;
		if (Target.Instance)
		{
			Glfw::SetWindowIcon(Target.Instance,
				Icon ? Icon->GetResolution() : NxFr::Vector2i::Zero,
				Icon ? static_cast<uint8*>(Icon->GetPixels()) : nullptr);
		}

		return *this;
	}

	WindowSystem& WindowSystem::SetCursorMode(Cursor::Mode Mode)
	{
		if (Pointer.CursorMode == Mode)
		{
			return *this;
		}

		Pointer.CursorMode = Mode;
		if (Target.Instance && Pointer.Instance)
		{
			Glfw::SetCursorMode(Target.Instance, (uint32)Pointer.CursorMode);
		}

		return *this;
	}

	WindowSystem& WindowSystem::SetCursorIcon(Cursor::Icon Icon, const Image* IconCustom)
	{
		if (Pointer.CursorIcon == Icon && Pointer.IconCustom == IconCustom)
		{
			return *this;
		}

		Pointer.CursorIcon = Icon;
		Pointer.IconCustom = IconCustom;
		if (Target.Instance && Pointer.Instance)
		{
			DestroyCursor();
			CreateCursor();
		}

		return *this;
	}

	void WindowSystem::OnInitialize()
	{
		System::OnInitialize();

		Application::GetSystem<SettingsSystem>()->GetOnChange() += { this, &WindowSystem::ApplySettings };
		Application::GetSystem<InputSystem>()->GetOnPoll() += Glfw::PollInput;

		if (Application::GetInstance<NexusEngineApplication>()->IsHeadless())
		{
			return;
		}

		Glfw::Initialize();

		FetchMonitors();
		CreateWindow();
		SetWindowVSync(Target.VSync);
		SetWindowMode(Target.WindowMode);
		SetWindowPosition(Target.Position);
		SetWindowResolution(Target.Resolution);
		SetWindowTitle(Target.Title);
		SetWindowIcon(Target.Icon);
		CreateCursor();
		SetCursorMode(Pointer.CursorMode);

		NX_LOG(Info, System, "Window created with resolution %d-%d", Target.Resolution.x, Target.Resolution.y);
	}

	void WindowSystem::OnShutdown()
	{
		System::OnShutdown();

		Application::GetSystem<SettingsSystem>()->GetOnChange() -= { this, &WindowSystem::ApplySettings };
		Application::GetSystem<InputSystem>()->GetOnPoll() -= Glfw::PollInput;

		if (Application::GetInstance<NexusEngineApplication>()->IsHeadless())
		{
			return;
		}

		DestroyCursor();
		DestroyWindow();

		Glfw::Shutdown();
	}

	void WindowSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		if (Application::GetInstance<NexusEngineApplication>()->IsHeadless())
		{
			return;
		}

		TickWindow();
	}

	void WindowSystem::OnMoved(NxFr::Vector2i Position)
	{
		Target.Position = Position;
	}

	void WindowSystem::OnResized(NxFr::Vector2i Size)
	{
		Target.Resolution = Size;
	}

	void WindowSystem::OnFocused(bool Focus)
	{
		Focused = Focus;
	}

	void WindowSystem::FetchMonitors()
	{
		NX_INSTUMENT_FUNCTION();

		NxFr::Array<void*> Instances = Glfw::GetMonitors();

		Monitors = NxFr::Array<Monitor>(Instances.GetCount());
		for (uint64 Index = 0; Index < Instances.GetCount(); ++Index)
		{
			Monitors[Index].Instance = Instances[Index];
			Glfw::GetMonitorSettings(Monitors[Index].Instance, Monitors[Index].Resolution.x, Monitors[Index].Resolution.y, Monitors[Index].RefreshRate);

			NX_LOG(Info, System, "Monitor found with resolution %d-%d", Monitors[Index].Resolution.x, Monitors[Index].Resolution.y);
		}
	}

	void WindowSystem::CreateWindow()
	{
		NX_INSTUMENT_FUNCTION();

		Target.Instance = Glfw::CreateWindow();
	}

	void WindowSystem::DestroyWindow()
	{
		NX_INSTUMENT_FUNCTION();

		Glfw::DestroyWindow(Target.Instance);
	}

	void WindowSystem::TickWindow()
	{
		NX_INSTUMENT_FUNCTION();

		Glfw::TickWindow(Target.Instance);
	}

	void WindowSystem::CreateCursor()
	{
		NX_INSTUMENT_FUNCTION();

		Glfw::CreateCursor(Target.Instance,
			(uint8)Pointer.CursorIcon,
			Pointer.IconCustom ? Pointer.IconCustom->GetResolution() : NxFr::Vector2i::Zero,
			Pointer.IconCustom ? static_cast<uint8*>(Pointer.IconCustom->GetPixels()) : nullptr);
	}

	void WindowSystem::DestroyCursor()
	{
		NX_INSTUMENT_FUNCTION();

		Glfw::DestroyCursor(Pointer.Instance);
	}

	void WindowSystem::ApplySettings()
	{
#if !NX_EDITOR
		SetWindowMode((Window::Mode)((uint8)SettingMode->GetValue()));
		SetWindowMonitor((uint8)SettingMonitor->GetValue());
#endif
		SetWindowResolution(SettingResolution->GetValue());
		SetWindowVSync(SettingVSync->GetValue());
	}
}
