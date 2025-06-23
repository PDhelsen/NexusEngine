#include "NexusEngine/Core/NexusEnginePch.h"
#include "glfw/include/GLFW/glfw3.h"
#include "Glfw.h"

namespace NxEn
{
	namespace Glfw
	{
		#define NEXUS_WINDOW(Window) static_cast<GLFWwindow*>(Window)

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

		void* CreateWindow(NxFr::Vector2i Position, NxFr::Vector2i Size, NxFr::StringView Title, uint8 Interval)
		{
			GLFWwindow* Window = glfwCreateWindow(Size.x, Size.y, Title.C(), NULL, NULL);
			if (!Window)
			{
				NEXUS_LOG(Error, Default, "Failed to create window");
				return nullptr;
			}

			SetWindowPosition(Window, Position);

			glfwMakeContextCurrent(Window);
			SetSwapInterval(Interval);

			glfwSetWindowCloseCallback(Window, CloseCallback);
			glfwSetWindowFocusCallback(Window, FocusCallback);
			glfwSetWindowPosCallback(Window, MoveCallback);
			glfwSetWindowSizeCallback(Window, ResizeCallback);
			return Window;
		}

		void DestroyWindow(void* Window)
		{
			glfwDestroyWindow(NEXUS_WINDOW(Window));
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
