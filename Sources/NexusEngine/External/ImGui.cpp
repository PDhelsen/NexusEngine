#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/External/ImGui.h"

#include "NexusEngine/External/Glfw.h"
#include "NexusEngine/External/imgui/imgui_impl_glfw.h"
#include "NexusEngine/External/imgui/imgui_impl_opengl3.h"
#include "NexusEngine/External/imgui/imgui_impl_opengl3_loader.h"

namespace NxEn
{
	namespace Imgui
	{
#define NEXUS_WINDOW(Window) static_cast<GLFWwindow*>(Window)

		void Initialize()
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();

			ImGuiIO& IO = ImGui::GetIO();
			IO.IniFilename = nullptr;
			IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

			void* Context = Glfw::GetContext();
			ImGui_ImplGlfw_InitForOpenGL(NEXUS_WINDOW(Context), true);
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
		}

		void Render()
		{
			//TODO: Move to RenderingSystem
			glClear(GL_COLOR_BUFFER_BIT);

			ImGui::Render();
			auto Data = ImGui::GetDrawData();
			ImGui_ImplOpenGL3_RenderDrawData(Data);

			ImGuiIO& IO = ImGui::GetIO();
			if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				void* Context = Glfw::GetContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				Glfw::SetContext(Context);
			}
		}
	}
}
