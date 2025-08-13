#include "NexusEditor/Systems/Editor/EditorWindow.h"
#include "NexusEditor/Systems/Editor/EditorSystem.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(EditorWindow)

	EditorWindow::EditorWindow()
		: GuiFlags(0), Style()
	{
		SetManual(true);
	}

	EditorWindow::~EditorWindow()
	{
	}

	void EditorWindow::OnInitialize()
	{
		Element::OnInitialize();

		GuiFlags =
			ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse |
			ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing |
			ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings;

		Style.AppendVar(ImGuiStyleVar_WindowRounding, 0.0f);
		Style.AppendVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		Style.AppendVarXY(ImGuiStyleVar_WindowPadding, NxFr::Vector2f(0.0f));

		NxEn::GUISystem::SetWindow(this);
		NxEn::GUISystem::GetMenu()->Show();
	}

	void EditorWindow::OnShutdown()
	{
		NxEn::GUISystem::GetMenu()->Hide();
		NxEn::GUISystem::SetWindow(nullptr);

		Element::OnShutdown();
	}

	void EditorWindow::OnTick(float TimeStep)
	{
		ImGuiViewport* Viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(Viewport->Pos);
		ImGui::SetNextWindowSize(Viewport->Size);

		Style.Push();
		if (ImGui::Begin("Editor", nullptr, GuiFlags))
		{
			Style.Pop();

			OnGui(TimeStep);
		}
		ImGui::End();
	}

	void EditorWindow::OnGui(float TimeStep)
	{
		ImGui::DockSpace(ImGui::GetID("Editor"));
	}
}
