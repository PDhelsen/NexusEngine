#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Factory/WorldObjectReferences.h"

namespace NxEn
{
	static NxFr::Stack<WorldObjectReferences*> References;

	WorldObjectReferences* WorldObjectReferences::GetReferences()
	{
		return References.GetCount() ? References.Get() : nullptr;
	}

	NxFr::GUID WorldObjectReferences::Resolve(NxFr::GUID Id)
	{
		WorldObjectReferences* Map = GetReferences();
		return Map && Id != 0 ? Map->Ids[Id] : Id;
	}

	WorldObjectReferences::WorldObjectReferences()
	{
		References.Append(this);
	}

	WorldObjectReferences::~WorldObjectReferences()
	{
		References.Remove();
	}
}
