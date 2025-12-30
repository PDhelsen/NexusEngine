#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Component/Component.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(Component)

	Component::Component()
		: ComponentId(0), Target()
	{
	}

	Component::~Component()
	{
	}

	void Component::OnGui(float TimeStep)
	{
		bool Enabled = IsEnabled();
		GUI::Drawer<bool>::Field(Enabled);
		if (Enabled != IsEnabled())
		{
			SetEnabled(Enabled);
		}

		ImGui::SameLine();

		GUI::Drawer<NxFr::StringId>::Property(GetObjectType(), "Type");

		GUI::Drawer<NxFr::GUID>::Property(ComponentId, "Id");
	}

	void Component::OnClone(const Object& Other)
	{
		const Component& Instance = static_cast<const Component&>(Other);

		SetFlag(ObjectFlags::Enabled, Instance.IsEnabled());
		SetFlag(ObjectFlags::Tickable, Instance.IsTickable());
	}

	void Component::OnSave(YAML::Node& Node)
	{
		Node["Type"] = GetObjectType();
		Node["Id"] = ComponentId;
		Node["Target"] = Target->GetId();
		Node["Enabled"] = GetFlag(ObjectFlags::Enabled);
	}

	void Component::OnLoad(const YAML::Node& Node)
	{
		NEXUS_ASSERT(ComponentId == Node["Id"].as<NxFr::GUID>(), Default, "Runtime and Serialized id should match");
		NEXUS_ASSERT(Target->GetId() == Node["Target"].as<NxFr::GUID>(), Default, "Runtime and Serialized id should match");
		SetFlag(ObjectFlags::Enabled, Node["Enabled"].as<bool>());
	}

	void Component::OnUnload()
	{
	}
}
