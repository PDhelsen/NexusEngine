#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace Glfw
	{
		void Initialize();
		void Shutdown();
		void PollInput();

		void* GetContext();
		void SetContext(void* Context);
		void SetSwapInterval(uint8 Interval);

		void* CreateWindow(uint8 Mode, void* Monitor, NxFr::Vector2i Position, NxFr::Vector2i Size, NxFr::StringView Title, NxFr::Vector2i IconResolution = NxFr::Vector2i::Zero, uint8* IconPixels = nullptr, uint8 Interval = 1);
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
		void SetWindowIcon(void* Window, NxFr::Vector2i Resolution, uint8* Pixels);

		NxFr::Array<void*> GetMonitors();
		void GetMonitorSettings(void* Monitor, int32& Width, int32& Height, int32& RefreshRate);

		void* UpdateCursorIcon(void* Window, void* Cursor, uint8 Icon, void* IconCustom);
		void SetCursorMode(void* Window, uint32 Mode);
	}
}
