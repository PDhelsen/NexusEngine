#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Project/ProjectPanel.h"

namespace NxEn
{
	static const NxEn::GUI::Menu::Item& MenuItemProject = GUI::Menu::Item::Create("File/Project", NxFr::Delegate<void()>([]()
	{
		Application::GetSystem<CommandsSystem>()->Execute("GUI.Panel ProjectPanel");
	}));

	static ProjectPanel* PanelProject = GUI::Panel::Create<ProjectPanel>();

	ProjectPanel::ProjectPanel()
		: Style(), Infos(nullptr)
	{
	}

	ProjectPanel::~ProjectPanel()
	{
	}

	void ProjectPanel::OnInitialize()
	{
		Panel::OnInitialize();

		SetTitle("Project");
	}

	void ProjectPanel::OnEnable()
	{
		Panel::OnEnable();
		Style.Reset();

		Infos = &Application::GetInstance()->GetProject();
	}

	void ProjectPanel::OnDraw()
	{
		GUI::Drawer<NxFr::String>::Property(NxFr::StringUtility::ToString(Infos->GetMode()), "Mode", &Style);
		ImGui::Separator();
		GUI::Drawer<NxFr::String>::Property(Infos->GetName(), "Name", &Style);
		GUI::Drawer<NxFr::String>::Property(Infos->GetRootPath(), "Root", &Style);
		GUI::Drawer<NxFr::String>::Property(Infos->GetProjectPath(), "Path", &Style);
		ImGui::Separator();
		GUI::Drawer<NxFr::String>::Property(Infos->GetExecutablePath(), "Executable", &Style);
		GUI::Drawer<NxFr::String>::Property(Infos->GetDllPath(), "Dll", &Style);
	}
}
