#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Bootstrapper.h"

namespace NxEn
{
	Bootstrapper::Bootstrapper()
	{
	}

	Bootstrapper::~Bootstrapper()
	{
	}

	void Bootstrapper::AddStep(NxFr::Delegate<void()> Step)
	{
		Steps += Step;
	}

	void Bootstrapper::AddSystem(NxFr::Delegate<System*()> System)
	{
		Dependencies.Append(System);
	}

	void Bootstrapper::Boot(NxFr::Array<System*>& Systems)
	{
		ExecuteSteps();
		Systems = CreateSystems();
	}

	void Bootstrapper::Unboot(NxFr::Array<System*>& Systems)
	{
		DestroySystems(Systems);
		ExecuteSteps();
	}

	void Bootstrapper::ExecuteSteps()
	{
		Steps.Invoke();
		Steps.Clear();
	}

	NxFr::Array<System*> Bootstrapper::CreateSystems()
	{
		NxFr::Array<System*> Systems(Dependencies.GetCount());
		for (uint64 Index = 0, Count = Dependencies.GetCount(); Index < Count; ++Index)
		{
			Systems[Index] = Dependencies[Index].Invoke();
			Systems[Index]->Initialize();
		}

		return Systems;
	}

	void Bootstrapper::DestroySystems(NxFr::Array<System*>& Systems)
	{
		for (uint64 Index = 0, Count = Systems.GetCount(); Index < Count; ++Index)
		{
			Systems[Index]->Shutdown();
			delete Systems[Index];
		}
	}
}
