#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/AssetsFactory.h"

namespace NxEn
{
	static NxFr::Dictionary<NxFr::StringId, AssetsFactory::Factory*>& GetFactories()
	{
		static NxFr::Dictionary<NxFr::StringId, AssetsFactory::Factory*> Factories;
		return Factories;
	}

	AssetsFactory::Factory* AssetsFactory::GetFactory(NxFr::StringId Id)
	{
		return GetFactories()[Id];
	}

	void AssetsFactory::SetFactory(NxFr::StringId Id, Factory* Instance)
	{
		GetFactories().AppendOrAssign(Id, Instance);
	}
}
