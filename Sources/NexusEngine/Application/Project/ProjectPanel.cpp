#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Project/ProjectPanel.h"

namespace NxEn
{
	static ProjectPanel* Panel = GUI::Panel::Create<ProjectPanel>();

	const static GUI::Menu::Item MenuItemProject = GUI::Menu::Item::Create("File/Project", NxFr::Delegate<void()>([]()
	{
		Application::GetInstance()->GetSystem<CommandsSystem>()->Execute("GUI.Panel,ProjectPanel");
	}));

	NEXUS_OBJECT_IMPLEMENTATION(ProjectPanel)

	ProjectPanel::ProjectPanel()
	{
	}

	ProjectPanel::~ProjectPanel()
	{
	}

	void ProjectPanel::OnInitialize()
	{
		Panel::OnInitialize();

		Title = "Project";
		GuiFlags = ImGuiWindowFlags_NoCollapse;
	}

	void ProjectPanel::OnGui(float TimeStep)
	{
		Project* ProjectInfos = GetProject();

		ImGui::Text("Mode: %s", Enum::ProjectModeToString(ProjectInfos->GetTarget()));
		ImGui::Separator();
		ImGui::Text("Name: %s", ProjectInfos->GetName().C());
		ImGui::Text("Root: %s", ProjectInfos->GetRootPath().C());
		ImGui::Text("Path: %s", ProjectInfos->GetPath().C());
		ImGui::Separator();
		ImGui::Text("Executable: %s", ProjectInfos->GetExecutablePath().C());
		ImGui::Text("Dll: %s", ProjectInfos->GetDllPath().C());
	}

	void* ProjectPanel::FetchDefaultTarget() const
	{
		return &Application::GetInstance()->GetProject();
	}
}
