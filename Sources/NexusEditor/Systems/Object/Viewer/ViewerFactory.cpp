#include "NexusEditor/Systems/Object/Viewer/ViewerFactory.h"

namespace NxEd
{
	static NxFr::Dictionary<NxFr::StringId, ViewerFactory::Factory*>& GetFactories()
	{
		static NxFr::Dictionary<NxFr::StringId, ViewerFactory::Factory*> Factories;
		return Factories;
	}

	ViewerFactory::Factory* ViewerFactory::GetFactory(NxFr::StringId Id)
	{
		Factory** Instance = GetFactories().TryGet(Id);
		return Instance ? *Instance : nullptr;
	}

	void ViewerFactory::SetFactory(NxFr::StringId Id, Factory* Instance)
	{
		GetFactories().AppendOrAssign(Id, Instance);
	}
}
