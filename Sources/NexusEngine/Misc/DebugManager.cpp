#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Misc/DebugManager.h"

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

	DebugManager::DebugManager(NxFr::StringView Folder, bool AutoStart)
	{
		NEXUS_ASSERT(!Folder.IsEmpty(), Default, "Folder can't be empty");

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
	}

	DebugManager::~DebugManager()
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
	}

	void DebugManager::Flush()
	{
		Stats->Unlock();
		Stats->Flush();
		Stats->Lock();

		Logger->Flush();
	}
}
