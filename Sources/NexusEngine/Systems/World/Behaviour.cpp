#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Behaviour.h"

namespace NxEn
{
	Behaviour::Behaviour()
		: BehaviourId(0), Target()
	{
		SetTickable(true);
	}

	Behaviour::~Behaviour()
	{
	}

	bool Behaviour::IsTicking() const
	{
		return Object::IsTicking() && GetGameObject()->IsTicking();
	}

	NxFr::StringView Behaviour::GetName() const
	{
		return Target->GetName();
	}

	NxFr::GUID Behaviour::GetId() const
	{
		return BehaviourId;
	}

	NxFr::Handle<GameObject> Behaviour::GetGameObject() const
	{
		return Target;
	}

	void Behaviour::OnUpdateHierarchy()
	{
		bool Enabled = IsEnabled() && GetGameObject()->IsEnabledInHierarchy();
		if (Enabled == IsEnabledInHierarchy())
		{
			return;
		}

		SetFlag(ObjectFlags::EnabledInHierarchy, Enabled);

		if (Enabled)
		{
			OnEnable();
		}
		else
		{
			OnDisable();
		}
	}

	void Behaviour::OnDraw()
	{
		GUI::Drawer<NxFr::StringId>::Property(GetObjectType(), "Type");
		GUI::Drawer<NxFr::GUID>::Property(BehaviourId, "Id");

		bool Enabled = IsEnabled();
		GUI::Drawer<bool>::Field(Enabled, "Enabled");
		if (Enabled != IsEnabled())
		{
			SetEnabled(Enabled);
		}
	}

	void Behaviour::OnClone(const Object& Other)
	{
		const Behaviour& Instance = static_cast<const Behaviour&>(Other);

		SetFlag(ObjectFlags::Enabled, Instance.IsEnabled());
		SetFlag(ObjectFlags::Tickable, Instance.IsTickable());
	}

	void Behaviour::OnSerialize(YAML::Node& Node) const
	{
		Node["Type"] = GetObjectType();
		Node["Id"] = BehaviourId;
		Node["Target"] = Target->GetId();
		Node["Enabled"] = GetFlag(ObjectFlags::Enabled);
		Node["Tickable"] = GetFlag(ObjectFlags::Tickable);
	}

	void Behaviour::OnDeserialize(const YAML::Node& Node)
	{
		NX_ASSERT(BehaviourId == Node["Id"].as<NxFr::GUID>(), Default, "Runtime and Serialized id should match");
		NX_ASSERT(Target->GetId() == Node["Target"].as<NxFr::GUID>(), Default, "Runtime and Serialized id should match");
		SetFlag(ObjectFlags::Enabled, Node["Enabled"].as<bool>());
		SetFlag(ObjectFlags::Tickable, Node["Tickable"].as<bool>());
	}
}
