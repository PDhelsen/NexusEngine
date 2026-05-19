#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class Image;

	class Window
	{
		friend class WindowSystem;

	public:
		enum class Mode : uint8
		{
			Windowed, Borderless, FullScreen
		};

		NX_ENGINE_API Window();
		NX_ENGINE_API Window(Mode WindowMode, int8 Monitor, NxFr::Vector2i Position, NxFr::Vector2i Resolution, const NxFr::String& Title, Image* Icon, bool VSync);
		NX_ENGINE_API ~Window();

		NX_ENGINE_API Window::Mode GetMode() const { return WindowMode; }
		NX_ENGINE_API int8 GetMonitor() const { return Monitor; }
		NX_ENGINE_API NxFr::Vector2i GetPosition() const { return Position; }
		NX_ENGINE_API NxFr::Vector2i GetResolution() const { return Resolution; }
		NX_ENGINE_API NxFr::StringView GetTitle() const { return Title; }
		NX_ENGINE_API const Image* GetIcon() const { return Icon; }
		NX_ENGINE_API bool GetVSync() const { return VSync; }

		NX_ENGINE_API bool IsValid() const { return Instance; }

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
		NX_ENGINE_API Monitor();
		NX_ENGINE_API Monitor(NxFr::Vector2i Resolution, int32 RefreshRate);
		NX_ENGINE_API ~Monitor();

		NX_ENGINE_API NxFr::Vector2i GetResolution() const { return Resolution; }
		NX_ENGINE_API int32 GetRefreshRate() const { return RefreshRate; }

		NX_ENGINE_API bool IsValid() const { return Instance; }

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

		NX_ENGINE_API Cursor();
		NX_ENGINE_API Cursor(Mode CursorMode, Icon CursorIcon, void* IconCustom = nullptr);
		NX_ENGINE_API ~Cursor();

		NX_ENGINE_API Mode GetMode() const { return CursorMode; }
		NX_ENGINE_API Icon GetIcon() const { return CursorIcon; }
		NX_ENGINE_API void* GetIconCustom() const { return IconCustom; }

		NX_ENGINE_API bool IsValid() const { return Instance; }

	private:
		Mode CursorMode;
		Icon CursorIcon;
		void* IconCustom;

		void* Instance;
	};
}
