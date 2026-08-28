#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Elements/Window.h"

namespace NxEn
{
	namespace GUI
	{
		Window::Window()
			: Menu(), Dock()
		{
		}

		Window::~Window()
		{
		}

		void Window::Draw()
		{
			if (!IsEnabled())
			{
				return;
			}

			bool IsOpen = true;
			const ImGuiViewport* Viewport = ImGui::GetMainViewport();

			ImGui::SetNextWindowPos(Viewport->WorkPos, ImGuiCond_Always);
			ImGui::SetNextWindowSize(Viewport->WorkSize, ImGuiCond_Always);

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin(GetNamedId().C(), &IsOpen, GetImGuiFlags());
			ImGui::PopStyleVar(3);

			OnDraw();

			ImGui::End();

			if (!IsOpen)
			{
				Close();
			}
		}

		void Window::OnInitialize()
		{
			Element::OnInitialize();
			Menu.Initialize();
			Dock.Initialize();

			Menu.SetNameId("Main Menu");
			Dock.SetNameId("Main Dock");

			SetGuiFlag(ElementFlags::Main, true);
			SetGuiFlag(ElementFlags::AutoDraw, false);
			SetImGuiFlags(
				ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoDocking |
				ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing
			);
			Menu.SetGuiFlag(ElementFlags::Main, true);
		}

		void Window::OnShutdown()
		{
			Dock.Shutdown();
			Menu.Shutdown();
			Element::OnShutdown();
		}

		void Window::OnEnable()
		{
			Element::OnEnable();
			Menu.Show();
			Dock.Show();
		}

		void Window::OnDisable()
		{
			Dock.Hide();
			Menu.Hide();
			Element::OnDisable();
		}

		void Window::OnDraw()
		{
			Menu.Draw();
			Dock.Draw();
		}
	}
}
