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

	namespace StatsHeader
	{
		const NxFr::StringId MemoryAllocatedId = "Memory - Allocated"_Sid;
		const NxFr::StringId MemoryAllocationId = "Memory - Allocation"_Sid;
		const NxFr::StringId PlatformMemoryId = "Platform - Memory"_Sid;
	}

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

		bool AutoStart = NxFr::Arguments::HasFlag("Profile");

		NxFr::Path Folder = NxFr::Paths::Saved + NxFr::Arguments::GetValue("DebugFolder", "debug");
		NEXUS_ASSERT(!Folder.Data.IsEmpty(), Default, "Folder can't be empty");
		NxFr::Directory(Folder).Create();

		Logger = new NxFr::Logger(FlushOnLog, NxFr::LoggerVerbosity::All, NxFr::LoggerOutput::All, Folder + "logs.txt");
		Logger->AddChannel(NxFr::LoggerChannel::Default, true);
		Logger->AddChannel(NxFr::LoggerChannel::Verbose, false);

		Stats = new NxFr::Stats(Folder + "stats.csv");
		NEXUS_STAT_HEADER_INSTANCE(Stats, StatsHeader::MemoryAllocatedId, UnsignedInteger, Set);
		NEXUS_STAT_HEADER_INSTANCE(Stats, StatsHeader::MemoryAllocationId, UnsignedInteger, Set);
		NEXUS_STAT_HEADER_INSTANCE(Stats, StatsHeader::PlatformMemoryId, UnsignedInteger, Set);
		Stats->Initialize();
		Stats->Lock();
		if (AutoStart)
		{
			Stats->StartRecording();
		}

		Instrumentor = NxFr::Instruments::Create(Folder + "instruments.json", false);
		if (AutoStart)
		{
			Instrumentor->StartRecording();
		}

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

		Logger->Flush();
		delete Logger;

		if (Stats->IsRecording())
		{
			Stats->StopRecording();
		}
		Stats->Unlock();
		Stats->Flush();
		delete Stats;

		if (Instrumentor->IsRecording())
		{
			Instrumentor->StopRecording();
		}
		NxFr::Instruments::Destroy(Instrumentor);

		System::OnShutdown();
	}

	void DebugSystem::OnTick(float TimeStep)
	{
		System::OnTick(TimeStep);

		Stats->Unlock();
		Stats->Flush();
		Stats->Lock();

		Logger->Flush();
	}
}
