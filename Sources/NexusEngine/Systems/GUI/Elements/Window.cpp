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
			if (!IsEnabled())
			{
				return;
			}

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
		}

		void Window::OnShutdown()
		{
			Menu.Shutdown();
			Element::OnShutdown();
		}

		void Window::OnEnable()
		{
			Element::OnEnable();
			Menu.Show();
		}

		void Window::OnDisable()
		{
			Menu.Hide();
			Element::OnDisable();
		}
	}
}
