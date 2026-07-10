#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/World/Component.h"

namespace NxEn
{
	enum class TransformSpace
	{
		World, Local
	};

	class NX_ENGINE_API Transform : public Component
	{
	public:
		static NxFr::Vector3f TransformPosition(const NxFr::Matrix4x4f& Space, NxFr::Vector3f Position);
		static NxFr::Vector3f TransformVector(const NxFr::Matrix4x4f& Space, NxFr::Vector3f Vector);
		static NxFr::Vector3f TransformDirection(const NxFr::Matrix4x4f& Space, NxFr::Vector3f Direction);
		static NxFr::Quaternion TransformRotation(const NxFr::Matrix4x4f& Space, NxFr::Quaternion Rotation);

		NX_OBJECT(Transform)

		Transform();
		~Transform();

		void Translate(NxFr::Vector3f Delta, TransformSpace Space = TransformSpace::World);
		void Rotate(NxFr::Quaternion Delta, TransformSpace Space = TransformSpace::World);
		void Scale(NxFr::Vector3f Factor);
		void LookAtPosition(NxFr::Vector3f Target, NxFr::Vector3f Up = NxFr::Vector3f::Up);
		void LookAtDirection(NxFr::Vector3f Direction, NxFr::Vector3f Up = NxFr::Vector3f::Up);

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
		void OnSerialize(YAML::Node& Node) const override;
		void OnDeserialize(const YAML::Node& Node) override;
		void OnUpdateHierarchy() override;

	private:
		void UpdateWorldMatrix() const;

		void SetDirty() const;
		void CleanDirty() const;
		bool IsDirty() const;

		NxFr::Handle<Transform> Parent;

		NxFr::Vector3f Position;
		NxFr::Quaternion Rotation;
		NxFr::Vector3f Scaling;

		mutable NxFr::Matrix4x4f World;
		mutable bool Dirty;
	};
}
