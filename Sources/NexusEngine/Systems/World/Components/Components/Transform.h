#pragma once

#include "NexusEngine/Systems/World/Components/Component.h"

namespace NxEn
{
	enum class TransformSpace
	{
		World, Local
	};

	class NX_ENGINE_API Transform : public Component
	{
		enum class DirtyFlag : uint8
		{
			None = 0,
			Position = 1 << 0,
			Rotation = 1 << 1,
			Scaling = 1 << 2,
			All = (1 << 3) - 1
		};

	public:
		static NxFr::Vector3f TransformPosition(const NxFr::Matrix4x4f& Space, NxFr::Vector3f Position);
		static NxFr::Vector3f TransformVector(const NxFr::Matrix4x4f& Space, NxFr::Vector3f Vector);
		static NxFr::Vector3f TransformDirection(const NxFr::Matrix4x4f& Space, NxFr::Vector3f Direction);
		static NxFr::Quaternion TransformRotation(const NxFr::Matrix4x4f& Space, NxFr::Quaternion Rotation);

		NX_COMPONENT_DECLARATION(Transform)

		Transform();
		~Transform();

		void Translate(NxFr::Vector3f Delta, TransformSpace Space = TransformSpace::World);
		void Rotate(NxFr::Quaternion Delta, TransformSpace Space = TransformSpace::World);
		void Scale(NxFr::Vector3f Factor, TransformSpace Space = TransformSpace::World);
		void LookAtPosition(NxFr::Vector3f Target, NxFr::Vector3f Up = NxFr::Vector3f::Up, TransformSpace Space = TransformSpace::World);
		void LookAtDirection(NxFr::Vector3f Direction, NxFr::Vector3f Up = NxFr::Vector3f::Up, TransformSpace Space = TransformSpace::World);

		NxFr::Vector3f GetPosition(TransformSpace Space = TransformSpace::World) const;
		void SetPosition(NxFr::Vector3f Position, TransformSpace Space = TransformSpace::World);
		NxFr::Quaternion GetRotation(TransformSpace Space = TransformSpace::World) const;
		void SetRotation(NxFr::Quaternion Rotation, TransformSpace Space = TransformSpace::World);
		NxFr::Vector3f GetScale(TransformSpace Space = TransformSpace::World) const;
		void SetScale(NxFr::Vector3f Scale, TransformSpace Space = TransformSpace::World);
		NxFr::Matrix4x4f GetMatrix(TransformSpace Space = TransformSpace::World) const;
		void SetMatrix(NxFr::Matrix4x4f Matrix, TransformSpace Space = TransformSpace::World);

		NxFr::Vector3f Right(TransformSpace Space = TransformSpace::World) const;
		NxFr::Vector3f Up(TransformSpace Space = TransformSpace::World) const;
		NxFr::Vector3f Forward(TransformSpace Space = TransformSpace::World) const;

		NxFr::Matrix4x4f LocalToWorld() const;
		NxFr::Matrix4x4f WorldToLocal() const;

	protected:
		void OnDraw() override;
		void OnClone(const Object& Other) override;
		void OnSave(YAML::Node& Node) override;
		void OnLoad(const YAML::Node& Node) override;
		void OnUpdateHierarchy() override;

	private:
		void SetDirty(DirtyFlag Flag, bool Recursive = true) const;
		void CleanDirty(DirtyFlag Flag) const;
		bool IsDirty(DirtyFlag Flag) const;

	private:
		NxFr::Handle<Transform> Parent;
		mutable uint8 Dirty;

		NxFr::Vector3f Position;
		NxFr::Quaternion Rotation;
		NxFr::Vector3f Scaling;

		mutable NxFr::Vector3f WorldPosition;
		mutable NxFr::Quaternion WorldRotation;
		mutable NxFr::Vector3f WorldScaling;
	};
}

