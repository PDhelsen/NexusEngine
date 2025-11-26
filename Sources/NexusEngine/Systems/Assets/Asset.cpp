#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(Asset)

	Asset::Asset()
	{
		SetTickable(false);
	}

	Asset::~Asset()
	{
	}

	void Asset::OnInitialize()
	{
	}

	void Asset::OnShutdown()
	{
	}

	void Asset::OnEnable()
	{
	}

	void Asset::OnDisable()
	{
	}

	void Asset::OnTick(float TimeStep)
	{
	}
}
