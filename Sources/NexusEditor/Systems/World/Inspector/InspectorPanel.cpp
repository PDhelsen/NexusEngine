#include "NexusEditor/Systems/World/Inspector/InspectorPanel.h"

namespace NxEd
{
	static InspectorPanel* Panel = NxEn::GUI::Panel::Create<InspectorPanel>();

	const static NxEn::GUI::Menu::Item MenuItemInspector = NxEn::GUI::Menu::Item::Create("Object/World/Inspector", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel InspectorPanel");
	}));

	NEXUS_OBJECT_IMPLEMENTATION(InspectorPanel)

	void InspectorPanel::Show(NxFr::Handle<NxEn::GameObject> Instance)
	{
		Target = Instance;
		Panel::Show();
	}

	void InspectorPanel::OnGui(float TimeStep)
	{
		if (!Target)
		{
			return;
		}

		Target->DrawGui(TimeStep);
	}
}
