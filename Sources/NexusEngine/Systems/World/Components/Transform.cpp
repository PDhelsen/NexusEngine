#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Components/Transform.h"

namespace NxEn
{
	NX_WORLD_OBJECT(Transform)

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
		: Parent(),
		Position(NxFr::Vector3f::Zero), Rotation(NxFr::Quaternion::Identity), Scaling(NxFr::Vector3f::One),
		World(), Dirty(false)
	{
		
	}

	Transform::~Transform()
	{
	}

	void Transform::Translate(NxFr::Vector3f Delta, TransformSpace Space)
	{
		NxFr::Vector3f Current = GetPosition(Space);
		Current += Delta;
		SetPosition(Current, Space);
	}

	void Transform::Rotate(NxFr::Quaternion Delta, TransformSpace Space)
	{
		NxFr::Quaternion Current = GetRotation(Space);
		Current = Space == TransformSpace::World ? Delta * Current : Current * Delta;
		SetRotation(Current, Space);
	}

	void Transform::Scale(NxFr::Vector3f Factor)
	{
		NxFr::Vector3f Current = GetScale(TransformSpace::Local);
		Current *= Factor;
		SetScale(Current, TransformSpace::Local);
	}

	void Transform::LookAtPosition(NxFr::Vector3f Target, NxFr::Vector3f Up)
	{
		NxFr::Vector3f Direction = NxFr::VectorUtility::Normalize(Target - GetPosition(TransformSpace::World));
		NxFr::Quaternion Orientation = NxFr::RotationUtility::LookAt(Direction, Up);

		SetRotation(Orientation, TransformSpace::World);
	}

	void Transform::LookAtDirection(NxFr::Vector3f Direction, NxFr::Vector3f Up)
	{
		Direction = NxFr::VectorUtility::Normalize(Direction);
		NxFr::Quaternion Orientation = NxFr::RotationUtility::LookAt(Direction, Up);

		SetRotation(Orientation, TransformSpace::World);
	}

	NxFr::Vector3f Transform::GetPosition(TransformSpace Space) const
	{
		if (Space == TransformSpace::World)
		{
			UpdateWorldMatrix();
			return World.GetPosition();
		}
		else
		{
			return Position;
		}
	}

	void Transform::SetPosition(NxFr::Vector3f Position, TransformSpace Space)
	{
		this->Position = Space == TransformSpace::World && Parent ?
			Transform::TransformPosition(Parent->WorldToLocal(), Position) :
			Position;

		SetDirty();
	}

	NxFr::Quaternion Transform::GetRotation(TransformSpace Space) const
	{
		if (Space == TransformSpace::World)
		{
			UpdateWorldMatrix();
			return World.GetRotation();
		}
		else
		{
			return Rotation;
		}
	}

	void Transform::SetRotation(NxFr::Quaternion Rotation, TransformSpace Space)
	{
		this->Rotation = Space == TransformSpace::World && Parent ?
			Parent->GetRotation(TransformSpace::World).Inverse() * Rotation :
			Rotation;

		SetDirty();
	}

	NxFr::Vector3f Transform::GetScale(TransformSpace Space) const
	{
		if (Space == TransformSpace::World)
		{
			UpdateWorldMatrix();
			return World.GetScale();
		}
		else
		{
			return Scaling;
		}
	}

	void Transform::SetScale(NxFr::Vector3f Scale, TransformSpace Space)
	{
		this->Scaling = Space == TransformSpace::World && Parent ?
			Scale / Parent->GetScale(Space) :
			Scale;

		SetDirty();
	}

	NxFr::Matrix4x4f Transform::GetMatrix(TransformSpace Space) const
	{
		if (Space == TransformSpace::World)
		{
			UpdateWorldMatrix();
			return World;
		}
		else
		{
			return NxFr::Matrix4x4f::TRS(Position, Rotation, Scaling);
		}
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
		UpdateWorldMatrix();
		return World;
	}

	NxFr::Matrix4x4f Transform::WorldToLocal() const
	{
		UpdateWorldMatrix();
		return World.Inverse();
	}

	void Transform::OnDraw()
	{
		Component::OnDraw();

		if (GUI::Drawer<NxFr::Vector3f>::Field(Position, "Position"))
		{
			SetDirty();
		}

		if (GUI::Drawer<NxFr::Quaternion>::Field(Rotation, "Rotation"))
		{
			SetDirty();
		}

		if (GUI::Drawer<NxFr::Vector3f>::Field(Scaling, "Scale"))
		{
			SetDirty();
		}
	}

	void Transform::OnClone(const Object& Other)
	{
		Component::OnClone(Other);
		const Transform& Instance = static_cast<const Transform&>(Other);

		Position = Instance.Position;
		Rotation = Instance.Rotation;
		Scaling = Instance.Scaling;

		SetDirty();
	}

	void Transform::OnSerialize(NxFr::Yaml::Node& Node) const
	{
		Component::OnSerialize(Node);

		Node["Position"] = Position;
		Node["Rotation"] = Rotation;
		Node["Scaling"] = Scaling;
	}

	void Transform::OnDeserialize(const NxFr::Yaml::Node& Node)
	{
		Component::OnDeserialize(Node);

		Position = Node["Position"].As<NxFr::Vector3f>();
		Rotation = Node["Rotation"].As<NxFr::Quaternion>();
		Scaling = Node["Scaling"].As<NxFr::Vector3f>();

		SetDirty();
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

		SetDirty();
	}

	void Transform::UpdateWorldMatrix() const
	{
		if (!IsDirty())
		{
			return;
		}

		NxFr::Matrix4x4f Local = GetMatrix(TransformSpace::Local);

		if (Parent)
		{
			World = Parent->GetMatrix(TransformSpace::World) * Local;
		}
		else
		{
			World = Local;
		}

		CleanDirty();
	}

	void Transform::SetDirty() const
	{
		if (IsDirty())
		{
			return;
		}

		for (auto It = GetGameObject()->BeginChild(); It != GetGameObject()->EndChild(); ++It)
		{
			NxFr::Handle<Transform> Instance = It->GetComponent<Transform>();
			if (Instance)
			{
				Instance->Dirty = true;
			}
		}
	}

	void Transform::CleanDirty() const
	{
		Dirty = false;
	}

	bool Transform::IsDirty() const
	{
		return Dirty;
	}
}
