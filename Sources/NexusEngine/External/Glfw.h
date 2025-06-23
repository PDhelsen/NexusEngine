#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace Glfw
	{
		void Initialize();
		void Shutdown();

		void* CreateWindow(NxFr::Vector2i Position, NxFr::Vector2i Size, NxFr::StringView Title, uint8 Interval = 1);
		void DestroyWindow(void* Window);
		void TickWindow(void* Window);

		void CloseWindow(void* Window);
		void MinimizeWindow(void* Window);
		void MaximizeWindow(void* Window);
		void RestoreWindow(void* Window);
		void ShowWindow(void* Window);
		void HideWindow(void* Window);
		void FocusWindow(void* Window);

		void SetWindowPosition(void* Window, NxFr::Vector2i Position);
		void SetWindowSize(void* Window, NxFr::Vector2i Size);
		void SetWindowTitle(void* Window, NxFr::StringView Title);
		void SetWindowIcon(void* Window, void* Icon);

		void SetSwapInterval(uint8 Interval);
	}
}
