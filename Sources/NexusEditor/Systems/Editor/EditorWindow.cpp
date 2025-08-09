#include "NexusEditor/Systems/Editor/EditorWindow.h"
#include "NexusEditor/Systems/Editor/EditorSystem.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"

namespace NxEd
{
	const NxFr::StringView Extension = ".txt";
	const NxFr::StringView SavedLayout = "layout";
	const NxFr::StringView FolderLayout = "layouts";

	class EditorLayoutPopup : public NxEn::GUI::Popup
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, EditorLayoutPopup)

		EditorLayoutPopup()
			: Name(64)
		{
		}

	protected:
		void OnInitialize() override
		{
			Popup::OnInitialize();

			Title = "Save Layout";
			AddButton("Save", { this, &EditorLayoutPopup::Save });
		}

		void OnGui(float TimeStep) override
		{
			ImGui::TextUnformatted("Name:");
			ImGui::SameLine();
			ImGui::InputText("##Name", Name.C_Buffer(), Name.GetCapacity());
		}

		void Save()
		{
			Name.Validate();
			NxEn::Application::GetInstance()->GetSystem<EditorSystem>()->GetWindow().SaveLayout(Name);
		}

	private:
		NxFr::String Name;
	};

	NEXUS_OBJECT_IMPLEMENTATION(EditorLayoutPopup)

	NEXUS_OBJECT_IMPLEMENTATION(EditorWindow)

	EditorWindow::EditorWindow()
		: GuiFlags(0), Style()
	{
	}

	EditorWindow::~EditorWindow()
	{
	}

	void EditorWindow::LoadLayout(NxFr::StringView Name) const
	{
		NxFr::Collection<NxFr::KeyValuePair<const NxFr::StringId, NxEn::GUI::Panel*>> Panels = NxEn::GUISystem::GetAllPanels();
		LoadLayout(GetLayoutPath(Name), Panels);
	}

	void EditorWindow::SaveLayout() const
	{
		EditorLayoutPopup* Popup = NxEn::Object::Create<EditorLayoutPopup>();
	}

	void EditorWindow::SaveLayout(NxFr::StringView Name) const
	{
		NxFr::Collection<NxFr::KeyValuePair<const NxFr::StringId, NxEn::GUI::Panel*>> Panels = NxEn::GUISystem::GetAllPanels();
		SaveLayout(GetLayoutPath(Name), Panels);
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

		NxFr::Collection<NxFr::KeyValuePair<const NxFr::StringId, NxEn::GUI::Panel*>> Panels = NxEn::GUISystem::GetAllPanels();
		LoadLayout(GetLayoutPath(), Panels);

		NxEn::GUI::Menu* Menu = NxEn::GUISystem::GetMenu();
		RecordPanels(*Menu, Panels);
		RecordLayouts(*Menu);
		Menu->Show();
	}

	void EditorWindow::OnShutdown()
	{
		NxEn::GUI::Menu* Menu = NxEn::GUISystem::GetMenu();
		Menu->Hide();

		NxFr::Collection<NxFr::KeyValuePair<const NxFr::StringId, NxEn::GUI::Panel*>> Panels = NxEn::GUISystem::GetAllPanels();
		SaveLayout(GetLayoutPath(), Panels);

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
			ImGui::End();
		}
	}

	void EditorWindow::OnGui(float TimeStep)
	{
		ImGui::DockSpace(ImGui::GetID("Editor"));
	}

	void EditorWindow::RecordPanels(NxEn::GUI::Menu& Menu, NxFr::Collection<NxFr::KeyValuePair<const NxFr::StringId, NxEn::GUI::Panel*>>& Panels) const
	{
		for (auto& It = Panels.Reset(); It != Panels.End(); ++It)
		{
			NxEn::GUI::Panel* Panel = It->Value;
			NxFr::Delegate<void()> Callback = [=]() { Panel->ShowWithTarget(true); };
			Menu.AddMenuItem(Callback, "Window/Panels/" + Panel->GetTitle(), "");
		}
	}

	void EditorWindow::RecordLayouts(NxEn::GUI::Menu& Menu)
	{
		NxFr::Directory Folder(GetLayoutPath("Dummy").GetDirectoryPath());
		NxFr::List<NxFr::String> Layouts = Folder.GetFiles();

		for (auto& Layout : Layouts)
		{
			NxFr::String Name = NxFr::Path::GetFileName(Layout).ToString();
			NxFr::String Path = "Window/Layouts/" + Name;
			Menu.AddMenuItem([=]() { NxEn::Application::GetInstance()->GetSystem<EditorSystem>()->GetWindow().LoadLayout(Name); }, Path);
		}

		NxFr::Delegate<void()> Callback = []() { NxEn::Application::GetInstance()->GetSystem<EditorSystem>()->GetWindow().SaveLayout(); };
		Menu.AddMenuItem(Callback, "Window/Layouts/Save", "", 1);
	}

	void EditorWindow::LoadLayout(const NxFr::Path& Path, NxFr::Collection<NxFr::KeyValuePair<const NxFr::StringId, NxEn::GUI::Panel*>>& Panels) const
	{
		if (!Path.Exist())
		{
			return;
		}

		NxFr::TextStream Stream(Path);
		Stream.Open(NxFr::File::Mode::Read, false);

		NxFr::Set<NxFr::StringId> Ids;
		while (!Stream.IsAtTheEnd())
		{
			Ids.Append(NxFr::StringId(Stream.Read()));
		}

		for (auto& It = Panels.Reset(); It != Panels.End(); ++It)
		{
			It->Value->ShowWithTarget(Ids.Contains(It->Key));
		}

		Stream.Close();
	}

	void EditorWindow::SaveLayout(const NxFr::Path& Path, NxFr::Collection<NxFr::KeyValuePair<const NxFr::StringId, NxEn::GUI::Panel*>>& Panels) const
	{
		NxFr::TextStream Stream(Path);
		Stream.Open(NxFr::File::Mode::Write, true);

		for (auto& It = Panels.Reset(); It != Panels.End(); ++It)
		{
			if (It->Value->IsEnabled())
			{
				Stream.Write(It->Key.C());
			}
		}

		Stream.Close();
	}

	NxFr::Path EditorWindow::GetLayoutPath(NxFr::StringView Name) const
	{
		NxFr::Path Path = NxFr::Path("");
		if (!Name.IsEmpty())
		{
			Path = NxFr::Path(NxFr::Paths::Configs.ToView()) + FolderLayout + (Name + Extension);
		}
		else
		{
			Path = NxFr::Path(NxFr::Paths::Saved.ToView()) + (SavedLayout + Extension);
		}

		NxFr::Directory(Path.GetDirectoryPath()).Create();
		return Path;
	}
}
