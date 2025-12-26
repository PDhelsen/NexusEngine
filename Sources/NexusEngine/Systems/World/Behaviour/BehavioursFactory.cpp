#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Behaviour/BehavioursFactory.h"

namespace NxEn
{
	static NxFr::Dictionary<NxFr::StringId, BehavioursFactory::Factory*>& GetFactories()
	{
		static NxFr::Dictionary<NxFr::StringId, BehavioursFactory::Factory*> Factories;
		return Factories;
	}

	BehavioursFactory::Factory* BehavioursFactory::GetFactory(NxFr::StringId Id)
	{
		return GetFactories()[Id];
	}

	void BehavioursFactory::SetFactory(NxFr::StringId Id, Factory* Instance)
	{
		GetFactories().AppendOrAssign(Id, Instance);
	}
}
