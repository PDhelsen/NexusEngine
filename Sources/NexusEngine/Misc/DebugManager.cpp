#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Misc/DebugManager.h"

namespace NxEn
{
#if NEXUS_DEBUG
	bool FlushOnLog = true;
#else
	bool FlushOnLog = false;
#endif

	DebugManager::DebugManager(NxFr::StringView Folder, bool AutoStart)
	{
		NEXUS_ASSERT(!Folder.IsEmpty(), Default, "Folder can't be empty");

		NxFr::Directory(Folder).Create();

		Logger = new NxFr::Logger(FlushOnLog, NxFr::LoggerVerbosity::All, NxFr::LoggerOutput::All, Folder + "logs.txt");
		Logger->AddChannel(NxFr::LoggerChannel::Default, true);
		Logger->AddChannel(NxFr::LoggerChannel::Verbose, false);

		Stats = new NxFr::Stats(Folder + "stats.csv");
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
