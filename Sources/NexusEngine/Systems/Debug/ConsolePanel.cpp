#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Debug/ConsolePanel.h"

namespace NxEn
{
	static NxFr::StringId TextInfo = "Info"_Sid;
	static NxFr::StringId TextWarning = "Warning"_Sid;
	static NxFr::StringId TextError = "Error"_Sid;
	static NxFr::StringId TextFatal = "Fatal"_Sid;
	static NxFr::StringId WidthButton = "WidthButton"_Sid;
	static NxFr::StringId WidthInpuText = "WidthInpuText"_Sid;

	static ConsolePanel* Panel = GUI::Panel::Create<ConsolePanel>();

	static NxEn::SettingVar* SettingConsoleAutoScroll = NxEn::SettingVar::Create("Preferences", "ConsoleAutoScroll", NxEn::Settings::Type::Bool);

	NEXUS_OBJECT_IMPLEMENTATION(ConsolePanel)

	NxFr::StringId ConsolePanel::GetStyle(NxFr::LoggerVerbosity Verbosity)
	{
		NxFr::StringId Id = 0;

		switch (Verbosity)
		{
		case NxFr::LoggerVerbosity::Info: Id = TextInfo; break;
		case NxFr::LoggerVerbosity::Warning: Id = TextWarning; break;
		case NxFr::LoggerVerbosity::Error: Id = TextError; break;
		case NxFr::LoggerVerbosity::Fatal: Id = TextFatal; break;

		case NxFr::LoggerVerbosity::None:
		case NxFr::LoggerVerbosity::All:
		case NxFr::LoggerVerbosity::COUNT:
		default: Id = 0; break;
		}

		return Id;
	}

	ConsolePanel::ConsolePanel()
		: Menu(), Logs(), FlagsVerbosity(), FlagsChannels(), Command(128), Search(128), Scroll(false)
	{
	}

	ConsolePanel::~ConsolePanel()
	{
	}

	void ConsolePanel::OnInitialize()
	{
		Panel::OnInitialize();
		Menu.Initialize();

		SetGuiFlag(ImGuiWindowFlags_MenuBar);
		SetTitle("Console");
	}

	void ConsolePanel::OnShutdown()
	{
		Menu.Shutdown();
		Panel::OnShutdown();
	}

	void ConsolePanel::OnEnable()
	{
		Panel::OnEnable();

		NxFr::Logger* Logger = Application::GetSystem<DebugSystem>()->GetLogger();
		Logger->RegisterCallback({ this, &ConsolePanel::AddLogs });

		uint8 VerbosityCount = NxFr::Enum::FlagIndex(NxFr::LoggerVerbosity::COUNT);
		FlagsVerbosity.Reserve(VerbosityCount);
		for (uint64 Index = 0; Index < VerbosityCount; ++Index)
		{
			uint64 Priority = FlagsVerbosity.GetCount();
			NxFr::LoggerVerbosity Verbosity = (NxFr::LoggerVerbosity)((uint64)1 << Index);
			FlagsVerbosity.Append(Verbosity, Logger->CheckVerbosity(Verbosity));

			NxFr::String Path = NxFr::StringView("Verbosity/") + NxFr::StringUtility::ToString(Verbosity);
			Menu.AddMenuToggle(Path, &FlagsVerbosity[Verbosity], nullptr, "", Priority);
		}

		NxFr::Array<NxFr::StringId> Channels = Logger->GetChannels();
		Channels.Sort([](const NxFr::StringId& A, const NxFr::StringId& B) { return A.C() <= B.C(); });
		FlagsChannels.Reserve(Channels.GetCount());
		for (uint64 Index = 0; Index < Channels.GetCount(); ++Index)
		{
			uint64 Priority = FlagsVerbosity.GetCount() + FlagsChannels.GetCount();
			NxFr::StringId Id = Channels[Index];
			FlagsChannels.Append(Id, Logger->CheckChannel(Id));

			NxFr::String Path = NxFr::StringView("Channels/") + Id.C();
			Menu.AddMenuToggle(Path, &FlagsChannels[Id], nullptr, "", Priority);
		}

		Menu.AddMenuToggle("Settings/AutoScroll", &SettingConsoleAutoScroll->As<bool>());
	}

	void ConsolePanel::OnDisable()
	{
		FlagsVerbosity.Clear();
		FlagsChannels.Clear();
		Menu.Clear();

		NxFr::Logger* Logger = Application::GetSystem<DebugSystem>()->GetLogger();
		Logger->UnregisterCallback({ this, &ConsolePanel::AddLogs });

		Panel::OnDisable();
	}

	void ConsolePanel::OnGui(float TimeStep)
	{
		// Menu
		{
			Menu.Tick(TimeStep);

			if (ImGui::BeginMenuBar())
			{
				ImGui::SetCursorPosX(GUI::Utils::Fill(NxFr::Vector2f(ImGui::CalcTextSize("Search:").x + GUI::Style::GetVar(WidthInpuText) + GUI::Style::GetVar(WidthButton)), 3, false, true).x);

				ImGui::AlignTextToFramePadding();
				ImGui::Text("Search:");
				{
					ImGui::SetNextItemWidth(GUI::Style::GetVar(WidthInpuText));
					if (ImGui::InputText("##Search", Search.Characters(), Search.GetCapacity()))
					{
						Search.Validate();
					}
				}

				if (ImGui::Button("Clear Logs", { GUI::Style::GetVar(WidthButton), ImGui::GetFrameHeight() }))
				{
					ClearLogs();
				}
			}
			ImGui::EndMenuBar();
		}

		// Logs
		{
			ImGui::BeginChild("Logs", { 0, GUI::Utils::Fill(NxFr::Vector2f(ImGui::GetTextLineHeightWithSpacing()), 2).y }, 0, ImGuiWindowFlags_HorizontalScrollbar);

			for (uint64 Index = 0; Index < Logs.GetCount(); ++Index)
			{
				Log& Log = Logs[Index];
				if (Log.Verbosity && Log.Channel && (Search.IsEmpty() || NxFr::StringUtility::Contains(Log.Text, Search)))
				{
					GUI::Style::Scope Style(Log.Style);
					ImGui::Text(Log.Text.C());
				}
			}

			if (SettingConsoleAutoScroll->As<bool>() && Scroll)
			{
				ImGui::SetScrollHereY(1.0f);
				Scroll = false;
			}

			ImGui::EndChild();
		}

		ImGui::Separator();

		// Command
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Command:");
			ImGui::SameLine();
			{
				ImGui::SetNextItemWidth(GUI::Utils::Fill(NxFr::Vector2f(GUI::Style::GetVar(WidthButton)), 1, false).x);
				if (ImGui::InputText("##Command", Command.Characters(), Command.GetCapacity(), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					ExecuteCommand();
				}
				ImGui::SameLine();
			}

			if (ImGui::Button("Execute", { GUI::Style::GetVar(WidthButton), ImGui::GetFrameHeight() }))
			{
				ExecuteCommand();
			}
		}
	}

	void ConsolePanel::ExecuteCommand()
	{
		Command.Validate();

		Application::GetSystem<CommandsSystem>()->Run(Command);

		Command.Clear();
	}

	void ConsolePanel::AddLogs(NxFr::LoggerVerbosity Verbosity, NxFr::StringId Channel, NxFr::StringView Message)
	{
		Logs.AppendConstruct(Message, GetStyle(Verbosity), FlagsVerbosity[Verbosity], FlagsChannels[Channel]);
		Scroll = true;
	}

	void ConsolePanel::ClearLogs()
	{
		Logs.Clear();
	}
}
