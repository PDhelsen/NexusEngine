#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Object.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(Object)

	Object::Object()
		: Flags(ObjectFlags::None)
	{
	}

	Object::~Object()
	{
	}

	void Object::Initialize()
	{
		if (IsInitialized())
		{
			NEXUS_LOG(Warning, Default, "Object (%s) is already initialized", GetName().C());
			return;
		}

		OnInitialize();
		OnEnable();

		SetFlag(ObjectFlags::Initialized, true);
	}

	void Object::Shutdown()
	{
		if (!IsInitialized())
		{
			NEXUS_LOG(Warning, Default, "Object (%s) is already shutdown", GetName().C());
			return;
		}

		OnDisable();
		OnShutdown();

		SetFlag(ObjectFlags::Initialized, false);
	}

	void Object::Tick()
	{
		if (!IsTickable())
		{
			return;
		}

		OnTick();
	}

	bool Object::IsInitialized() const
	{
		return GetFlag(ObjectFlags::Initialized);
	}

	bool Object::IsEnabled() const
	{
		return GetFlag(ObjectFlags::Enabled);
	}

	void Object::SetEnabled(bool Enabled)
	{
		SetFlag(ObjectFlags::Enabled, Enabled);

		if (IsEnabled())
		{
			OnEnable();
		}
		else
		{
			OnDisable();
		}
	}

	bool Object::IsTickable() const
	{
		return GetFlag(ObjectFlags::Tickable);
	}

	void Object::SetTickable(bool Tickable)
	{
		SetFlag(ObjectFlags::Tickable, Tickable);
	}

	NxFr::String Object::ToString() const
	{
		return GetObjectType().ToString();
	}

	NxFr::StringView Object::GetName() const
	{
		return GetObjectType().C();
	}

	bool Object::GetFlag(ObjectFlags Flag) const
	{
		return Enum::CheckFlag(Flags, Flag);
	}

	void Object::SetFlag(ObjectFlags Flag, bool Value)
	{
		Flags = Enum::SetFlag(Flags, Flag, Value);
	}
}
