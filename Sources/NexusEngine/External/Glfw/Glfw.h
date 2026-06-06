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

		NxFr::Array<void*> GetMonitors();
		void GetMonitorSettings(void* Monitor, int32& Width, int32& Height, int32& RefreshRate);

		void* CreateWindow();
		void DestroyWindow(void* Window);
		void TickWindow(void* Window);
		void CloseWindow(void* Window);
		void MinimizeWindow(void* Window);
		void MaximizeWindow(void* Window);
		void RestoreWindow(void* Window);
		void ShowWindow(void* Window);
		void HideWindow(void* Window);
		void FocusWindow(void* Window);
		void SetWindowMode(void* Window, uint8 Mode, void* Monitor, NxFr::Vector2i Position, NxFr::Vector2i Resolution, uint64 RefreshRate);
		void SetWindowPosition(void* Window, NxFr::Vector2i Position);
		void SetWindowSize(void* Window, NxFr::Vector2i Size);
		void SetWindowTitle(void* Window, NxFr::StringView Title);
		void SetWindowIcon(void* Window, NxFr::Vector2i Resolution, uint8* Pixels);

		void* CreateCursor(void* Window, uint8 Icon, NxFr::Vector2i IconResolution = NxFr::Vector2i::Zero, uint8* IconPixels = nullptr);
		void DestroyCursor(void* Cursor);
		void SetCursorMode(void* Window, uint32 Mode);
	}
}
