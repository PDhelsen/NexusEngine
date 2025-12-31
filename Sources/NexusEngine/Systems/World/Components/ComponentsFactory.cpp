#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Components/ComponentsFactory.h"

namespace NxEn
{
	static NxFr::Dictionary<NxFr::StringId, ComponentsFactory*>& GetFactories()
	{
		static NxFr::Dictionary<NxFr::StringId, ComponentsFactory*> Factories;
		return Factories;
	}

	ComponentsFactory* ComponentsFactory::Create(NxFr::StringId Type)
	{
		ComponentsFactory* Template = GetFactory(Type);
		return Template->Clone();
	}

	ComponentsFactory* ComponentsFactory::GetFactory(NxFr::StringId Id)
	{
		return GetFactories()[Id];
	}

	void ComponentsFactory::SetFactory(NxFr::StringId Id, ComponentsFactory* Instance)
	{
		GetFactories().AppendOrAssign(Id, Instance);
	}
}
