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

	namespace StatsHeader
	{
		const NxFr::StringId MemoryAllocatedId = "Memory - Allocated"_Sid;
		const NxFr::StringId MemoryAllocationId = "Memory - Allocation"_Sid;
		const NxFr::StringId PlatformMemoryId = "Platform - Memory"_Sid;
	}

	NEXUS_OBJECT_IMPLEMENTATION(DebugSystem)

	void DebugSystem::OnInitialize()
	{
		System::OnInitialize();

		bool AutoStart = NxFr::Arguments::HasFlag("Profile", false);

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

		NxFr::Globals::Logs = Logger;
		NxFr::Globals::Statistiques = Stats;
		NxFr::Globals::Instrumentor = Instrumentor;
	}

	void DebugSystem::OnShutdown()
	{
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

		NxFr::Globals::Logs = nullptr;
		NxFr::Globals::Statistiques = nullptr;
		NxFr::Globals::Instrumentor = nullptr;

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
