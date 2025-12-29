#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Component/Components/Transform.h"

namespace NxEn
{
	NEXUS_COMPONENT_IMPLEMENTATION(Transform)

	Transform::Transform()
		: Position(), Rotation(), Scale()
	{
	}

	Transform::~Transform()
	{
	}

	void Transform::OnGui(float TimeStep)
	{
		Component::OnGui(TimeStep);

		GUI::Drawer<NxFr::Vector3f>::Field(Position, "Position");
		GUI::Drawer<NxFr::Quaternion>::Field(Rotation, "Rotation");
		GUI::Drawer<NxFr::Vector3f>::Field(Scale, "Scale");
	}

	void Transform::OnClone(const Object& Other)
	{
		Component::OnClone(Other);
		const Transform& Instance = static_cast<const Transform&>(Other);

		Position = Instance.Position;
		Rotation = Instance.Rotation;
		Scale = Instance.Scale;
	}

	void Transform::OnSave(YAML::Node& Node)
	{
		Node["Position"] = Position;
		Node["Rotation"] = Rotation;
		Node["Scale"] = Scale;
	}

	void Transform::OnLoad(const YAML::Node& Node)
	{
		Position = Node["Position"].as<NxFr::Vector3f>();
		Rotation = Node["Rotation"].as<NxFr::Quaternion>();
		Scale = Node["Scale"].as<NxFr::Vector3f>();
	}

	void Transform::OnUnload()
	{
	}
}
