#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Debug/ConsolePanel.h"

namespace NxEn
{
	static ConsolePanel* Panel = GUI::Panel::Create<ConsolePanel>();

	NEXUS_OBJECT_IMPLEMENTATION(ConsolePanel)

	ConsolePanel::ConsolePanel()
		: Menu(), Styles(), Commands(nullptr), Logger(nullptr), Command(256), Search(64), LoggerFlags(), LoggerLines()
	{
		GUI::Style StyleInfo;
		StyleInfo.AppendColor(ImGuiCol_Text, NxFr::Color(0.95f, 0.95f, 0.95f, 1.0f));
		GUI::Style StyleWarning;
		StyleWarning.AppendColor(ImGuiCol_Text, NxFr::Color(0.95f, 0.85f, 0.05f, 1.0f));
		GUI::Style StyleError;
		StyleError.AppendColor(ImGuiCol_Text, NxFr::Color(0.95f, 0.05f, 0.05f, 1.0f));
		GUI::Style StyleFatal;
		StyleFatal.AppendColor(ImGuiCol_Text, NxFr::Color(0.85f, 0.05f, 0.55f, 1.0f));

		Styles = NxFr::Array<GUI::Style>(NxFr::Enum::ToIndex(NxFr::LoggerVerbosity::COUNT));
		Styles[NxFr::Enum::ToIndex(NxFr::LoggerVerbosity::Info)] = StyleInfo;
		Styles[NxFr::Enum::ToIndex(NxFr::LoggerVerbosity::Warning)] = StyleWarning;
		Styles[NxFr::Enum::ToIndex(NxFr::LoggerVerbosity::Error)] = StyleError;
		Styles[NxFr::Enum::ToIndex(NxFr::LoggerVerbosity::Fatal)] = StyleFatal;
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

		Logger->RegisterCallback({ this, &ConsolePanel::AddLogs });

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
		Logger->UnregisterCallback({ this, &ConsolePanel::AddLogs });

		LoggerFlags.Clear();
		Menu.Clear();

		Panel::OnDisable();
	}

	void ConsolePanel::OnGui(float TimeStep)
	{
		static float ButtonWidthSearch = 250;
		static float ButtonWidthExec = 150.0f;
		static float LineHeight = ImGui::GetTextLineHeightWithSpacing() + 10.0f;

		// Menu
		{
			Menu.Tick(TimeStep);

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::Button("Clear"))
				{
					ClearLogs();
				}

				ImGui::Dummy({ GUI::Utils::AvailableSpaceHorizontal(ButtonWidthSearch), ImGui::GetFrameHeight() });

				ImGui::TextUnformatted("Search:");
				if (ImGui::InputText("##ConsoleCmd", Search.C_Buffer(), Search.GetCapacity(), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					Search.Validate();
				}
			}
			ImGui::EndMenuBar();
		}

		// Logs
		{
			ImGui::BeginChild("##Logs", { 0, GUI::Utils::AvailableSpaceVertical(LineHeight) }, 0, ImGuiWindowFlags_HorizontalScrollbar);

			for (auto& Line : LoggerLines)
			{
				GUI::Scope::Style Style(Styles[Line.GetSecond()]);
				ImGui::Text(Line.GetFirst().C());
			}

			ImGui::EndChild();
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

	void ConsolePanel::AddLogs(NxFr::LoggerVerbosity Verbosity, NxFr::StringId Channel, NxFr::StringView Message)
	{
		LoggerLines.AppendConstruct(Message.C(), NxFr::Enum::ToIndex(Verbosity));
	}

	void ConsolePanel::ClearLogs()
	{
		LoggerLines.Clear();
	}
}
