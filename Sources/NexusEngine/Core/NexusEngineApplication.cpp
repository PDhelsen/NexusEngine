#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"
#include "NexusFramework/Core/NexusFrameworkGlobals.h"

namespace NxEn
{
	NEXUS_APPLICATION_IMPLEMENTATION(::NxEn::NexusEngineApplication)

	static void Tick()
	{
		TimeManager& Time = Application::GetInstance()->GetTime();
		NEXUS_LOG(Info, Default, "Tick - %.2f", Time.GetDeltaTime());

		if (Time.GetFrameIndex() > 100)
		{
			Application::GetInstance()->Quit();
		}

		NxFr::AllocatorContext Context(MemorySystem::Allocator(AllocatorType::Temp, 512));
		uint64* Pointer = new uint64[1];
		NxFr::Handle<uint64> Handle = NxFr::Memory::Create<uint64>(MemorySystem::Handles());
	}

	void NexusEngineApplication::OnInitialize(NxEn::Bootstrapper& Bootstrap, NxEn::SystemManager& Systems)
	{
		Application::OnInitialize(Bootstrap, Systems);

		Bootstrap.AppendSystem(Systems.CreateSystem<DebugSystem>());
		Bootstrap.AppendSystem(Systems.CreateSystem<MemorySystem>());
	}

	void NexusEngineApplication::OnShutdown(NxEn::Bootstrapper& Unbootstrap, NxEn::SystemManager& Systems)
	{
		Unbootstrap.AppendSystem(Systems.GetSystem<MemorySystem>());
		Unbootstrap.AppendSystem(Systems.GetSystem<DebugSystem>());

		Unbootstrap.AppendStep([&]()
		{
			NEXUS_LOG(Info, Default, "Application last for %d seconds", (uint64)Application::GetInstance()->GetTime().GetUnscaledTime());
		}, "Application duration");

		Application::OnShutdown(Unbootstrap, Systems);
	}

	void NexusEngineApplication::OnExecute(NxEn::Ticker& Ticks, NxEn::SystemManager& Systems)
	{
		Application::OnExecute(Ticks, Systems);

		Ticks.AppendSystem(Systems.GetSystem<DebugSystem>(), NxEn::Ticker::TickBucket::Cleanup).AppendDependency<DebugSystem, MemorySystem>();
		Ticks.AppendSystem(Systems.GetSystem<MemorySystem>(), NxEn::Ticker::TickBucket::Cleanup);

		Ticks.AppendTickCallback(&Tick);
		Ticks.AppendTickOnceCallback([]() { NEXUS_LOG(Info, Default, "Tick Once Callback"); });
	}
}
