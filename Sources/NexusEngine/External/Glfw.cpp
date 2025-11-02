#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/External/Glfw.h"

#include "glfw/include/GLFW/glfw3.h"

namespace NxEn
{
	namespace Glfw
	{
	#define NEXUS_WINDOW(Window) static_cast<GLFWwindow*>(Window)
	#define NEXUS_MONITOR(Monitor) static_cast<GLFWmonitor*>(Monitor)
	#define NEXUS_CURSOR(Cursor) static_cast<GLFWcursor*>(Cursor)
	#define NEXUS_KEYCOUNT (GLFW_KEY_LAST + 1)
	#define NEXUS_MOUSECOUNT (GLFW_MOUSE_BUTTON_LAST + 1)

		static NxFr::Array<Input::Button, NEXUS_KEYCOUNT>& GlfwKeyCodeToNexusButton()
		{
			static NxFr::Array<Input::Button, NEXUS_KEYCOUNT> Conversion;
			Conversion[GLFW_KEY_SPACE]				= Input::Button::Space;
			Conversion[GLFW_KEY_APOSTROPHE]			= Input::Button::Apostrophe;
			Conversion[GLFW_KEY_COMMA]				= Input::Button::Comma;
			Conversion[GLFW_KEY_MINUS]				= Input::Button::Minus;
			Conversion[GLFW_KEY_PERIOD]				= Input::Button::Period;
			Conversion[GLFW_KEY_SLASH]				= Input::Button::Slash;
			Conversion[GLFW_KEY_0]					= Input::Button::Alpha0;
			Conversion[GLFW_KEY_1]					= Input::Button::Alpha1;
			Conversion[GLFW_KEY_2]					= Input::Button::Alpha2;
			Conversion[GLFW_KEY_3]					= Input::Button::Alpha3;
			Conversion[GLFW_KEY_4]					= Input::Button::Alpha4;
			Conversion[GLFW_KEY_5]					= Input::Button::Alpha5;
			Conversion[GLFW_KEY_6]					= Input::Button::Alpha6;
			Conversion[GLFW_KEY_7]					= Input::Button::Alpha7;
			Conversion[GLFW_KEY_8]					= Input::Button::Alpha8;
			Conversion[GLFW_KEY_9]					= Input::Button::Alpha9;
			Conversion[GLFW_KEY_SEMICOLON]			= Input::Button::Semicolon;
			Conversion[GLFW_KEY_EQUAL]				= Input::Button::Equal;
			Conversion[GLFW_KEY_A]					= Input::Button::A;
			Conversion[GLFW_KEY_B]					= Input::Button::B;
			Conversion[GLFW_KEY_C]					= Input::Button::C;
			Conversion[GLFW_KEY_D]					= Input::Button::D;
			Conversion[GLFW_KEY_E]					= Input::Button::E;
			Conversion[GLFW_KEY_F]					= Input::Button::F;
			Conversion[GLFW_KEY_G]					= Input::Button::G;
			Conversion[GLFW_KEY_H]					= Input::Button::H;
			Conversion[GLFW_KEY_I]					= Input::Button::I;
			Conversion[GLFW_KEY_J]					= Input::Button::J;
			Conversion[GLFW_KEY_K]					= Input::Button::K;
			Conversion[GLFW_KEY_L]					= Input::Button::L;
			Conversion[GLFW_KEY_M]					= Input::Button::M;
			Conversion[GLFW_KEY_N]					= Input::Button::N;
			Conversion[GLFW_KEY_O]					= Input::Button::O;
			Conversion[GLFW_KEY_P]					= Input::Button::P;
			Conversion[GLFW_KEY_Q]					= Input::Button::Q;
			Conversion[GLFW_KEY_R]					= Input::Button::R;
			Conversion[GLFW_KEY_S]					= Input::Button::S;
			Conversion[GLFW_KEY_T]					= Input::Button::T;
			Conversion[GLFW_KEY_U]					= Input::Button::U;
			Conversion[GLFW_KEY_V]					= Input::Button::V;
			Conversion[GLFW_KEY_W]					= Input::Button::W;
			Conversion[GLFW_KEY_X]					= Input::Button::X;
			Conversion[GLFW_KEY_Y]					= Input::Button::Y;
			Conversion[GLFW_KEY_Z]					= Input::Button::Z;
			Conversion[GLFW_KEY_LEFT_BRACKET]		= Input::Button::LeftBracket;
			Conversion[GLFW_KEY_BACKSLASH]			= Input::Button::Backslash;
			Conversion[GLFW_KEY_RIGHT_BRACKET]		= Input::Button::RightBracket;
			Conversion[GLFW_KEY_GRAVE_ACCENT]		= Input::Button::Invalid;
			Conversion[GLFW_KEY_WORLD_1]			= Input::Button::Invalid;
			Conversion[GLFW_KEY_WORLD_2]			= Input::Button::Invalid;
			Conversion[GLFW_KEY_ESCAPE]				= Input::Button::Escape;
			Conversion[GLFW_KEY_ENTER]				= Input::Button::Enter;
			Conversion[GLFW_KEY_TAB]				= Input::Button::Tab;
			Conversion[GLFW_KEY_BACKSPACE]			= Input::Button::Backslash;
			Conversion[GLFW_KEY_INSERT]				= Input::Button::Insert;
			Conversion[GLFW_KEY_DELETE]				= Input::Button::Delete;
			Conversion[GLFW_KEY_RIGHT]				= Input::Button::Right;
			Conversion[GLFW_KEY_LEFT]				= Input::Button::Left;
			Conversion[GLFW_KEY_DOWN]				= Input::Button::Down;
			Conversion[GLFW_KEY_UP]					= Input::Button::Up;
			Conversion[GLFW_KEY_PAGE_UP]			= Input::Button::PageUp;
			Conversion[GLFW_KEY_PAGE_DOWN]			= Input::Button::PageDown;
			Conversion[GLFW_KEY_HOME]				= Input::Button::Home;
			Conversion[GLFW_KEY_END]				= Input::Button::End;
			Conversion[GLFW_KEY_CAPS_LOCK]			= Input::Button::CapsLock;
			Conversion[GLFW_KEY_SCROLL_LOCK]		= Input::Button::ScrollLock;
			Conversion[GLFW_KEY_NUM_LOCK]			= Input::Button::NumLock;
			Conversion[GLFW_KEY_PRINT_SCREEN]		= Input::Button::PrintScreen;
			Conversion[GLFW_KEY_PAUSE]				= Input::Button::Pause;
			Conversion[GLFW_KEY_F1]					= Input::Button::F1;
			Conversion[GLFW_KEY_F2]					= Input::Button::F2;
			Conversion[GLFW_KEY_F3]					= Input::Button::F3;
			Conversion[GLFW_KEY_F4]					= Input::Button::F4;
			Conversion[GLFW_KEY_F5]					= Input::Button::F5;
			Conversion[GLFW_KEY_F6]					= Input::Button::F6;
			Conversion[GLFW_KEY_F7]					= Input::Button::F7;
			Conversion[GLFW_KEY_F8]					= Input::Button::F8;
			Conversion[GLFW_KEY_F9]					= Input::Button::F9;
			Conversion[GLFW_KEY_F10]				= Input::Button::F10;
			Conversion[GLFW_KEY_F11]				= Input::Button::F11;
			Conversion[GLFW_KEY_F12]				= Input::Button::F12;
			Conversion[GLFW_KEY_F13]				= Input::Button::Invalid;
			Conversion[GLFW_KEY_F14]				= Input::Button::Invalid;
			Conversion[GLFW_KEY_F15]				= Input::Button::Invalid;
			Conversion[GLFW_KEY_F16]				= Input::Button::Invalid;
			Conversion[GLFW_KEY_F17]				= Input::Button::Invalid;
			Conversion[GLFW_KEY_F18]				= Input::Button::Invalid;
			Conversion[GLFW_KEY_F19]				= Input::Button::Invalid;
			Conversion[GLFW_KEY_F20]				= Input::Button::Invalid;
			Conversion[GLFW_KEY_F21]				= Input::Button::Invalid;
			Conversion[GLFW_KEY_F22]				= Input::Button::Invalid;
			Conversion[GLFW_KEY_F23]				= Input::Button::Invalid;
			Conversion[GLFW_KEY_F24]				= Input::Button::Invalid;
			Conversion[GLFW_KEY_F25]				= Input::Button::Invalid;
			Conversion[GLFW_KEY_KP_0]				= Input::Button::Num0;
			Conversion[GLFW_KEY_KP_1]				= Input::Button::Num1;
			Conversion[GLFW_KEY_KP_2]				= Input::Button::Num2;
			Conversion[GLFW_KEY_KP_3]				= Input::Button::Num3;
			Conversion[GLFW_KEY_KP_4]				= Input::Button::Num4;
			Conversion[GLFW_KEY_KP_5]				= Input::Button::Num5;
			Conversion[GLFW_KEY_KP_6]				= Input::Button::Num6;
			Conversion[GLFW_KEY_KP_7]				= Input::Button::Num7;
			Conversion[GLFW_KEY_KP_8]				= Input::Button::Num8;
			Conversion[GLFW_KEY_KP_9]				= Input::Button::Num9;
			Conversion[GLFW_KEY_KP_DECIMAL]			= Input::Button::NumDecimal;
			Conversion[GLFW_KEY_KP_DIVIDE]			= Input::Button::NumDivide;
			Conversion[GLFW_KEY_KP_MULTIPLY]		= Input::Button::NumMultiply;
			Conversion[GLFW_KEY_KP_SUBTRACT]		= Input::Button::NumSubtract;
			Conversion[GLFW_KEY_KP_ADD]				= Input::Button::NumAdd;
			Conversion[GLFW_KEY_KP_ENTER]			= Input::Button::NumEnter;
			Conversion[GLFW_KEY_KP_EQUAL]			= Input::Button::NumEqual;
			Conversion[GLFW_KEY_LEFT_SHIFT]			= Input::Button::LeftShift;
			Conversion[GLFW_KEY_LEFT_CONTROL]		= Input::Button::LeftControl;
			Conversion[GLFW_KEY_LEFT_ALT]			= Input::Button::LeftAlt;
			Conversion[GLFW_KEY_LEFT_SUPER]			= Input::Button::Invalid;
			Conversion[GLFW_KEY_RIGHT_SHIFT]		= Input::Button::RightShift;
			Conversion[GLFW_KEY_RIGHT_CONTROL]		= Input::Button::RightControl;
			Conversion[GLFW_KEY_RIGHT_ALT]			= Input::Button::RightAlt;
			Conversion[GLFW_KEY_RIGHT_SUPER]		= Input::Button::Invalid;
			Conversion[GLFW_KEY_MENU]				= Input::Button::Menu;
			return Conversion;
		}
		static NxFr::Array<Input::Button, NEXUS_KEYCOUNT>& GlfwMouseCodeToNexusButton()
		{
			static NxFr::Array<Input::Button, NEXUS_KEYCOUNT> Conversion;
			Conversion[GLFW_MOUSE_BUTTON_1] = Input::Button::MouseLeft;
			Conversion[GLFW_MOUSE_BUTTON_2] = Input::Button::MouseRight;
			Conversion[GLFW_MOUSE_BUTTON_3] = Input::Button::MouseMiddle;
			Conversion[GLFW_MOUSE_BUTTON_4] = Input::Button::Mouse1;
			Conversion[GLFW_MOUSE_BUTTON_5] = Input::Button::Mouse2;
			Conversion[GLFW_MOUSE_BUTTON_6] = Input::Button::Mouse3;
			Conversion[GLFW_MOUSE_BUTTON_7] = Input::Button::Mouse4;
			Conversion[GLFW_MOUSE_BUTTON_8] = Input::Button::Mouse5;
			return Conversion;
		}
		static bool MouseFocus = true;

#pragma region Callback

		static void ErrorCallback(int ErrorCode, const char* Description)
		{
			NEXUS_LOG(Error, Default, "Error (%d): %s", ErrorCode, Description);
		}

		static void CloseCallback(GLFWwindow* Window)
		{
			Application::GetSystem<WindowSystem>()->GetOnClose().Invoke();
		}

		static void MoveCallback(GLFWwindow* Window, int X, int Y)
		{
			Application::GetSystem<WindowSystem>()->GetOnMove().Invoke(NxFr::Vector2i(X, Y));
		}

		static void ResizeCallback(GLFWwindow* Window, int Width, int Height)
		{
			Application::GetSystem<WindowSystem>()->GetOnResize().Invoke(NxFr::Vector2i(Width, Height));
		}

		static void KeyButtonCallback(GLFWwindow* Window, int KeyCode, int ScanCode, int Action, int Mods)
		{
			if (Action != GLFW_PRESS && Action != GLFW_RELEASE)
			{
				return;
			}

			Input::Button Button = GlfwKeyCodeToNexusButton()[KeyCode];
			Input::State State = Action == GLFW_PRESS ? Input::State::Pressed : Input::State::Released;
			Application::GetSystem<InputSystem>()->GetOnButtonChange().Invoke(Button, State);
		}

		static void MouseButtonCallback(GLFWwindow* Window, int Mouse, int Action, int Mods)
		{
			if (Action != GLFW_PRESS && Action != GLFW_RELEASE)
			{
				return;
			}

			Input::Button Button = GlfwMouseCodeToNexusButton()[Mouse];
			Input::State State = Action == GLFW_PRESS ? Input::State::Pressed : Input::State::Released;
			Application::GetSystem<InputSystem>()->GetOnButtonChange().Invoke(Button, State);
		}

		static void MouseCallback(GLFWwindow* Window, double X, double Y)
		{
			if (!MouseFocus)
			{
				return;
			}

			Application::GetSystem<InputSystem>()->GetOnMouseChange().Invoke(NxFr::Vector2f(X, Y));
		}

		static void ScrollCallback(GLFWwindow* Window, double X, double Y)
		{
			Application::GetSystem<InputSystem>()->GetOnAxisChange().Invoke(Input::Axis::ScrollX, (float)X);
			Application::GetSystem<InputSystem>()->GetOnAxisChange().Invoke(Input::Axis::ScrollY, (float)Y);
		}

		static void WindowFocusCallback(GLFWwindow* Window, int Focused)
		{
			Application::GetSystem<WindowSystem>()->GetOnFocus().Invoke(Focused > 0);
		}

		static void MouseFocusCallback(GLFWwindow* Window, int Entered)
		{
			MouseFocus = Entered;
			if (!MouseFocus)
			{
				Application::GetSystem<InputSystem>()->GetOnMouseChange().Invoke(-NxFr::Vector2f::One);
			}
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

			auto& KeyConversion = GlfwKeyCodeToNexusButton();
			auto& MouseConversion = GlfwMouseCodeToNexusButton();
			MouseFocus = false;
		}

		void Shutdown()
		{
			glfwTerminate();
		}

		void PollInput()
		{
			glfwPollEvents();
		}

		void SetSwapInterval(uint8 Interval)
		{
			glfwSwapInterval(Interval);
		}

		void* GetContext()
		{
			return glfwGetCurrentContext();
		}

		void SetContext(void* Context)
		{
			glfwMakeContextCurrent(NEXUS_WINDOW(Context));
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

		void* CreateWindow(uint8 Mode, void* Monitor, NxFr::Vector2i Position, NxFr::Vector2i Size, NxFr::StringView Title, NxFr::Vector2i IconResolution, uint8* IconPixels, uint8 Interval)
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
			if (IconPixels)
			{
				SetWindowIcon(Instance, IconResolution, IconPixels);
			}

			glfwSetWindowCloseCallback(Instance, CloseCallback);
			glfwSetWindowPosCallback(Instance, MoveCallback);
			glfwSetWindowSizeCallback(Instance, ResizeCallback);

			glfwSetKeyCallback(Instance, KeyButtonCallback);
			glfwSetMouseButtonCallback(Instance, MouseButtonCallback);
			glfwSetCursorPosCallback(Instance, MouseCallback);
			glfwSetScrollCallback(Instance, ScrollCallback);

			glfwSetWindowFocusCallback(Instance, WindowFocusCallback);
			glfwSetCursorEnterCallback(Instance, MouseFocusCallback);

			MouseFocus = true;
			return Instance;
		}

		void DestroyWindow(void* Window)
		{
			GLFWwindow* Instance = NEXUS_WINDOW(Window);

			glfwSetWindowCloseCallback(Instance, nullptr);
			glfwSetWindowPosCallback(Instance, nullptr);
			glfwSetWindowSizeCallback(Instance, nullptr);

			glfwSetKeyCallback(Instance, nullptr);
			glfwSetMouseButtonCallback(Instance, nullptr);
			glfwSetCursorPosCallback(Instance, nullptr);
			glfwSetScrollCallback(Instance, nullptr);

			glfwSetWindowFocusCallback(Instance, nullptr);
			glfwSetCursorEnterCallback(Instance, nullptr);

			glfwDestroyWindow(Instance);
			MouseFocus = true;
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

		void SetWindowIcon(void* Window, NxFr::Vector2i Resolution, uint8* Pixels)
		{
			GLFWimage Image;
			Image.width = Resolution.x;
			Image.height = Resolution.y;
			Image.pixels = Pixels;
			glfwSetWindowIcon(NEXUS_WINDOW(Window), Pixels ? 1 : 0, Pixels ? &Image : nullptr);
		}

		void* UpdateCursorIcon(void* Window, void* Cursor, uint8 Icon, void* IconCustom)
		{
			if (Cursor != nullptr)
			{
				glfwDestroyCursor(NEXUS_CURSOR(Cursor));
			}

			switch (Icon)
			{
			case 0: Cursor = nullptr; break;
			case 1: Cursor = glfwCreateCursor(nullptr, 0, 0); break;
			case 2: Cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR); break;
			case 3: Cursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR); break;
			case 4: Cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR); break;
			case 5: Cursor = glfwCreateStandardCursor(GLFW_POINTING_HAND_CURSOR); break;
			case 6: Cursor = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR); break;
			case 7: Cursor = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR); break;
			}

			glfwSetCursor(NEXUS_WINDOW(Window), NEXUS_CURSOR(Cursor));
			return Cursor;
		}

		void SetCursorMode(void* Window, uint32 Mode)
		{
			switch (Mode)
			{
			case 0: Mode = (uint32)GLFW_CURSOR_NORMAL; break;
			case 1: Mode = (uint32)GLFW_CURSOR_CAPTURED; break;
			case 2: Mode = (uint32)GLFW_CURSOR_HIDDEN; break;
			case 3: Mode = (uint32)GLFW_CURSOR_DISABLED; break;
			}

			glfwSetInputMode(NEXUS_WINDOW(Window), GLFW_CURSOR, Mode);
		}

#pragma endregion
	}
}
