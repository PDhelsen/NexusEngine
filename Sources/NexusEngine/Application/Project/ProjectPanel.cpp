#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Project/ProjectPanel.h"

namespace NxEn
{
	static ProjectPanel* Panel = GUI::Panel::Create<ProjectPanel>();

	const static GUI::Menu::Item MenuItemProject = GUI::Menu::Item::Create("File/Project", NxFr::Delegate<void()>([]()
	{
		Application::GetSystem<CommandsSystem>()->Execute("GUI.Panel ProjectPanel");
	}));

	NEXUS_OBJECT_IMPLEMENTATION(ProjectPanel)

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
		Style.Width = -1.0f;
		Style.WidthLabel = -1.0f;

		Infos = &Application::GetInstance()->GetProject();
	}

	void ProjectPanel::OnGui(float TimeStep)
	{
		GUI::Drawer<NxFr::String>::Property(NxFr::StringUtility::ToString(Infos->GetTarget()), "Mode", &Style);
		ImGui::Separator();
		GUI::Drawer<NxFr::String>::Property(Infos->GetName(), "Name", &Style);
		GUI::Drawer<NxFr::String>::Property(Infos->GetRootPath(), "Root", &Style);
		GUI::Drawer<NxFr::String>::Property(Infos->GetPath(), "Path", &Style);
		ImGui::Separator();
		GUI::Drawer<NxFr::String>::Property(Infos->GetExecutablePath(), "Executable", &Style);
		GUI::Drawer<NxFr::String>::Property(Infos->GetDllPath(), "Dll", &Style);
	}
}
