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

	void Bootstrapper::Boot()
	{
		ExecuteSteps();
	}

	void Bootstrapper::Unboot()
	{
		ExecuteSteps();
	}

	void Bootstrapper::ExecuteSteps()
	{
		Steps.Invoke();
		Steps.Clear();
	}
}
