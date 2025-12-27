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

	bool Object::operator==(const Object& Other) const
	{
		return GetId() == Other.GetId();
	}

	bool Object::operator!=(const Object& Other) const
	{
		return !(*this == Other);
	}

	void Object::Initialize()
	{
		if (IsInitialized())
		{
			NEXUS_LOG(Warning, Default, "Object (%s) is already initialized", GetName().C());
			return;
		}

		OnInitialize();
		SetFlag(ObjectFlags::Initialized, true);
	}

	void Object::Shutdown()
	{
		if (!IsInitialized())
		{
			NEXUS_LOG(Warning, Default, "Object (%s) is already shutdown", GetName().C());
			return;
		}

		OnShutdown();
		SetFlag(ObjectFlags::Initialized, false);
	}

	void Object::Start()
	{
		if (!IsInitialized() || !IsEnabled())
		{
			return;
		}

		OnStart();
	}

	void Object::Tick(float TimeStep)
	{
		if (!IsTickable() || !IsEnabled())
		{
			return;
		}

		OnTick(TimeStep);
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
		if (IsEnabled() == Enabled)
		{
			return;
		}

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

	Object* Object::Clone() const
	{
		NEXUS_ASSERT(false, Default, "Not Implemented");
		return nullptr;
	}

	void Object::Clone(Object* Target)
	{
		NEXUS_ASSERT(false, Default, "Not Implemented");
	}

	void Object::Clone(const Object* Target)
	{
		NEXUS_ASSERT(false, Default, "Not Implemented");
	}

	NxFr::StringView Object::GetName() const
	{
		return GetObjectType().GetString();
	}

	NxFr::GUID Object::GetId() const
	{
		return reinterpret_cast<NxFr::GUID>(this);
	}

	bool Object::GetFlag(ObjectFlags Flag) const
	{
		return NxFr::Enum::CheckFlag(Flags, Flag);
	}

	void Object::SetFlag(ObjectFlags Flag, bool Value)
	{
		Flags = NxFr::Enum::SetFlag(Flags, Flag, Value);
	}
}
