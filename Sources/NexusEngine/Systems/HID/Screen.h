#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class Image;

	class NX_ENGINE_API Window
	{
		friend class WindowSystem;

	public:
		enum class Mode : uint8
		{
			Windowed, Borderless, FullScreen
		};

		Window();
		Window(Mode WindowMode, int8 Monitor, NxFr::Vector2i Position, NxFr::Vector2i Resolution, const NxFr::String& Title, Image* Icon, bool VSync);
		~Window();

		Window::Mode GetMode() const { return WindowMode; }
		int8 GetMonitor() const { return Monitor; }
		NxFr::Vector2i GetPosition() const { return Position; }
		NxFr::Vector2i GetResolution() const { return Resolution; }
		NxFr::StringView GetTitle() const { return Title; }
		const Image* GetIcon() const { return Icon; }
		bool GetVSync() const { return VSync; }

		bool IsValid() const { return Instance; }

	private:
		Mode WindowMode;
		int8 Monitor;
		NxFr::Vector2i Position;
		NxFr::Vector2i Resolution;
		NxFr::String Title;
		Image* Icon;
		bool VSync;

		void* Instance;
	};

	class Monitor
	{
		friend class WindowSystem;

	public:
		Monitor();
		Monitor(NxFr::Vector2i Resolution, int32 RefreshRate);
		~Monitor();

		NxFr::Vector2i GetResolution() const { return Resolution; }
		int32 GetRefreshRate() const { return RefreshRate; }

		bool IsValid() const { return Instance; }

	private:
		NxFr::Vector2i Resolution;
		int32 RefreshRate;

		void* Instance;
	};

	class Cursor
	{
		friend class WindowSystem;

	public:
		enum class Mode : uint8
		{
			Default, Captured, Hidden, Disabled
		};

		enum class Icon : uint8
		{
			Default, Custom, Arrow, IBeam, CrossHair, Hand, Resize, NotAllowed
		};

		Cursor();
		Cursor(Mode CursorMode, Icon CursorIcon, void* IconCustom = nullptr);
		~Cursor();

		Mode GetMode() const { return CursorMode; }
		Icon GetIcon() const { return CursorIcon; }
		void* GetIconCustom() const { return IconCustom; }

		bool IsValid() const { return Instance; }

	private:
		Mode CursorMode;
		Icon CursorIcon;
		void* IconCustom;

		void* Instance;
	};
}
