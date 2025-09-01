#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Systems/SystemManager.h"

namespace NxEn
{
	SystemManager::SystemManager()
		: Systems()
	{
	}

	SystemManager::~SystemManager()
	{
	}

	System* SystemManager::GetSystem(NxFr::StringId Type) const
	{
		auto Instance = Systems.TryGet(Type);
		return Instance ? *Instance : nullptr;
	}

	void SystemManager::RegisterSystem(System* Instance)
	{
		Systems.Append(Instance->GetObjectType(), Instance);
	}

	void SystemManager::UnregisterSystem(System* Instance)
	{
		Systems.Remove(Instance->GetObjectType());
	}

	NxFr::Array<System*> SystemManager::SortSystems(NxFr::Dictionary<NxFr::StringId, SystemDependencies>& SystemsDependencies) const
	{
		NxFr::Array<System*> Result = NxFr::Array<System*>(SystemsDependencies.GetCount());
		NxFr::Queue<NxFr::StringId> Queue;
		uint64 Index = 0;

		for (auto& [Type, Dependencies] : SystemsDependencies)
		{
			for (auto& Dependency : Dependencies.Dependencies)
			{
				SystemsDependencies[Dependency].Dependents.Append(Type);
			}
			
			Dependencies.WaitOn = Dependencies.Dependencies.GetCount();
			if (Dependencies.WaitOn == 0)
			{
				Queue.Append(Type);
			}
		}

		NEXUS_ASSERT(!Queue.IsEmpty(), Application, "Impossible to start sorting the systems because each one depened on another one, probably circular dependency.");

		while (!Queue.IsEmpty())
		{
			NxFr::StringId Type = Queue.Get();
			Queue.Remove();
			
			SystemDependencies& Dependencies = SystemsDependencies[Type];
			Result[Index++] = const_cast<System*>(Systems[Type]);
			
			for (auto& Dependent : Dependencies.Dependents)
			{
				if (--SystemsDependencies[Dependent].WaitOn == 0)
				{
					Queue.Append(Dependent);
				}
			}
			
			Dependencies.Dependencies.Clear();
			Dependencies.Dependents.Clear();
		}
		
		NEXUS_ASSERT(Index == SystemsDependencies.GetCount(), Application, "Some Systems were not sorted, probaly unable to resolve all the dependencies");

		return Result;
	}
}
