#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Object.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(Object)

	Object::Object()
		: Enabled(true)
	{
	}

	Object::~Object()
	{
	}

	bool Object::IsEnabled() const
	{
		return Enabled;
	}

	void Object::SetEnabled(bool enabled)
	{
		Enabled = enabled;
		if (Enabled)
		{
			OnEnable();
		}
		else
		{
			OnDisable();
		}
	}

	NxFr::String Object::ToString() const
	{
		return GetObjectType().ToString();
	}

	NxFr::StringView Object::GetName() const
	{
		return GetObjectType().C();
	}

	void Object::Initialize()
	{
		OnInitialize();
		OnEnable();
	}

	void Object::Shutdown()
	{
		OnDisable();
		OnShutdown();
	}

	void Object::Tick()
	{
		OnTick();
	}
}
