#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Components/Component.h"

namespace NxEn
{
	Component::Component()
		: ComponentId(0), Target()
	{
	}

	Component::~Component()
	{
	}

	bool Component::IsTicking() const
	{
		return false;
	}

	NxFr::StringView Component::GetName() const
	{
		return Target->GetName();
	}

	NxFr::GUID Component::GetId() const
	{
		return ComponentId;
	}

	NxFr::Handle<GameObject> Component::GetGameObject() const
	{
		return Target;
	}

	void Component::OnUpdateHierarchy()
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

	void Component::OnDraw()
	{
		GUI::Drawer<NxFr::StringId>::Property(GetObjectType(), "Type");
		GUI::Drawer<NxFr::GUID>::Property(ComponentId, "Id");

		bool Enabled = IsEnabled();
		GUI::Drawer<bool>::Field(Enabled, "Enabled");
		if (Enabled != IsEnabled())
		{
			SetEnabled(Enabled);
		}
	}

	void Component::OnClone(const Object& Other)
	{
		const Component& Instance = static_cast<const Component&>(Other);

		SetFlag(ObjectFlags::Enabled, Instance.IsEnabled());
		SetFlag(ObjectFlags::Tickable, Instance.IsTickable());
	}

	void Component::OnSerialize(YAML::Node& Node)
	{
		Node["Type"] = GetObjectType();
		Node["Id"] = ComponentId;
		Node["Target"] = Target->GetId();
		Node["Enabled"] = GetFlag(ObjectFlags::Enabled);
	}

	void Component::OnDeserialize(const YAML::Node& Node)
	{
		NX_ASSERT(ComponentId == Node["Id"].as<NxFr::GUID>(), Default, "Runtime and Serialized id should match");
		NX_ASSERT(Target->GetId() == Node["Target"].as<NxFr::GUID>(), Default, "Runtime and Serialized id should match");
		SetFlag(ObjectFlags::Enabled, Node["Enabled"].as<bool>());
	}

	void Component::OnPatchReferences()
	{
	}
}
