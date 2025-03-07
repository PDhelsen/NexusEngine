#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"
#include "NexusFramework/Core/NexusFrameworkGlobals.h"

#include "NexusEngine/Systems/Debug/DebugSystem.h"

namespace NxEn
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxEn::NexusEngineApplication)

	static void InitializeFolders()
	{
		NxFr::Directory(NxFr::Paths::Cooked).Create();
		NxFr::Directory(NxFr::Paths::Saved).Create();
	}

	static void InitializeDebug()
	{
		bool Profile = NxFr::Arguments::HasFlag("Profile", false);

		NxFr::Path DebugPath = NxFr::Paths::Saved + NxFr::Arguments::GetValue("DebugFolder", "debug");
		NxFr::Directory(DebugPath).Create();

		NxFr::Logger* Logs = new NxFr::Logger(false, NxFr::LoggerVerbosity::All, NxFr::LoggerOutput::All, DebugPath + "logs.txt");
		Logs->AddChannel(NxFr::LoggerChannel::Default, true);
		Logs->AddChannel(NxFr::LoggerChannel::Verbose, false);
		NxFr::Globals::Logs = Logs;

		NxFr::Stats* Statistiques = new NxFr::Stats(DebugPath + "stats.csv");
		Statistiques->Initialize();
		if (Profile)
		{
			Statistiques->StartRecording();
		}
		NxFr::Globals::Statistiques = Statistiques;

		NxFr::Instruments* Instrumentor = NxFr::Instruments::Create(DebugPath + "instruments.json", false);
		if (Profile)
		{
			Instrumentor->StartRecording();
		}
		NxFr::Globals::Instrumentor = Instrumentor;
	}

	static void ShutdownDebug()
	{
		NxFr::Logger* Logs = NxFr::Globals::Logs;
		NxFr::Globals::Logs = nullptr;
		delete Logs;

		NxFr::Stats* Statistiques = NxFr::Globals::Statistiques;
		if (Statistiques->IsRecording())
		{
			Statistiques->StopRecording();
		}
		NxFr::Globals::Statistiques = nullptr;
		delete Statistiques;

		NxFr::Instruments* Instrumentor = NxFr::Globals::Instrumentor;
		if (Instrumentor->IsRecording())
		{
			Instrumentor->StopRecording();
		}
		NxFr::Globals::Instrumentor = nullptr;
		NxFr::Instruments::Destroy(Instrumentor);
	}

	void NexusEngineApplication::OnInitialize(NxEn::Bootstrapper& Bootstrap)
	{
		Bootstrap.AddStep(&InitializeFolders, "Generate Folders");
		Bootstrap.AddStep(&InitializeDebug, "Initialize Debug Globals");

		Bootstrap.AddSystem<DebugSystem>();
	}

	void NexusEngineApplication::OnShutdown(NxEn::Bootstrapper& Bootstrap)
	{
		Bootstrap.AddStep(&ShutdownDebug, "Shutdown Debug Globals");
	}
}
