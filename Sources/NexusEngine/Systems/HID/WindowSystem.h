#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/HID/Screen.h"

namespace NxEn
{
	class Image;

	class NX_ENGINE_API WindowSystem : public System
	{
		friend class GUISystem;

	public:
		NX_OBJECT(WindowSystem)

		WindowSystem();
		~WindowSystem();

		void Close();
		void Minimize();
		void Maximize();
		void Restore();
		void Show();
		void Hide();
		void Focus();

		WindowSystem& SetWindowMode(Window::Mode Mode);
		WindowSystem& SetWindowMonitor(int8 Index);
		WindowSystem& SetWindowVSync(bool VSync);
		WindowSystem& SetWindowRefreshRate(uint64 RefreshRate);
		WindowSystem& SetWindowPosition(NxFr::Vector2i Position);
		WindowSystem& SetWindowResolution(NxFr::Vector2i Resolution);
		WindowSystem& SetWindowTitle(NxFr::StringView Title);
		WindowSystem& SetWindowIcon(const Image* Icon);
		WindowSystem& SetCursorMode(Cursor::Mode Mode);
		WindowSystem& SetCursorIcon(Cursor::Icon Icon, const Image* IconCustom = nullptr);

		NxFr::Event<>& GetOnClose() { return OnClose; }
		NxFr::Event<bool>& GetOnFocus() { return OnFocus; }
		NxFr::Event<NxFr::Vector2i>& GetOnMove() { return OnMove; }
		NxFr::Event<NxFr::Vector2i>& GetOnResize() { return OnResize; }

		const NxFr::Array<Monitor>& GetMonitors() const { return Monitors; }
		const Window& GetWindow() const { return Target; }
		const Cursor& GetCursor() const { return Pointer; }
		bool IsFocused() const { return Focused; }

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnTick(float TimeStep = 0.0f) override;

		void OnFocused(bool Focus);
		void OnMoved(NxFr::Vector2i Position);
		void OnResized(NxFr::Vector2i Size);

	private:
		void FetchMonitors();
		void CreateWindow();
		void DestroyWindow();
		void TickWindow();
		void CreateCursor();
		void DestroyCursor();

		void ApplySettings();

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
