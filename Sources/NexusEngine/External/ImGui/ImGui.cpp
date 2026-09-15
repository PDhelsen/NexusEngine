#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/External/ImGui/ImGui.h"

#include "NexusEngine/External/Glfw/Glfw.h"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace NxEn
{
	namespace Imgui
	{
#define NX_WINDOW(Window) static_cast<GLFWwindow*>(Window)

		void Initialize()
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();

			ImGuiIO& IO = ImGui::GetIO();
			IO.IniFilename = nullptr;
			IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			// TODO: Fix Multiviewport and Glfw callbacks
			IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

			void* Context = Glfw::GetContext();
			ImGui_ImplGlfw_InitForOpenGL(NX_WINDOW(Context), true);
			ImGui_ImplOpenGL3_Init();
		}

		void Shutdown()
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		void Tick()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}

		void Render()
		{
			ImGui_ImplOpenGL3_Clear();

			ImGui::Render();
			ImDrawData* Data = ImGui::GetDrawData();
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
