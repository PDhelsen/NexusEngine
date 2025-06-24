#pragma once

#include "NexusEngine/Systems/System.h"
#include "NexusEngine/Systems/HID/Screen.h"

namespace NxEn
{
	class WindowSystem : public System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, WindowSystem)

		NxFr::Event<> OnClose;
		NxFr::Event<bool> OnFocus;
		NxFr::Event<NxFr::Vector2i> OnMove;
		NxFr::Event<NxFr::Vector2i> OnResize;

		NEXUS_ENGINE_API WindowSystem();

		NEXUS_ENGINE_API void Close();
		NEXUS_ENGINE_API void Minimize();
		NEXUS_ENGINE_API void Maximize();
		NEXUS_ENGINE_API void Restore();
		NEXUS_ENGINE_API void Show();
		NEXUS_ENGINE_API void Hide();
		NEXUS_ENGINE_API void Focus();

		NEXUS_ENGINE_API WindowSystem& SetWindowMode(Window::Mode Mode);
		NEXUS_ENGINE_API WindowSystem& SetWindowMonitor(uint8 MonitorIndex);
		NEXUS_ENGINE_API WindowSystem& SetWindowPosition(NxFr::Vector2i Position);
		NEXUS_ENGINE_API WindowSystem& SetWindowResolution(NxFr::Vector2i Resolution);
		NEXUS_ENGINE_API WindowSystem& SetWindowTitle(NxFr::StringView Title);
		NEXUS_ENGINE_API WindowSystem& SetWindowIcon(void* Icon);
		NEXUS_ENGINE_API WindowSystem& SetWindowVSync(bool VSync);

		Window& GetWindow() { return Target; }
		Monitor& GetMonitor(uint8 Index = 0) { return Monitors[Index]; }

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;
		NEXUS_ENGINE_API void OnFocused(bool Focus);
		NEXUS_ENGINE_API void OnMoved(NxFr::Vector2i Position);
		NEXUS_ENGINE_API void OnResized(NxFr::Vector2i Size);

		NEXUS_ENGINE_API void FetchMonitors();
		NEXUS_ENGINE_API void CreateWindow();
		NEXUS_ENGINE_API void DestroyWindow();
		NEXUS_ENGINE_API void TickWindow();

	private:
		NxFr::Array<Monitor> Monitors;
		Window Target;
		bool Focused;
	};
}
