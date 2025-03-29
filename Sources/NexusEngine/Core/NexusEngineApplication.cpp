#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"
#include "NexusFramework/Core/NexusFrameworkGlobals.h"

namespace NxEn
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxEn::NexusEngineApplication)

	static NxFr::Array<uint64, (uint64)AllocatorType::COUNT> MemoryAllocatorSize()
	{
		NxFr::Array<uint64, (uint64)AllocatorType::COUNT> Sizes = NxFr::Array<uint64, (uint64)AllocatorType::COUNT>();
		Sizes[(uint64)AllocatorType::Raw] = 0;
		Sizes[(uint64)AllocatorType::General] = 1024;
		Sizes[(uint64)AllocatorType::Temp] = 1024;
		Sizes[(uint64)AllocatorType::Temp2] = 1024;
		Sizes[(uint64)AllocatorType::Constant] = 1024;
		Sizes[(uint64)AllocatorType::Small] = 1024;
		Sizes[(uint64)AllocatorType::Managed] = 1024;
		return Sizes;
	}

	static void Tick()
	{
		// TEMP: Avoid looping too fast for now since the app is empty
		//NxFr::Platform::GetInstance()->Sleep(1);

		if (Application::GetInstance()->GetTime().GetFrameIndex() == 5)
		{
			Application::GetInstance()->GetTicker().AppendTickOnceCallback([]() { Application::GetInstance()->GetTime().SetMultiplier(0.1f); });
		}

		if (Application::GetInstance()->GetTime().GetFrameIndex() > 100)
		{
			Application::GetInstance()->Quit();
		}


		NEXUS_LOG(Info, Default, "Memory: %d", NxFr::MemoryTracker::GetInstance()->GetAllocatedAmount());

		NxFr::AllocatorContext Context(MemoryManager::Allocator(AllocatorType::General, 512));
		uint64* Pointer = new uint64[1];

		NxFr::Handle<uint64> Handle = NxFr::Memory::Create<uint64>(MemoryManager::Handles());
	}

	void NexusEngineApplication::OnInitialize(NxEn::Bootstrapper& Bootstrap, NxEn::SystemManager& Systems)
	{
		Application::OnInitialize(Bootstrap, Systems);

		Bootstrap.AppendStep([&]()
		{
			NxFr::Path Folder = NxFr::Paths::Saved + NxFr::Arguments::GetValue("DebugFolder", "debug");
			bool AutoStart = NxFr::Arguments::HasFlag("Profile", false);
			Debug = new DebugManager(Folder, AutoStart);

			NxFr::Globals::Logs = Debug->GetLogger();
			NxFr::Globals::Statistiques = Debug->GetStats();
			NxFr::Globals::Instrumentor = Debug->GetInstrumentor();

		}, "Initialize Debug Manager");
		Bootstrap.AppendStep([&]() { Memory = new MemoryManager(MemoryAllocatorSize(), { 32, 256 }, 1024, 1.0f); }, "Initialize Memory Manager");

		Bootstrap.AppendSystem(Systems.CreateSystem<System>());
	}

	void NexusEngineApplication::OnShutdown(NxEn::Bootstrapper& Unbootstrap, NxEn::SystemManager& Systems)
	{
		Unbootstrap.AppendSystem(Systems.GetSystem<System>());

		Unbootstrap.AppendStep([&]()
		{
			NEXUS_LOG(Info, Default, "Application last for %d seconds", (uint64)Application::GetInstance()->GetTime().GetUnscaledTime());
		}, "Application duration");
		Unbootstrap.AppendStep([&]() { delete Memory; Memory = nullptr; }, "Shutdown Memory Manager");
		Unbootstrap.AppendStep([&]()
		{
			delete Debug;
			Debug = nullptr;

			NxFr::Globals::Logs = nullptr;
			NxFr::Globals::Statistiques = nullptr;
			NxFr::Globals::Instrumentor = nullptr;
		}, "Shutdown Debug Manager");

		Application::OnShutdown(Unbootstrap, Systems);
	}

	void NexusEngineApplication::OnExecute(NxEn::Ticker& Ticks, NxEn::SystemManager& Systems)
	{
		Application::OnExecute(Ticks, Systems);

		Ticks.AppendTickCallback({ Debug, &DebugManager::Flush }, Ticker::TickBucket::Cleanup, "Debug Manager");
		Ticks.AppendTickCallback({ Memory, &MemoryManager::Tick }, Ticker::TickBucket::Cleanup, "Memory Manager");

		Ticks.AppendTickCallback(&Tick);
		Ticks.AppendTickOnceCallback([]() { NEXUS_LOG(Info, Default, "Tick Once Callback"); });

		Ticks.AppendSystem(Systems.GetSystem<System>(), NxEn::Ticker::TickBucket::Engine);
	}
}
