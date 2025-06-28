#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/External/Glfw.h"

#include "glfw/include/GLFW/glfw3.h"

namespace NxEn
{
	namespace Glfw
	{
	#define NEXUS_WINDOW(Window) static_cast<GLFWwindow*>(Window)
	#define NEXUS_MONITOR(Monitor) static_cast<GLFWmonitor*>(Monitor)
	#define NEXUS_KEYCOUNT (GLFW_KEY_LAST + 1)

		static NxFr::Array<Input::KeyCode, NEXUS_KEYCOUNT>& KeyCodeGlfwToNexus()
		{
			static NxFr::Array<Input::KeyCode, NEXUS_KEYCOUNT> Conversion;
			Conversion[GLFW_KEY_SPACE]				= Input::KeyCode::Space;
			Conversion[GLFW_KEY_APOSTROPHE]			= Input::KeyCode::Apostrophe;
			Conversion[GLFW_KEY_COMMA]				= Input::KeyCode::Comma;
			Conversion[GLFW_KEY_MINUS]				= Input::KeyCode::Minus;
			Conversion[GLFW_KEY_PERIOD]				= Input::KeyCode::Period;
			Conversion[GLFW_KEY_SLASH]				= Input::KeyCode::Slash;
			Conversion[GLFW_KEY_0]					= Input::KeyCode::Alpha0;
			Conversion[GLFW_KEY_1]					= Input::KeyCode::Alpha1;
			Conversion[GLFW_KEY_2]					= Input::KeyCode::Alpha2;
			Conversion[GLFW_KEY_3]					= Input::KeyCode::Alpha3;
			Conversion[GLFW_KEY_4]					= Input::KeyCode::Alpha4;
			Conversion[GLFW_KEY_5]					= Input::KeyCode::Alpha5;
			Conversion[GLFW_KEY_6]					= Input::KeyCode::Alpha6;
			Conversion[GLFW_KEY_7]					= Input::KeyCode::Alpha7;
			Conversion[GLFW_KEY_8]					= Input::KeyCode::Alpha8;
			Conversion[GLFW_KEY_9]					= Input::KeyCode::Alpha9;
			Conversion[GLFW_KEY_SEMICOLON]			= Input::KeyCode::Semicolon;
			Conversion[GLFW_KEY_EQUAL]				= Input::KeyCode::Equal;
			Conversion[GLFW_KEY_A]					= Input::KeyCode::A;
			Conversion[GLFW_KEY_B]					= Input::KeyCode::B;
			Conversion[GLFW_KEY_C]					= Input::KeyCode::C;
			Conversion[GLFW_KEY_D]					= Input::KeyCode::D;
			Conversion[GLFW_KEY_E]					= Input::KeyCode::E;
			Conversion[GLFW_KEY_F]					= Input::KeyCode::F;
			Conversion[GLFW_KEY_G]					= Input::KeyCode::G;
			Conversion[GLFW_KEY_H]					= Input::KeyCode::H;
			Conversion[GLFW_KEY_I]					= Input::KeyCode::I;
			Conversion[GLFW_KEY_J]					= Input::KeyCode::J;
			Conversion[GLFW_KEY_K]					= Input::KeyCode::K;
			Conversion[GLFW_KEY_L]					= Input::KeyCode::L;
			Conversion[GLFW_KEY_M]					= Input::KeyCode::M;
			Conversion[GLFW_KEY_N]					= Input::KeyCode::N;
			Conversion[GLFW_KEY_O]					= Input::KeyCode::O;
			Conversion[GLFW_KEY_P]					= Input::KeyCode::P;
			Conversion[GLFW_KEY_Q]					= Input::KeyCode::Q;
			Conversion[GLFW_KEY_R]					= Input::KeyCode::R;
			Conversion[GLFW_KEY_S]					= Input::KeyCode::S;
			Conversion[GLFW_KEY_T]					= Input::KeyCode::T;
			Conversion[GLFW_KEY_U]					= Input::KeyCode::U;
			Conversion[GLFW_KEY_V]					= Input::KeyCode::V;
			Conversion[GLFW_KEY_W]					= Input::KeyCode::W;
			Conversion[GLFW_KEY_X]					= Input::KeyCode::X;
			Conversion[GLFW_KEY_Y]					= Input::KeyCode::Y;
			Conversion[GLFW_KEY_Z]					= Input::KeyCode::Z;
			Conversion[GLFW_KEY_LEFT_BRACKET]		= Input::KeyCode::LeftBracket;
			Conversion[GLFW_KEY_BACKSLASH]			= Input::KeyCode::Backslash;
			Conversion[GLFW_KEY_RIGHT_BRACKET]		= Input::KeyCode::RightBracket;
			Conversion[GLFW_KEY_GRAVE_ACCENT]		= Input::KeyCode::Invalid;
			Conversion[GLFW_KEY_WORLD_1]			= Input::KeyCode::Invalid;
			Conversion[GLFW_KEY_WORLD_2]			= Input::KeyCode::Invalid;
			Conversion[GLFW_KEY_ESCAPE]				= Input::KeyCode::Escape;
			Conversion[GLFW_KEY_ENTER]				= Input::KeyCode::Enter;
			Conversion[GLFW_KEY_TAB]				= Input::KeyCode::Tab;
			Conversion[GLFW_KEY_BACKSPACE]			= Input::KeyCode::Backslash;
			Conversion[GLFW_KEY_INSERT]				= Input::KeyCode::Insert;
			Conversion[GLFW_KEY_DELETE]				= Input::KeyCode::Delete;
			Conversion[GLFW_KEY_RIGHT]				= Input::KeyCode::Right;
			Conversion[GLFW_KEY_LEFT]				= Input::KeyCode::Left;
			Conversion[GLFW_KEY_DOWN]				= Input::KeyCode::Down;
			Conversion[GLFW_KEY_UP]					= Input::KeyCode::Up;
			Conversion[GLFW_KEY_PAGE_UP]			= Input::KeyCode::PageUp;
			Conversion[GLFW_KEY_PAGE_DOWN]			= Input::KeyCode::PageDown;
			Conversion[GLFW_KEY_HOME]				= Input::KeyCode::Home;
			Conversion[GLFW_KEY_END]				= Input::KeyCode::End;
			Conversion[GLFW_KEY_CAPS_LOCK]			= Input::KeyCode::CapsLock;
			Conversion[GLFW_KEY_SCROLL_LOCK]		= Input::KeyCode::ScrollLock;
			Conversion[GLFW_KEY_NUM_LOCK]			= Input::KeyCode::NumLock;
			Conversion[GLFW_KEY_PRINT_SCREEN]		= Input::KeyCode::PrintScreen;
			Conversion[GLFW_KEY_PAUSE]				= Input::KeyCode::Pause;
			Conversion[GLFW_KEY_F1]					= Input::KeyCode::F1;
			Conversion[GLFW_KEY_F2]					= Input::KeyCode::F2;
			Conversion[GLFW_KEY_F3]					= Input::KeyCode::F3;
			Conversion[GLFW_KEY_F4]					= Input::KeyCode::F4;
			Conversion[GLFW_KEY_F5]					= Input::KeyCode::F5;
			Conversion[GLFW_KEY_F6]					= Input::KeyCode::F6;
			Conversion[GLFW_KEY_F7]					= Input::KeyCode::F7;
			Conversion[GLFW_KEY_F8]					= Input::KeyCode::F8;
			Conversion[GLFW_KEY_F9]					= Input::KeyCode::F9;
			Conversion[GLFW_KEY_F10]				= Input::KeyCode::F10;
			Conversion[GLFW_KEY_F11]				= Input::KeyCode::F11;
			Conversion[GLFW_KEY_F12]				= Input::KeyCode::F12;
			Conversion[GLFW_KEY_F13]				= Input::KeyCode::Invalid;
			Conversion[GLFW_KEY_F14]				= Input::KeyCode::Invalid;
			Conversion[GLFW_KEY_F15]				= Input::KeyCode::Invalid;
			Conversion[GLFW_KEY_F16]				= Input::KeyCode::Invalid;
			Conversion[GLFW_KEY_F17]				= Input::KeyCode::Invalid;
			Conversion[GLFW_KEY_F18]				= Input::KeyCode::Invalid;
			Conversion[GLFW_KEY_F19]				= Input::KeyCode::Invalid;
			Conversion[GLFW_KEY_F20]				= Input::KeyCode::Invalid;
			Conversion[GLFW_KEY_F21]				= Input::KeyCode::Invalid;
			Conversion[GLFW_KEY_F22]				= Input::KeyCode::Invalid;
			Conversion[GLFW_KEY_F23]				= Input::KeyCode::Invalid;
			Conversion[GLFW_KEY_F24]				= Input::KeyCode::Invalid;
			Conversion[GLFW_KEY_F25]				= Input::KeyCode::Invalid;
			Conversion[GLFW_KEY_KP_0]				= Input::KeyCode::Num0;
			Conversion[GLFW_KEY_KP_1]				= Input::KeyCode::Num1;
			Conversion[GLFW_KEY_KP_2]				= Input::KeyCode::Num2;
			Conversion[GLFW_KEY_KP_3]				= Input::KeyCode::Num3;
			Conversion[GLFW_KEY_KP_4]				= Input::KeyCode::Num4;
			Conversion[GLFW_KEY_KP_5]				= Input::KeyCode::Num5;
			Conversion[GLFW_KEY_KP_6]				= Input::KeyCode::Num6;
			Conversion[GLFW_KEY_KP_7]				= Input::KeyCode::Num7;
			Conversion[GLFW_KEY_KP_8]				= Input::KeyCode::Num8;
			Conversion[GLFW_KEY_KP_9]				= Input::KeyCode::Num9;
			Conversion[GLFW_KEY_KP_DECIMAL]			= Input::KeyCode::NumDecimal;
			Conversion[GLFW_KEY_KP_DIVIDE]			= Input::KeyCode::NumDivide;
			Conversion[GLFW_KEY_KP_MULTIPLY]		= Input::KeyCode::NumMultiply;
			Conversion[GLFW_KEY_KP_SUBTRACT]		= Input::KeyCode::NumSubtract;
			Conversion[GLFW_KEY_KP_ADD]				= Input::KeyCode::NumAdd;
			Conversion[GLFW_KEY_KP_ENTER]			= Input::KeyCode::NumEnter;
			Conversion[GLFW_KEY_KP_EQUAL]			= Input::KeyCode::NumEqual;
			Conversion[GLFW_KEY_LEFT_SHIFT]			= Input::KeyCode::LeftShift;
			Conversion[GLFW_KEY_LEFT_CONTROL]		= Input::KeyCode::LeftControl;
			Conversion[GLFW_KEY_LEFT_ALT]			= Input::KeyCode::LeftAlt;
			Conversion[GLFW_KEY_LEFT_SUPER]			= Input::KeyCode::Invalid;
			Conversion[GLFW_KEY_RIGHT_SHIFT]		= Input::KeyCode::RightShift;
			Conversion[GLFW_KEY_RIGHT_CONTROL]		= Input::KeyCode::RightControl;
			Conversion[GLFW_KEY_RIGHT_ALT]			= Input::KeyCode::RightAlt;
			Conversion[GLFW_KEY_RIGHT_SUPER]		= Input::KeyCode::Invalid;
			Conversion[GLFW_KEY_MENU]				= Input::KeyCode::Menu;
			return Conversion;
		}

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

		static void KeyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
		{
			if (Action != GLFW_PRESS && Action != GLFW_RELEASE)
			{
				return;
			}

			Input::KeyCode Code = KeyCodeGlfwToNexus()[Key];
			Input::State State = Action == GLFW_PRESS ? Input::State::Pressed : Input::State::Released;
			Application::GetInstance()->GetSystems().GetSystem<InputSystem>()->OnKeyChange.Invoke(Code, State);
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

			auto& Conversion = KeyCodeGlfwToNexus();
		}

		void Shutdown()
		{
			glfwTerminate();
		}

		void PollInput()
		{
			glfwPollEvents();
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

			glfwSetKeyCallback(Instance, KeyCallback);

			return Instance;
		}

		void DestroyWindow(void* Window)
		{
			GLFWwindow* Instance = NEXUS_WINDOW(Window);

			glfwSetWindowCloseCallback(Instance, nullptr);
			glfwSetWindowFocusCallback(Instance, nullptr);
			glfwSetWindowPosCallback(Instance, nullptr);
			glfwSetWindowSizeCallback(Instance, nullptr);

			glfwSetKeyCallback(Instance, nullptr);

			glfwDestroyWindow(Instance);
		}

		void TickWindow(void* Window)
		{
			glfwSwapBuffers(NEXUS_WINDOW(Window));
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
