#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Debug/ConsolePanel.h"

namespace NxEn
{
	static ConsolePanel* Panel = GUI::Panel::Create<ConsolePanel>();

	NEXUS_OBJECT_IMPLEMENTATION(ConsolePanel)

	ConsolePanel::ConsolePanel()
		: Menu(), Commands(nullptr), Logger(nullptr), Command(256), Search(64), LoggerFlags()
	{
	}

	ConsolePanel::~ConsolePanel()
	{
	}

	void ConsolePanel::OnInitialize()
	{
		Panel::OnInitialize();
		Menu.Initialize();

		GuiFlags |= ImGuiWindowFlags_MenuBar;
		Title = "Console";
	}

	void ConsolePanel::OnShutdown()
	{
		Menu.Shutdown();
		Panel::OnShutdown();
	}

	void ConsolePanel::OnEnable()
	{
		Panel::OnEnable();

		Application* App = Application::GetInstance();
		Commands = App->GetSystem<CommandsSystem>();
		Logger = App->GetSystem<DebugSystem>()->GetLogger();

		for (uint64 Index = 0, Flag = 1; Index < NxFr::Math::LogTwoPowerOfTwo((uint64)NxFr::LoggerVerbosity::COUNT); ++Index, Flag = 1 << Index)
		{
			uint64 Count = LoggerFlags.GetCount();
			LoggerFlags.Append(Logger->CheckVerbosity((NxFr::LoggerVerbosity)Flag));

			NxFr::String Path = NxFr::StringView("Verbosity/") + NxFr::Enum::LoggerVerbosityToString(Index);
			Menu.AddMenuToggle(Path, &LoggerFlags.Last(), [=]() { Logger->SetVerbosity((NxFr::LoggerVerbosity)Flag, LoggerFlags[Count]); }, "", Count);
		}

		NxFr::Array<NxFr::StringId> Channels = Logger->GetChannels();
		Channels.Sort([](const NxFr::StringId& A, const NxFr::StringId& B) { return A.C() <= B.C(); });
		for (auto& Id : Channels)
		{
			uint64 Count = LoggerFlags.GetCount();
			LoggerFlags.Append(Logger->CheckChannel(Id));

			NxFr::String Path = NxFr::StringView("Channels/") + Id.C();
			Menu.AddMenuToggle(Path, &LoggerFlags.Last(), [=]() { Logger->SetChannel(Id, LoggerFlags[Count]); }, "", Count);
		}
	}

	void ConsolePanel::OnDisable()
	{
		LoggerFlags.Clear();
		Menu.Clear();

		Panel::OnDisable();
	}

	void ConsolePanel::OnGui(float TimeStep)
	{
		static float ButtonWidthSearch = 250;
		static float ButtonWidthExec = 150.0f;

		// Menu
		{
			Menu.Tick(TimeStep);

			if (ImGui::BeginMenuBar())
			{
				ImGui::Dummy({ GUI::Utils::SpaceHorizontal(ButtonWidthSearch), ImGui::GetFrameHeight() });
				ImGui::SameLine();

				ImGui::TextUnformatted("Search:");
				ImGui::SameLine();

				if (ImGui::InputText("##ConsoleCmd", Search.C_Buffer(), Search.GetCapacity(), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					Search.Validate();
				}
			}
			ImGui::EndMenuBar();
		}

		ImGui::Separator();

		// Command
		{
			ImGui::TextUnformatted("Command:");
			ImGui::SameLine();

			{
				GUI::Scope::HorizontalFill Fill(ButtonWidthExec);
				if (ImGui::InputText("##ConsoleCmd", Command.C_Buffer(), Command.GetCapacity(), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					ExecuteCommand();
				}
				ImGui::SameLine();
			}

			if (ImGui::Button("Execute##ConsoleCmdExecute", { ButtonWidthExec, ImGui::GetFrameHeight() }))
			{
				ExecuteCommand();
			}
		}
	}

	void ConsolePanel::ExecuteCommand()
	{
		Command.Validate();
		Commands->Run(Command);
		Command.Clear();
	}
}
