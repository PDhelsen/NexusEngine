#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Components/Components/Transform.h"

namespace NxEn
{
	NX_COMPONENT_IMPLEMENTATION(Transform)

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
		: Parent(), Dirty(0),
		Position(NxFr::Vector3f::Zero), Rotation(NxFr::Quaternion::Identity), Scaling(NxFr::Vector3f::One),
		WorldPosition(), WorldRotation(), WorldScaling()
	{
		SetDirty(DirtyFlag::All, false);
	}

	Transform::~Transform()
	{
	}

	void Transform::Translate(NxFr::Vector3f Delta, TransformSpace Space)
	{
		switch (Space)
		{
		case NxEn::TransformSpace::World: Position += Delta; break;
		case NxEn::TransformSpace::Local: Position += Rotation * Delta; break;
		}

		SetDirty(DirtyFlag::Position);
	}

	void Transform::Rotate(NxFr::Quaternion Delta, TransformSpace Space)
	{
		switch (Space)
		{
		case NxEn::TransformSpace::World: Rotation = Rotation * Delta; break;
		case NxEn::TransformSpace::Local: Rotation = Delta * Rotation; break;
		}

		SetDirty(DirtyFlag::Rotation);

	}

	void Transform::Scale(NxFr::Vector3f Factor, TransformSpace Space)
	{
		Scaling *= Factor;

		SetDirty(DirtyFlag::Scaling);
	}

	void Transform::LookAtPosition(NxFr::Vector3f Target, NxFr::Vector3f Up, TransformSpace Space)
	{
		if (Space == TransformSpace::Local && Parent)
		{
			Target = Transform::TransformPosition(Parent->LocalToWorld(), Target);
		}

		NxFr::Vector3f Direction = NxFr::VectorUtility::Normalize(Target - GetPosition(TransformSpace::World));
		Rotation = NxFr::RotationUtility::LookAt(Direction, Up);

		SetDirty(DirtyFlag::Rotation);
	}

	void Transform::LookAtDirection(NxFr::Vector3f Direction, NxFr::Vector3f Up, TransformSpace Space)
	{
		if (Space == TransformSpace::Local && Parent)
		{
			Direction = Transform::TransformDirection(Parent->LocalToWorld(), Direction);
		}

		Direction = NxFr::VectorUtility::Normalize(Direction);
		Rotation = NxFr::RotationUtility::LookAt(Direction, Up);

		SetDirty(DirtyFlag::Rotation);
	}

	NxFr::Vector3f Transform::GetPosition(TransformSpace Space) const
	{
		NxFr::Vector3f Result = Position;

		if (Space == TransformSpace::World && Parent)
		{
			if (IsDirty(DirtyFlag::Position))
			{
				WorldPosition = Transform::TransformPosition(Parent->LocalToWorld(), Position);
				CleanDirty(DirtyFlag::Position);
			}

			Result = WorldPosition;
		}

		return Result;
	}

	void Transform::SetPosition(NxFr::Vector3f Position, TransformSpace Space)
	{
		if (Space == TransformSpace::World && Parent)
		{
			Position = Transform::TransformPosition(Parent->WorldToLocal(), Position);
		}

		this->Position = Position;

		SetDirty(DirtyFlag::Position);
	}

	NxFr::Quaternion Transform::GetRotation(TransformSpace Space) const
	{
		NxFr::Quaternion Result = Rotation;

		if (Space == TransformSpace::World && Parent)
		{
			if (IsDirty(DirtyFlag::Rotation))
			{
				WorldRotation = Parent->GetRotation(Space) * Result;
				CleanDirty(DirtyFlag::Rotation);
			}

			Result = WorldRotation;
		}

		return Result;
	}

	void Transform::SetRotation(NxFr::Quaternion Rotation, TransformSpace Space)
	{
		if (Space == TransformSpace::World && Parent)
		{
			Rotation = Parent->GetRotation(Space).Inverse() * Rotation;
		}

		this->Rotation = Rotation;

		SetDirty(DirtyFlag::Rotation);
	}

	NxFr::Vector3f Transform::GetScale(TransformSpace Space) const
	{
		NxFr::Vector3f Result = Scaling;

		if (Space == TransformSpace::World && Parent)
		{
			if (IsDirty(DirtyFlag::Scaling))
			{
				WorldScaling = Parent->GetScale(Space) * Result;
				CleanDirty(DirtyFlag::Scaling);
			}

			Result = WorldScaling;
		}

		return Result;
	}

	void Transform::SetScale(NxFr::Vector3f Scale, TransformSpace Space)
	{
		if (Space == TransformSpace::World && Parent)
		{
			Scale /= Parent->GetScale(Space);
		}

		this->Scaling = Scale;

		SetDirty(DirtyFlag::Scaling);
	}

	NxFr::Matrix4x4f Transform::GetMatrix(TransformSpace Space) const
	{
		return NxFr::Matrix4x4f::TRS(GetPosition(Space), GetRotation(Space), GetScale(Space));
	}

	void Transform::SetMatrix(NxFr::Matrix4x4f Matrix, TransformSpace Space)
	{
		SetPosition(Matrix.GetPosition(), Space);
		SetRotation(Matrix.GetRotation(), Space);
		SetScale(Matrix.GetScale(), Space);
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
		return GetMatrix(TransformSpace::World);
	}

	NxFr::Matrix4x4f Transform::WorldToLocal() const
	{
		return GetMatrix(TransformSpace::World).Inverse();
	}

	void Transform::OnGui(float TimeStep)
	{
		Component::OnGui(TimeStep);

		if (GUI::Drawer<NxFr::Vector3f>::Field(Position, "Position"))
		{
			SetDirty(DirtyFlag::Position);
		}

		if (GUI::Drawer<NxFr::Quaternion>::Field(Rotation, "Rotation"))
		{
			SetDirty(DirtyFlag::Rotation);
		}

		if (GUI::Drawer<NxFr::Vector3f>::Field(Scaling, "Scale"))
		{
			SetDirty(DirtyFlag::Scaling);
		}
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

	void Transform::OnUpdateHierarchy()
	{
		Parent = NxFr::Handle<Transform>();

		NxFr::Handle<GameObject> Target = GetGameObject()->GetParent();
		while (Target)
		{
			Parent = Target->GetComponent<Transform>();
			if (Parent)
			{
				break;
			}

			Target = Target->GetParent();
		}

		SetDirty(DirtyFlag::All, false);
	}

	void Transform::SetDirty(DirtyFlag Flag, bool Recursive) const
	{
		Dirty = NxFr::Integer::SetFlag(Dirty, (uint8)Flag, true);

		if (!Recursive)
		{
			return;
		}

		NxFr::Handle<GameObject> Child = GetGameObject()->GetChild();
		while (Child)
		{
			NxFr::Handle<Transform> ChildTransform = Child->GetComponent<Transform>();
			if (ChildTransform)
			{
				ChildTransform->SetDirty(Flag, Recursive);
			}
			Child = Child->GetNext();
		}
	}

	void Transform::CleanDirty(DirtyFlag Flag) const
	{
		Dirty = NxFr::Integer::SetFlag(Dirty, (uint8)Flag, false);
	}

	bool Transform::IsDirty(DirtyFlag Flag) const
	{
		return NxFr::Integer::CheckFlag(Dirty, (uint8)Flag);
	}
}
