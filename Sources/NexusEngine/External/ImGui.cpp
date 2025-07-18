#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/External/ImGui.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_opengl3_loader.h"

namespace NxEn
{
	namespace Imgui
	{
#define NEXUS_WINDOW(Window) static_cast<GLFWwindow*>(Window)

		void Initialize(void* Window)
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ::ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

			ImGui_ImplGlfw_InitForOpenGL(NEXUS_WINDOW(Window), true);
			ImGui_ImplOpenGL3_Init();
		}

		void Shutdown()
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		void Frame()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::ShowDemoWindow();
		}

		void Render()
		{
			ImGui::Render();
			glClear(GL_COLOR_BUFFER_BIT);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}
}
