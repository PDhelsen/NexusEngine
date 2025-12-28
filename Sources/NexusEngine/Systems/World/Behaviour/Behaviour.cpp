#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Behaviour/Behaviour.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(Behaviour)

	Behaviour::Behaviour()
		: BehaviourId(0), Target()
	{
		SetTickable(true);
	}

	Behaviour::~Behaviour()
	{
	}

	void Behaviour::Start()
	{
		if (!Target.IsValid() || !Target->IsTickable() || !Target->IsEnabled())
		{
			return;
		}

		if (!IsTickable() || !IsEnabled())
		{
			return;
		}

		OnStart();
	}

	void Behaviour::Tick(float TimeStep)
	{
		if (!Target.IsValid() || !Target->IsTickable() || !Target->IsEnabled())
		{
			return;
		}

		if (!IsTickable() || !IsEnabled())
		{
			return;
		}

		OnTick(TimeStep);
	}

	void Behaviour::DrawGui(float TimeStep)
	{
		OnGui(TimeStep);
	}

	void Behaviour::OnGui(float TimeStep)
	{
		GUI::Drawer<NxFr::StringId>::Property(GetObjectType(), "Type");
		GUI::Drawer<NxFr::GUID>::Property(BehaviourId, "Id");

		bool Enabled = IsEnabled();
		GUI::Drawer<bool>::Field(Enabled, "Enabled", "");
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

	YAML::Node Behaviour::Save()
	{
		YAML::Node Node;
		Node["Type"] = GetObjectType();
		Node["Id"] = BehaviourId;
		Node["Target"] = Target->GetId();
		Node["Enabled"] = GetFlag(ObjectFlags::Enabled);

		YAML::Node Data;
		OnSave(Data);
		Node["Data"] = Data;

		return Node;
	}

	void Behaviour::Load(const YAML::Node& Node)
	{
		NEXUS_ASSERT(BehaviourId == Node["Id"].as<NxFr::GUID>(), Default, "Runtime and Serialized id should match");
		NEXUS_ASSERT(Target->GetId() == Node["Target"].as<NxFr::GUID>(), Default, "Runtime and Serialized id should match");
		SetFlag(ObjectFlags::Enabled, Node["Enabled"].as<bool>());

		OnLoad(Node["Data"]);
	}

	void Behaviour::Unload()
	{
		OnUnload();
	}
}
