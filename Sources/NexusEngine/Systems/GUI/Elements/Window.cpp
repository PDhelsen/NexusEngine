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

			Menu.Draw();
			Dock.Draw();

			OnDraw();
		}

		void Window::OnInitialize()
		{
			Element::OnInitialize();
			Menu.Initialize();
			Dock.Initialize();

			Menu.SetNameId("Main Menu");
			Dock.SetNameId("Main Dock");

			SetGuiFlag(ElementFlags::AutoDraw, false);
			Menu.SetGuiFlag(ElementFlags::Main, true);
			Dock.SetGuiFlag(ElementFlags::Main, true);
			Dock.SetImGuiFlags(
				ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoDocking |
				ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing
			);
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
	}
}
