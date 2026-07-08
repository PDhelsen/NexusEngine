#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Factory/WorldObjectReferences.h"

namespace NxEn
{
	NxFr::Context<WorldObjectReferences>& WorldObjectReferences::GetContexts()
	{
		static NxFr::Context<WorldObjectReferences> Contexts;
		return Contexts;
	}

	NxFr::GUID WorldObjectReferences::Resolve(NxFr::GUID Id)
	{
		WorldObjectReferences* Map = GetContexts().TryGet();
		return Map && Id != 0 ? Map->Ids[Id] : Id;
	}
}
