#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/HID/Screen.h"

namespace NxEn
{
	class WindowSystem : public System
	{
		friend class GUISystem;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, WindowSystem)

		NEXUS_ENGINE_API WindowSystem();
		NEXUS_ENGINE_API ~WindowSystem();

		NEXUS_ENGINE_API void Close();
		NEXUS_ENGINE_API void Minimize();
		NEXUS_ENGINE_API void Maximize();
		NEXUS_ENGINE_API void Restore();
		NEXUS_ENGINE_API void Show();
		NEXUS_ENGINE_API void Hide();
		NEXUS_ENGINE_API void Focus();

		NEXUS_ENGINE_API WindowSystem& SetWindowVSync(bool VSync);
		NEXUS_ENGINE_API WindowSystem& SetWindowMode(Window::Mode Mode);
		NEXUS_ENGINE_API WindowSystem& SetWindowMonitor(uint8 MonitorIndex);
		NEXUS_ENGINE_API WindowSystem& SetWindowPosition(NxFr::Vector2i Position);
		NEXUS_ENGINE_API WindowSystem& SetWindowResolution(NxFr::Vector2i Resolution);
		NEXUS_ENGINE_API WindowSystem& SetWindowTitle(NxFr::StringView Title);
		NEXUS_ENGINE_API WindowSystem& SetWindowIcon(void* Icon);
		NEXUS_ENGINE_API WindowSystem& SetCursorMode(Cursor::Mode Mode);
		NEXUS_ENGINE_API WindowSystem& SetCursorIcon(Cursor::Icon Icon, void* IconCustom = nullptr);

		NEXUS_ENGINE_API NxFr::Event<>& GetOnClose() { return OnClose; }
		NEXUS_ENGINE_API NxFr::Event<bool>& GetOnFocus() { return OnFocus; }
		NEXUS_ENGINE_API NxFr::Event<NxFr::Vector2i>& GetOnMove() { return OnMove; }
		NEXUS_ENGINE_API NxFr::Event<NxFr::Vector2i>& GetOnResize() { return OnResize; }

		NEXUS_ENGINE_API const Window& GetWindow() const { return Target; }
		NEXUS_ENGINE_API const Monitor& GetMonitor(uint8 Index = 0) const { return Monitors[Index]; }

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;
		NEXUS_ENGINE_API void OnFocused(bool Focus);
		NEXUS_ENGINE_API void OnMoved(NxFr::Vector2i Position);
		NEXUS_ENGINE_API void OnResized(NxFr::Vector2i Size);

	private:
		NEXUS_ENGINE_API void FetchMonitors();
		NEXUS_ENGINE_API void CreateWindow();
		NEXUS_ENGINE_API void DestroyWindow();
		NEXUS_ENGINE_API void TickWindow();
		NEXUS_ENGINE_API void UpdateWindow();
		NEXUS_ENGINE_API void UpdateCursor();
		NEXUS_ENGINE_API void ApplySettings();

	private:
		NxFr::Event<> OnClose;
		NxFr::Event<bool> OnFocus;
		NxFr::Event<NxFr::Vector2i> OnMove;
		NxFr::Event<NxFr::Vector2i> OnResize;

		NxFr::Array<Monitor> Monitors;
		Window Target;
		Cursor Pointer;
		bool Focused;
	};
}
