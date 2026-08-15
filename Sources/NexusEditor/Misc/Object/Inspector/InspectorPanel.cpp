#include "NexusEditor/Misc/Object/Inspector/InspectorPanel.h"

namespace NxEd
{
	static InspectorPanel* Panel = NxEn::GUI::Panel::Create<InspectorPanel>();

	static const NxEn::GUI::Menu::Item* MenuItemBrowser = NxEn::GUI::Menu::Create("Object/Objects/Inspector", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel InspectorPanel");
	}));

	InspectorPanel::InspectorPanel()
		: Instance(nullptr), Lock(false)
	{
	}

	InspectorPanel::~InspectorPanel()
	{
	}

	void InspectorPanel::Show(NxEn::ObjectInstance<NxEn::Object> Target, bool Force)
	{
		Panel::Show();

		if (Lock && !Force)
		{
			return;
		}

		Instance = Target;
	}

	void InspectorPanel::OnInitialize()
	{
		Panel::OnInitialize();
		Menu.Initialize();

		SetNameId("Inspector");
		SetImGuiFlag(ImGuiWindowFlags_MenuBar, true);

		Menu.AddMenuToggle("Lock", &Lock);
	}

	void InspectorPanel::OnShutdown()
	{
		Menu.Shutdown();
		Panel::OnShutdown();
	}

	void InspectorPanel::OnEnable()
	{
		Panel::OnEnable();
		Menu.Show();
	}

	void InspectorPanel::OnDisable()
	{
		Instance = nullptr;

		Menu.Hide();
		Panel::OnDisable();
	}

	void InspectorPanel::OnDraw()
	{
		Menu.Draw();

		if (!Instance)
		{
			return;
		}

		Instance->Draw();
	}
}
