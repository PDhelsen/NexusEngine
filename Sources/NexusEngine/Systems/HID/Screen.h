#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class Window
	{
		friend class WindowSystem;

	public:
		enum class Mode : uint8
		{
			Windowed, Borderless, FullScreen
		};

		NEXUS_ENGINE_API Window();
		NEXUS_ENGINE_API Window(Mode WindowMode, int8 Monitor, NxFr::Vector2i Position, NxFr::Vector2i Resolution, const NxFr::String& Title, void* Icon, bool VSync);
		NEXUS_ENGINE_API ~Window();

		NEXUS_ENGINE_API Window::Mode GetMode() const { return WindowMode; }
		NEXUS_ENGINE_API int8 GetMonitor() const { return Monitor; }
		NEXUS_ENGINE_API NxFr::Vector2i GetPosition() const { return Position; }
		NEXUS_ENGINE_API NxFr::Vector2i GetResolution() const { return Resolution; }
		NEXUS_ENGINE_API NxFr::StringView GetTitle() const { return Title; }
		NEXUS_ENGINE_API const void* GetIcon() const { return Icon; }
		NEXUS_ENGINE_API bool GetVSync() const { return VSync; }

		NEXUS_ENGINE_API bool IsValid() const { return Instance; }

	private:
		Mode WindowMode;
		int8 Monitor;
		NxFr::Vector2i Position;
		NxFr::Vector2i Resolution;
		NxFr::String Title;
		void* Icon;
		bool VSync;

		void* Instance;
	};

	class Monitor
	{
		friend class WindowSystem;

	public:
		NEXUS_ENGINE_API Monitor();
		NEXUS_ENGINE_API Monitor(NxFr::Vector2i Resolution, int32 RefreshRate);
		NEXUS_ENGINE_API ~Monitor();

		NEXUS_ENGINE_API NxFr::Vector2i GetResolution() const { return Resolution; }
		NEXUS_ENGINE_API int32 GetRefreshRate() const { return RefreshRate; }

		NEXUS_ENGINE_API bool IsValid() const { return Instance; }

	private:
		NxFr::Vector2i Resolution;
		int32 RefreshRate;

		void* Instance;
	};
}
