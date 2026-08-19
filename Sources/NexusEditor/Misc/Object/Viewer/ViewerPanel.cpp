#include "NexusEditor/Misc/Object/Viewer/ViewerPanel.h"

namespace NxEd
{
	static ViewerPanel* Panel = NxEn::GUI::Panel::Create<ViewerPanel>();

	static const NxEn::GUI::Menu::Item* MenuItemBrowser = NxEn::GUI::Menu::Create("Object/Objects/Viewer", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel ViewerPanel");
	}));

	NxFr::Factory<ViewerContext>& ViewerPanel::GetFactory()
	{
		static NxFr::Factory<ViewerContext> Factory;
		return Factory;
	}

	void ViewerPanel::Clear()
	{
		if (!Context)
		{
			return;
		}

		Context->Clear();

		delete Context;
		Context = nullptr;
	}

	void ViewerPanel::Show(NxEn::Object* Instance)
	{
		Clear();

		if (!Instance)
		{
			return;
		}

		Panel::Show();

		Context = GetFactory().Create(Instance->GetObjectType());
		if (!Context)
		{
			NX_LOG(Warning, Default, "No ViewerContext is associated to this object %llu", Instance->GetId());
			return;
		}

		Context->Setup(Instance);
	}

	void ViewerPanel::OnInitialize()
	{
		Panel::OnInitialize();

		SetNameId("Viewer");
		SetImGuiFlag(ImGuiWindowFlags_MenuBar, true);
	}

	void ViewerPanel::OnDraw()
	{
		if (!Context)
		{
			return;
		}

		Context->Draw();
	}
}
