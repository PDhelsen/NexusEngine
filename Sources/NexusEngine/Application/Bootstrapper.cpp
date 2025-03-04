#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Bootstrapper.h"

#include "NexusEngine/Application/Application.h"

namespace NxEn
{
	Bootstrapper::StepInfo::StepInfo(const Signature& Target, NxFr::StringView Tag)
		: Target(Target), Tag(Tag)
	{
	}

	Bootstrapper::SystemInfo::SystemInfo(System* Target, NxFr::StringView Tag)
		: Target(Target), Tag(Tag), Remaining(0), Dependencies(), Dependents()
	{
	}

	Bootstrapper::Bootstrapper()
	{
	}

	Bootstrapper::~Bootstrapper()
	{
	}

	void Bootstrapper::Boot()
	{
		ExecuteSteps();
		CreateSystems();
	}

	void Bootstrapper::Unboot()
	{
		DestroySystems();
		ExecuteSteps();
	}

	void Bootstrapper::ExecuteSteps()
	{
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
		Application* Instance = Application::GetInstance();
		NxFr::Array<SystemInfo*> Infos = SortSystems();

		for (auto It = Infos.Begin(); It != Infos.End() && Instance->IsRunning(); ++It)
		{
			NEXUS_LOG(Info, Default, "Bootstrapper - System (%i / %i) %s", It.Id() + 1, Infos.GetCount(), It.Get()->Tag.C());
			It.Get()->Target->Initialize();
		}
	}

	void Bootstrapper::DestroySystems()
	{
		Application* Instance = Application::GetInstance();
		NxFr::Array<SystemInfo*> Infos = SortSystems();

		for (auto It = Infos.BeginReverse(); It != Infos.EndReverse() && Instance->IsRunning(); --It)
		{
			NEXUS_LOG(Info, Default, "Bootstrapper - System (%i / %i) %s", It.Id() + 1, Infos.GetCount(), It.Get()->Tag.C());
			It.Get()->Target->Shutdown();

			delete It.Get()->Target;
		}
	}

	NxFr::Array<Bootstrapper::SystemInfo*> Bootstrapper::SortSystems()
	{
		if (GetSystemsCount() == 0)
		{
			return NxFr::Array<SystemInfo*>();
		}

		NxFr::Array<SystemInfo*> Infos = NxFr::Array<SystemInfo*>(GetSystemsCount());
		NxFr::Queue<NxFr::StringId> Queue;
		int Index = 0;

		for (auto& [Name, Info] : SystemInfos)
		{
			for (auto Dependency : Info.Dependencies)
			{
				SystemInfos[Dependency].Dependents.Append(Name);
			}

			Info.Remaining = Info.Dependencies.GetCount();
			if (Info.Remaining == 0)
			{
				Queue.Append(Name);
			}
		}

		NEXUS_ASSERT(!Queue.IsEmpty(), Default, "Impossible to start creating the systems because each one depened on another one, probably circular dependency.");

		while (!Queue.IsEmpty())
		{
			NxFr::StringId Name = Queue.Get();
			Queue.Remove();

			SystemInfo& Info = SystemInfos[Name];
			Infos[Index++] = &Info;

			for (auto Dependent : Info.Dependents)
			{
				if (--SystemInfos[Dependent].Remaining == 0)
				{
					Queue.Append(Dependent);
				}
			}

			Info.Dependents.Clear();
		}

		NEXUS_ASSERT(Index == GetSystemsCount(), Default, "Some Systems were not initialized, probaly unable to resolve all the dependencies");
		return Infos;
	}
}
