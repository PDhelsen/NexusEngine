#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Factory/WorldObjectFactoryContext.h"

namespace NxEn
{
	static NxFr::Stack<WorldObjectFactory*> Factories;

	WorldObjectFactory* WorldObjectFactoryContext::GetFactory()
	{
		return !Factories.IsEmpty() ? Factories.Get() : nullptr;
	}

	WorldObjectFactoryContext::WorldObjectFactoryContext(World* Instance)
	{
		Factories.Append(Instance->Factory);
	}

	WorldObjectFactoryContext::~WorldObjectFactoryContext()
	{
		Factories.Remove();
	}
}
