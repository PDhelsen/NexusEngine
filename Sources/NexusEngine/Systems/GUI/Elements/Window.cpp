#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Elements/Window.h"

namespace NxEn
{
	namespace GUI
	{
		Window::Window()
			: Menu(), DockId(0)
		{
		}

		Window::~Window()
		{
		}

		void Window::Draw()
		{
			Menu.Draw();

			DockId = ImGui::GetID(GetNamedId().C());
			ImGui::DockSpaceOverViewport(DockId);

			OnDraw();
		}

		void Window::OnInitialize()
		{
			Element::OnInitialize();
			Menu.Initialize();

			Menu.SetGuiFlag(ElementFlags::MainMenuBar, true);
			SetGuiFlag(ElementFlags::AutoDraw, false);
			SetImGuiFlags(
				ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse |
				ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing |
				ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings
			);
		}

		void Window::OnShutdown()
		{
			Menu.Shutdown();
			Element::OnShutdown();
		}
	}
}
