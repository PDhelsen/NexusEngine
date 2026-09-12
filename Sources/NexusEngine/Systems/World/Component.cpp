#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Component.h"

namespace NxEn
{
	Component::Component()
		: ComponentId(0), Target()
	{
		SetTickable(false);
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

	void Component::OnSerialize(NxFr::Yaml::Node& Node) const
	{
		Node["Type"] = GetObjectType();
		Node["Id"] = ComponentId;
		Node["Target"] = Target->GetId();
		Node["Enabled"] = GetFlag(ObjectFlags::Enabled);
	}

	void Component::OnDeserialize(const NxFr::Yaml::Node& Node)
	{
		SetFlag(ObjectFlags::Enabled, Node["Enabled"].As<bool>());
	}
}
