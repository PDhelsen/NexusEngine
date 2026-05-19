#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/HID/Screen.h"

namespace NxEn
{
	class Image;

	class WindowSystem : public System
	{
		friend class GUISystem;

	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, WindowSystem)

		NX_ENGINE_API WindowSystem();
		NX_ENGINE_API ~WindowSystem();

		NX_ENGINE_API void Close();
		NX_ENGINE_API void Minimize();
		NX_ENGINE_API void Maximize();
		NX_ENGINE_API void Restore();
		NX_ENGINE_API void Show();
		NX_ENGINE_API void Hide();
		NX_ENGINE_API void Focus();

		NX_ENGINE_API WindowSystem& SetWindowVSync(bool VSync);
		NX_ENGINE_API WindowSystem& SetWindowMode(Window::Mode Mode);
		NX_ENGINE_API WindowSystem& SetWindowMonitor(uint8 MonitorIndex);
		NX_ENGINE_API WindowSystem& SetWindowPosition(NxFr::Vector2i Position);
		NX_ENGINE_API WindowSystem& SetWindowResolution(NxFr::Vector2i Resolution);
		NX_ENGINE_API WindowSystem& SetWindowTitle(NxFr::StringView Title);
		NX_ENGINE_API WindowSystem& SetWindowIcon(Image* Icon);
		NX_ENGINE_API WindowSystem& SetCursorMode(Cursor::Mode Mode);
		NX_ENGINE_API WindowSystem& SetCursorIcon(Cursor::Icon Icon, void* IconCustom = nullptr);

		NX_ENGINE_API NxFr::Event<>& GetOnClose() { return OnClose; }
		NX_ENGINE_API NxFr::Event<bool>& GetOnFocus() { return OnFocus; }
		NX_ENGINE_API NxFr::Event<NxFr::Vector2i>& GetOnMove() { return OnMove; }
		NX_ENGINE_API NxFr::Event<NxFr::Vector2i>& GetOnResize() { return OnResize; }

		NX_ENGINE_API const Window& GetWindow() const { return Target; }
		NX_ENGINE_API const Monitor& GetMonitor(uint8 Index = 0) const { return Monitors[Index]; }
		NX_ENGINE_API bool IsFocused() const { return Focused; }

	protected:
		NX_ENGINE_API void OnInitialize() override;
		NX_ENGINE_API void OnShutdown() override;
		NX_ENGINE_API void OnTick(float TimeStep = 0.0f) override;
		NX_ENGINE_API void OnFocused(bool Focus);
		NX_ENGINE_API void OnMoved(NxFr::Vector2i Position);
		NX_ENGINE_API void OnResized(NxFr::Vector2i Size);

	private:
		NX_ENGINE_API void FetchMonitors();
		NX_ENGINE_API void CreateWindow();
		NX_ENGINE_API void DestroyWindow();
		NX_ENGINE_API void TickWindow();
		NX_ENGINE_API void UpdateCursor();
		NX_ENGINE_API void ApplySettings();

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
