#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Components/Components/Transform.h"

namespace NxEn
{
	NEXUS_COMPONENT_IMPLEMENTATION(Transform)

	NxFr::Vector3f Transform::TransformPosition(const NxFr::Matrix4x4f& Space, NxFr::Vector3f Position)
	{
		return Space * NxFr::Vector4f(Position.x, Position.y, Position.z, 1.0f);
	}

	NxFr::Vector3f Transform::TransformVector(const NxFr::Matrix4x4f& Space, NxFr::Vector3f Vector)
	{
		return Space * NxFr::Vector4f(Vector.x, Vector.y, Vector.z, 0.0f);
	}

	NxFr::Vector3f Transform::TransformDirection(const NxFr::Matrix4x4f& Space, NxFr::Vector3f Direction)
	{
		return Space.GetRotation() * NxFr::Vector4f(Direction.x, Direction.y, Direction.z, 0.0f);
	}

	NxFr::Quaternion Transform::TransformRotation(const NxFr::Matrix4x4f& Space, NxFr::Quaternion Rotation)
	{
		return Space.GetRotation() * Rotation;
	}

	Transform::Transform()
		: Position(NxFr::Vector3f::Zero), Rotation(NxFr::Quaternion::Identity), Scaling(NxFr::Vector3f::One)
	{
	}

	Transform::~Transform()
	{
	}

	void Transform::Translate(NxFr::Vector3f Delta, TransformSpace Space)
	{
		switch (Space)
		{
		case NxEn::TransformSpace::World: Position += Delta; return;
		case NxEn::TransformSpace::Local: Position += Rotation * Delta; return;
		}
	}

	void Transform::Rotate(NxFr::Quaternion Delta, TransformSpace Space)
	{
		switch (Space)
		{
		case NxEn::TransformSpace::World: Rotation = Rotation * Delta; return;
		case NxEn::TransformSpace::Local: Rotation = Delta * Rotation; return;
		}
	}

	void Transform::Scale(NxFr::Vector3f Factor, TransformSpace Space)
	{
		Scaling *= Factor;
	}

	void Transform::LookAtPosition(NxFr::Vector3f Target, NxFr::Vector3f Up, TransformSpace Space)
	{
		NxFr::Handle<Transform> Parent = GetParent();
		if (Space == TransformSpace::Local && Parent)
		{
			Target = Parent->LocalToWorld() * NxFr::Vector4f(Target.x, Target.y, Target.z, 1.0f);
		}

		NxFr::Vector3f Direction = NxFr::VectorUtility::Normalize(Target - GetPosition(TransformSpace::World));
		Rotation = NxFr::RotationUtility::LookAt(Direction, Up);
	}

	void Transform::LookAtDirection(NxFr::Vector3f Direction, NxFr::Vector3f Up, TransformSpace Space)
	{
		NxFr::Handle<Transform> Parent = GetParent();
		if (Space == TransformSpace::Local && Parent)
		{
			Direction = Parent->LocalToWorld() * NxFr::Vector4f(Direction.x, Direction.y, Direction.z, 0.0f);
		}

		Direction = NxFr::VectorUtility::Normalize(Direction);
		Rotation = NxFr::RotationUtility::LookAt(Direction, Up);
	}

	NxFr::Vector3f Transform::GetPosition(TransformSpace Space) const
	{
		NxFr::Vector3f Result = Position;

		NxFr::Handle<Transform> Parent = GetParent();
		if (Space == TransformSpace::World && Parent)
		{
			Result = Parent->GetPosition(TransformSpace::World) + Transform::TransformPosition(Parent->LocalToWorld(), Position);
		}

		return Result;
	}

	void Transform::SetPosition(NxFr::Vector3f Position, TransformSpace Space)
	{
		NxFr::Handle<Transform> Parent = GetParent();
		if (Space == TransformSpace::World && Parent)
		{
			Position = Transform::TransformPosition(Parent->WorldToLocal(), Position - Parent->GetPosition(Space));
		}

		this->Position = Position;
	}

	NxFr::Quaternion Transform::GetRotation(TransformSpace Space) const
	{
		NxFr::Quaternion Result = Rotation;

		NxFr::Handle<Transform> Parent = GetParent();
		if (Space == TransformSpace::World && Parent)
		{
			Result = Parent->GetRotation(Space) * Result;
		}

		return Result;
	}

	void Transform::SetRotation(NxFr::Quaternion Rotation, TransformSpace Space)
	{
		NxFr::Handle<Transform> Parent = GetParent();
		if (Space == TransformSpace::World && Parent)
		{
			Rotation = Parent->GetRotation(Space).Inverse() * Rotation;
		}

		this->Rotation = Rotation;
	}

	NxFr::Vector3f Transform::GetScale(TransformSpace Space) const
	{
		NxFr::Vector3f Result = Scaling;

		NxFr::Handle<Transform> Parent = GetParent();
		if (Space == TransformSpace::World && Parent)
		{
			Result *= Parent->GetScale(Space);
		}

		return Result;
	}

	void Transform::SetScale(NxFr::Vector3f Scale, TransformSpace Space)
	{
		NxFr::Handle<Transform> Parent = GetParent();
		if (Space == TransformSpace::World && Parent)
		{
			Scale /= Parent->GetScale(Space);
		}

		this->Scaling = Scale;
	}

	NxFr::Matrix4x4f Transform::GetMatrix() const
	{
		return NxFr::Matrix4x4f::TRS(Position, Rotation, Scaling);
	}

	void Transform::SetMatrix(NxFr::Matrix4x4f Matrix)
	{
		Position = Matrix.GetPosition();
		Rotation = Matrix.GetRotation();
		Scaling = Matrix.GetScale();
	}

	NxFr::Vector3f Transform::Right(TransformSpace Space) const
	{
		return GetRotation(Space) * NxFr::Vector3f::Right;
	}

	NxFr::Vector3f Transform::Up(TransformSpace Space) const
	{
		return GetRotation(Space) * NxFr::Vector3f::Up;
	}

	NxFr::Vector3f Transform::Forward(TransformSpace Space) const
	{
		return GetRotation(Space) * NxFr::Vector3f::Forward;
	}

	NxFr::Matrix4x4f Transform::LocalToWorld() const
	{
		NxFr::Matrix4x4f Result = GetMatrix();

		NxFr::Handle<Transform> Parent = GetParent();
		if (Parent)
		{
			return Parent->LocalToWorld() * Result;
		}

		return Result;
	}

	NxFr::Matrix4x4f Transform::WorldToLocal() const
	{
		NxFr::Matrix4x4f Result = GetMatrix().Inverse();

		NxFr::Handle<Transform> Parent = GetParent();
		if (Parent)
		{
			return Result * Parent->WorldToLocal();
		}

		return Result;
	}

	void Transform::OnGui(float TimeStep)
	{
		Component::OnGui(TimeStep);

		GUI::Drawer<NxFr::Vector3f>::Field(Position, "Position");
		GUI::Drawer<NxFr::Quaternion>::Field(Rotation, "Rotation");
		GUI::Drawer<NxFr::Vector3f>::Field(Scaling, "Scale");
	}

	void Transform::OnClone(const Object& Other)
	{
		Component::OnClone(Other);
		const Transform& Instance = static_cast<const Transform&>(Other);

		Position = Instance.Position;
		Rotation = Instance.Rotation;
		Scaling = Instance.Scaling;
	}

	void Transform::OnSave(YAML::Node& Node)
	{
		Component::OnSave(Node);

		Node["Position"] = Position;
		Node["Rotation"] = Rotation;
		Node["Scaling"] = Scaling;
	}

	void Transform::OnLoad(const YAML::Node& Node)
	{
		Component::OnLoad(Node);

		Position = Node["Position"].as<NxFr::Vector3f>();
		Rotation = Node["Rotation"].as<NxFr::Quaternion>();
		Scaling = Node["Scaling"].as<NxFr::Vector3f>();
	}

	NxFr::Handle<Transform> Transform::GetParent() const
	{
		return GetGameObject()->GetParent()->GetComponent<Transform>();
	}
}
