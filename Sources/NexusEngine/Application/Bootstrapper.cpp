#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Bootstrapper.h"

namespace NxEn
{
	Bootstrapper::StepInfo::StepInfo(const Signature& Target, NxFr::StringView Tag)
		: Target(Target), Tag(Tag)
	{
	}

	Bootstrapper::SystemInfo::SystemInfo(System* Target, NxFr::StringView Tag)
		: Target(Target), Tag(Tag), WaitOn(0), Dependencies(), Dependents()
	{
	}

	Bootstrapper::Bootstrapper()
	{
	}

	Bootstrapper::~Bootstrapper()
	{
	}

	void Bootstrapper::RunBoot()
	{
		ExecuteSteps();
		CreateSystems();
	}

	void Bootstrapper::RunUnboot()
	{
		DestroySystems();
		ExecuteSteps();
	}

	void Bootstrapper::ExecuteSteps()
	{
		if (GetStepsCount() == 0)
		{
			NEXUS_LOG(Warning, Default, "There is no boot step");
			return;
		}

		Application* Instance = Application::GetInstance();

		for (auto It = StepInfos.Begin(); It != StepInfos.End() && Instance->IsRunning(); ++It)
		{
			NEXUS_LOG(Info, Default, "Bootstrapper - Step (%i / %i) %s", It.Id() + 1, StepInfos.GetCount(), It->Tag.C());
			It->Target.Invoke();
		}

		StepInfos.Clear();
	}

	void Bootstrapper::CreateSystems()
	{
		if (GetSystemsCount() == 0)
		{
			NEXUS_LOG(Warning, Default, "There is no systems to initialize");
			return;
		}

		Application* Instance = Application::GetInstance();
		NxFr::Array<SystemInfo*> Infos = SortSystems();

		for (auto It = Infos.Begin(); It != Infos.End() && Instance->IsRunning(); ++It)
		{
			NEXUS_LOG(Info, Default, "Bootstrapper - Create System (%i / %i) %s", It.Id() + 1, Infos.GetCount(), It.Get()->Tag.C());
			It.Get()->Target->Initialize();
		}

		SystemInfos.Clear();
	}

	void Bootstrapper::DestroySystems()
	{
		if (GetSystemsCount() == 0)
		{
			NEXUS_LOG(Warning, Default, "There is no systems to shutdown");
			return;
		}

		Application* Instance = Application::GetInstance();
		NxFr::Array<SystemInfo*> Infos = SortSystems();

		for (auto It = Infos.Begin(); It != Infos.End() && Instance->IsRunning(); ++It)
		{
			NEXUS_LOG(Info, Default, "Bootstrapper - Destroy System (%i / %i) %s", Infos.GetCount() - It.Id(), Infos.GetCount(), It.Get()->Tag.C());
			It.Get()->Target->Shutdown();

			delete It.Get()->Target;
		}

		SystemInfos.Clear();
	}

	NxFr::Array<Bootstrapper::SystemInfo*> Bootstrapper::SortSystems()
	{
		NxFr::Array<SystemInfo*> Infos = NxFr::Array<SystemInfo*>(GetSystemsCount());
		NxFr::Queue<NxFr::StringId> Queue;
		int Index = 0;

		for (auto& [Type, Info] : SystemInfos)
		{
			for (auto& Dependency : Info.Dependencies)
			{
				SystemInfos[Dependency].Dependents.Append(Type);
			}

			Info.WaitOn = Info.Dependencies.GetCount();
			if (Info.WaitOn == 0)
			{
				Queue.Append(Type);
			}
		}

		NEXUS_ASSERT(!Queue.IsEmpty(), Default, "Impossible to start sorting the systems because each one depened on another one, probably circular dependency.");

		while (!Queue.IsEmpty())
		{
			NxFr::StringId Type = Queue.Get();
			Queue.Remove();

			SystemInfo& Info = SystemInfos[Type];
			Infos[Index++] = &Info;

			for (auto& Dependent : Info.Dependents)
			{
				if (--SystemInfos[Dependent].WaitOn == 0)
				{
					Queue.Append(Dependent);
				}
			}

			Info.Dependents.Clear();
			Info.Dependencies.Clear();
		}

		NEXUS_ASSERT(Index == GetSystemsCount(), Default, "Some Systems were not sorted, probaly unable to resolve all the dependencies");
		return Infos;
	}
}
