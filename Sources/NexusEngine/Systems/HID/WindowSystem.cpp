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
		: OnClose(), OnFocus(), OnMove(), OnResize(), Monitors(), Target(), Pointer(), Focused(true)
	{
		Target.WindowMode = Window::Mode::Windowed;
		Target.Position = NxFr::Vector2i(50);
		Target.Resolution = NxFr::Vector2i(1920, 1080);
		Target.Title = "Nexus";
		Target.VSync = true;

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

	WindowSystem& WindowSystem::SetWindowVSync(bool VSync)
	{
		Target.VSync = VSync;
		if (Target.Instance)
		{
			Glfw::SetSwapInterval(VSync);
		}

		return *this;
	}

	// TODO: Window switch mode at runtime
	WindowSystem& WindowSystem::SetWindowMode(Window::Mode Mode)
	{
		if (Target.Instance)
		{
			NX_LOG(Error, System, "Window is already created. SetWindowMode has to be called before the window creation and can't be called afterward");
		}
		else
		{
			Target.WindowMode = Mode;
		}

		return *this;
	}

	WindowSystem& WindowSystem::SetWindowMonitor(uint8 Index)
	{
		if (Target.Instance)
		{
			NX_LOG(Error, System, "Window is already created. SetWindowMonitor has to be called before the window creation and can't be called afterward");
		}
		else
		{
			Target.Monitor = Index;
		}

		return *this;
	}

	WindowSystem& WindowSystem::SetWindowPosition(NxFr::Vector2i Position)
	{
		if (Target.WindowMode != Window::Mode::Windowed)
		{
			NX_LOG(Warning, System, "Window is not in window mode editor, SetWindowPosition won't have an impact");
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
		if (Target.WindowMode != Window::Mode::Windowed)
		{
			return *this;
		}

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

	WindowSystem& WindowSystem::SetWindowIcon(Image* Icon)
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
			UpdateCursor();
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
			UpdateCursor();
		}

		return *this;
	}

	void WindowSystem::OnInitialize()
	{
		System::OnInitialize();

		Application::GetSystem<SettingsSystem>()->GetOnChange() += { this, &WindowSystem::ApplySettings };
		Application::GetSystem<InputSystem>()->GetOnPoll() += Glfw::PollInput;

		ApplySettings();

		if (Application::GetInstance<NexusEngineApplication>()->IsHeadless())
		{
			return;
		}

		Glfw::Initialize();
		FetchMonitors();
		CreateWindow();
		UpdateCursor();
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

		SetCursorIcon(Cursor::Icon::Default);
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

		Target.Instance = Glfw::CreateWindow(
			(uint8)Target.WindowMode, Target.Monitor >= 0 ? Monitors[Target.Monitor].Instance : nullptr,
			Target.Position, Target.Resolution, Target.Title,
			Target.Icon ? Target.Icon->GetResolution() : NxFr::Vector2i::Zero, Target.Icon ? static_cast<uint8*>(Target.Icon->GetPixels()) : nullptr,
			Target.VSync);

		NX_LOG(Info, System, "Window created with resolution %d-%d", Target.Resolution.x, Target.Resolution.y);
	}

	void WindowSystem::DestroyWindow()
	{
		NX_INSTUMENT_FUNCTION();

		Glfw::DestroyWindow(Target.Instance);

		NX_LOG(Info, System, "Window destroyed");
	}

	void WindowSystem::TickWindow()
	{
		NX_INSTUMENT_FUNCTION();

		Glfw::TickWindow(Target.Instance);
	}

	void WindowSystem::UpdateCursor()
	{
		NX_INSTUMENT_FUNCTION();

		Pointer.Instance = Glfw::UpdateCursorIcon(Target.Instance, Pointer.Instance,
			(uint8)Pointer.CursorIcon,
			Pointer.IconCustom ? Pointer.IconCustom->GetResolution() : NxFr::Vector2i::Zero,
			Pointer.IconCustom ? static_cast<uint8*>(Pointer.IconCustom->GetPixels()) : nullptr);
		Glfw::SetCursorMode(Target.Instance, (uint32)Pointer.CursorMode);
	}

	void WindowSystem::ApplySettings()
	{
		SetWindowResolution(SettingResolution->GetValue());
		SetWindowVSync(SettingVSync->GetValue());

#if !NX_EDITOR
		if (Target.Instance)
		{
			SetWindowMode((Window::Mode)((uint8)SettingMode->GetValue()));
			SetWindowMonitor((uint8)SettingMonitor->GetValue());
		}
#endif
	}
}
