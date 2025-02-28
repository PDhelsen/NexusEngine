#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Bootstrapper.h"

namespace NxEn
{
	Bootstrapper::SystemInfo::SystemInfo(System* Target)
		: Target(Target), Remaining(0), Dependencies(), Dependents()
	{
	}

	Bootstrapper::Bootstrapper()
	{
	}

	Bootstrapper::~Bootstrapper()
	{
	}

	void Bootstrapper::Boot(NxFr::Array<System*>* Systems)
	{
		ExecuteSteps();
		CreateSystems(*Systems);
	}

	void Bootstrapper::Unboot(NxFr::Array<System*>* Systems)
	{
		DestroySystems(*Systems);
		ExecuteSteps();
	}

	void Bootstrapper::ExecuteSteps()
	{
		Steps.Invoke();
		Steps.Clear();
	}

	void Bootstrapper::CreateSystems(NxFr::Array<System*>& Systems)
	{
		Systems = NxFr::Array<System*>(SystemInfos.GetCount());

		SortSystems(Systems);
		for (auto It = Systems.Begin(); It != Systems.End(); ++It)
		{
			(*It)->Initialize();
		}
	}

	void Bootstrapper::DestroySystems(NxFr::Array<System*>& Systems)
	{
		SortSystems(Systems);
		for (auto It = Systems.BeginReverse(); It != Systems.EndReverse(); --It)
		{
			(*It)->Shutdown();
			delete *It;
		}
	}

	void Bootstrapper::SortSystems(NxFr::Array<System*>& Systems)
	{
		int Index = 0;
		NxFr::Queue<NxFr::StringView> Queue;

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
			NxFr::StringView Name = Queue.Get();
			Queue.Remove();

			SystemInfo& Info = SystemInfos[Name];
			Systems[Index++] = Info.Target;

			for (auto Dependent : Info.Dependents)
			{
				if (--SystemInfos[Dependent].Remaining == 0)
				{
					Queue.Append(Dependent);
				}
			}

			Info.Dependents.Clear();
		}

		NEXUS_ASSERT(Index == Systems.GetCount(), Default, "Some Systems were not initialized, probaly unable to resolve all the dependencies");
	}
}
