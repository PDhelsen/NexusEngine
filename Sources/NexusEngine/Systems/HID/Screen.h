#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class Image;

	struct Monitor
	{
		friend class WindowSystem;

	public:
		Monitor();
		~Monitor();

		NxFr::Vector2i Resolution;
		int32 RefreshRate;

	private:
		void* Instance;
	};

	struct NX_ENGINE_API Window
	{
		friend class WindowSystem;

	public:
		enum class Mode : uint8
		{
			Windowed, Borderless, FullScreen
		};

		Window();
		~Window();

		Mode WindowMode;
		int8 Monitor;
		uint64 RefreshRate;
		bool VSync;
		NxFr::Vector2i Position;
		NxFr::Vector2i Resolution;
		NxFr::String Title;
		const Image* Icon;

	private:
		void* Instance;
	};

	struct Cursor
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
		~Cursor();

		Mode CursorMode;
		Icon CursorIcon;
		const Image* IconCustom;

	private:
		void* Instance;
	};
}
