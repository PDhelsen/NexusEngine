#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Debug/DebugSystem.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"
#include "NexusFramework/Core/NexusFrameworkGlobals.h"

namespace NxEn
{
#if NEXUS_DEBUG
	bool FlushOnLog = true;
#else
	bool FlushOnLog = false;
#endif

	const static Command CmdDebugLoggerChannel = Command::Create("Debug.Logger.Channel"_Sid, "Enable/Disable logger channel", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Channel, NxFr::StringView Enabled)
	{
		Application::GetSystem<DebugSystem>()->GetLogger()->SetChannel(NxFr::StringId(Channel), Enabled == "true");
	}));
	const static Command CmdDebugLoggerAllChannels = Command::Create("Debug.Logger.AllChannels"_Sid, "Enable/Disable all logger channel", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Enabled)
	{
		Application::GetSystem<DebugSystem>()->GetLogger()->SetAllChannels(Enabled == "true");
	}));
	const static Command CmdDebugProfiler = Command::Create("Debug.Profiler"_Sid, "Enable/Disable profiler", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Enabled)
	{
		NxFr::Instruments* Instrumentor = Application::GetSystem<DebugSystem>()->GetInstrumentor();
		if (Enabled == "true") Instrumentor->StartRecording(); else Instrumentor->StopRecording();
	}));
	const static Command CmdDebugStats = Command::Create("Debug.Stats"_Sid, "Enable/Disable stats", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Enabled)
	{
		NxFr::Stats* Stats = Application::GetSystem<DebugSystem>()->GetStats();
		if (Enabled == "true") Stats->StartRecording(); else Stats->StopRecording();
	}));

	NEXUS_OBJECT_IMPLEMENTATION(DebugSystem)

	DebugSystem::DebugSystem()
		: Logger(nullptr), Stats(nullptr), Instrumentor(nullptr)
	{
	}

	DebugSystem::~DebugSystem()
	{
	}

	void DebugSystem::OnInitialize()
	{
		System::OnInitialize();

		NxFr::Path Folder = NxFr::Paths::Saved + NxFr::Arguments::GetValue("DebugFolder", "debug");
		NEXUS_ASSERT(!Folder.Data.IsEmpty(), Default, "Folder can't be empty");
		NxFr::Directory(Folder).Create();

		Logger = new NxFr::Logger(FlushOnLog, NxFr::LoggerVerbosity::All, NxFr::LoggerOutput::All, Folder + "logs.txt");
		Logger->AddChannel(NxFr::LoggerChannel::Default, true);

		Stats = new NxFr::Stats(Folder + "stats.csv");
		Instrumentor = NxFr::Instruments::Create(Folder + "instruments.json", false);
		
		// Push DebugSystem instance to globals
		NxFr::Globals::Logs = Logger;
		NxFr::Globals::Statistiques = Stats;
		NxFr::Globals::Instrumentor = Instrumentor;
	}

	void DebugSystem::OnShutdown()
	{
		// Remove DebugSystem instance from globals only if they are still globals
		if (NxFr::Globals::Logs == Logger) NxFr::Globals::Logs = nullptr;
		if (NxFr::Globals::Statistiques == Stats) NxFr::Globals::Statistiques = nullptr;
		if (NxFr::Globals::Instrumentor == Instrumentor) NxFr::Globals::Instrumentor = nullptr;

		NxFr::Instruments::Destroy(Instrumentor);
		delete Stats;
		delete Logger;

		System::OnShutdown();
	}

	void DebugSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		FlushTools();
	}

	void DebugSystem::StartTools()
	{
		bool AutoStart = NxFr::Arguments::HasFlag("Profile");

		if (AutoStart)
		{
			Instrumentor->StartRecording();
		}

		Stats->Initialize();
		Stats->Lock();
		if (AutoStart)
		{
			Stats->StartRecording();
		}
	}

	void DebugSystem::StopTools()
	{
		if (Instrumentor->IsRecording())
		{
			Instrumentor->StopRecording();
		}

		if (Stats->IsRecording())
		{
			Stats->StopRecording();
		}
		Stats->Unlock();
		Stats->Flush();

		Logger->Flush();
	}

	void DebugSystem::FlushTools()
	{
		Stats->Unlock();
		Stats->Flush();
		Stats->Lock();

		Logger->Flush();
	}
}
