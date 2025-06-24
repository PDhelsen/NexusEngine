#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/External/Glfw.h"

#include "glfw/include/GLFW/glfw3.h"

namespace NxEn
{
	namespace Glfw
	{
	#define NEXUS_WINDOW(Window) static_cast<GLFWwindow*>(Window)
	#define NEXUS_MONITOR(Monitor) static_cast<GLFWmonitor*>(Monitor)

#pragma region Callback

		static void ErrorCallback(int ErrorCode, const char* Description)
		{
			NEXUS_LOG(Error, Default, "Error (%d): %s", ErrorCode, Description);
		}

		static void CloseCallback(GLFWwindow* Window)
		{
			Application::GetInstance()->GetSystems().GetSystem<WindowSystem>()->OnClose.Invoke();
		}

		static void FocusCallback(GLFWwindow* Window, int Focused)
		{
			Application::GetInstance()->GetSystems().GetSystem<WindowSystem>()->OnFocus.Invoke(Focused > 0);
		}

		static void MoveCallback(GLFWwindow* Window, int X, int Y)
		{
			Application::GetInstance()->GetSystems().GetSystem<WindowSystem>()->OnMove.Invoke(NxFr::Vector2i(X, Y));
		}

		static void ResizeCallback(GLFWwindow* Window, int Width, int Height)
		{
			Application::GetInstance()->GetSystems().GetSystem<WindowSystem>()->OnResize.Invoke(NxFr::Vector2i(Width, Height));
		}

#pragma endregion

#pragma region LifeCycle

		void Initialize()
		{
			glfwSetErrorCallback(ErrorCallback);
			if (!glfwInit())
			{
				NEXUS_LOG(Error, Default, "Failed to init glfw");
				return;
			}
		}

		void Shutdown()
		{
			glfwTerminate();
		}

#pragma endregion

#pragma region Window

		NxFr::Array<void*> GetMonitors()
		{
			int Count;
			GLFWmonitor** Instances = glfwGetMonitors(&Count);

			NxFr::Array<void*> Monitors(Count);
			for (uint8 Index = 0; Index < Count; ++Index)
			{
				Monitors[Index] = Instances[Index];
			}
			return Monitors;
		}

		void GetMonitorSettings(void* Monitor, int32& Width, int32& Height, int32& RefreshRate)
		{
			const GLFWvidmode* Mode = glfwGetVideoMode(NEXUS_MONITOR(Monitor));
			Width = Mode->width;
			Height = Mode->height;
			RefreshRate = Mode->refreshRate;
		}

		void* CreateWindow(uint8 Mode, void* Monitor, NxFr::Vector2i Position, NxFr::Vector2i Size, NxFr::StringView Title, uint8 Interval)
		{
			GLFWwindow* Instance = nullptr;
			switch (Mode)
			{
				// Windowed
				case 0:
				{
					Instance = glfwCreateWindow(Size.x, Size.y, Title.C(), nullptr, nullptr);
					SetWindowPosition(Instance, Position);
				}
				break;
				// Borderless
				case 1:
				{
					GLFWmonitor* Target = NEXUS_MONITOR(Monitor);
					const GLFWvidmode* VideoMode = glfwGetVideoMode(Target);
					glfwWindowHint(GLFW_REFRESH_RATE, VideoMode->refreshRate);
					Instance = glfwCreateWindow(VideoMode->width, VideoMode->height, Title.C(), nullptr, nullptr);
					glfwSetWindowMonitor(Instance, Target, 0, 0, VideoMode->width, VideoMode->height, VideoMode->refreshRate);
				}
				break;
				// FullScreen
				case 2:
				{
					GLFWmonitor* Target = NEXUS_MONITOR(Monitor);
					const GLFWvidmode* VideoMode = glfwGetVideoMode(Target);
					glfwWindowHint(GLFW_REFRESH_RATE, VideoMode->refreshRate);
					Instance = glfwCreateWindow(VideoMode->width, VideoMode->height, Title.C(), Target, nullptr);
				}
				break;
			}

			if (!Instance)
			{
				NEXUS_LOG(Error, Default, "Failed to create window");
				return nullptr;
			}

			glfwMakeContextCurrent(Instance);
			SetSwapInterval(Interval);

			glfwSetWindowCloseCallback(Instance, CloseCallback);
			glfwSetWindowFocusCallback(Instance, FocusCallback);
			glfwSetWindowPosCallback(Instance, MoveCallback);
			glfwSetWindowSizeCallback(Instance, ResizeCallback);

			return Instance;
		}

		void DestroyWindow(void* Window)
		{
			GLFWwindow* Instance = NEXUS_WINDOW(Window);

			glfwSetWindowCloseCallback(Instance, nullptr);
			glfwSetWindowFocusCallback(Instance, nullptr);
			glfwSetWindowPosCallback(Instance, nullptr);
			glfwSetWindowSizeCallback(Instance, nullptr);

			glfwDestroyWindow(Instance);
		}

		void TickWindow(void* Window)
		{
			glfwSwapBuffers(NEXUS_WINDOW(Window));
			glfwPollEvents();
		}

		void CloseWindow(void* Window)
		{
			glfwSetWindowShouldClose(NEXUS_WINDOW(Window), 1);
		}

		void MinimizeWindow(void* Window)
		{
			glfwIconifyWindow(NEXUS_WINDOW(Window));
		}

		void MaximizeWindow(void* Window)
		{
			glfwMaximizeWindow(NEXUS_WINDOW(Window));
		}

		void RestoreWindow(void* Window)
		{
			glfwRestoreWindow(NEXUS_WINDOW(Window));
		}

		void ShowWindow(void* Window)
		{
			glfwShowWindow(NEXUS_WINDOW(Window));
		}

		void HideWindow(void* Window)
		{
			glfwHideWindow(NEXUS_WINDOW(Window));
		}

		void FocusWindow(void* Window)
		{
			glfwFocusWindow(NEXUS_WINDOW(Window));
		}

		void SetWindowPosition(void* Window, NxFr::Vector2i Position)
		{
			glfwSetWindowPos(NEXUS_WINDOW(Window), Position.x, Position.y);
		}

		void SetWindowSize(void* Window, NxFr::Vector2i Size)
		{
			glfwSetWindowSize(NEXUS_WINDOW(Window), Size.x, Size.y);
		}

		void SetWindowTitle(void* Window, NxFr::StringView Title)
		{
			glfwSetWindowTitle(NEXUS_WINDOW(Window), Title.C());
		}

		void SetWindowIcon(void* Window, void* Icon)
		{
			glfwSetWindowIcon(NEXUS_WINDOW(Window), 1, nullptr);
		}

		void SetSwapInterval(uint8 Interval)
		{
			glfwSwapInterval(Interval);
		}

#pragma endregion
	}
}
