#pragma once

#include "NexusEngine/Systems/World/Components/Component.h"

namespace NxEn
{
	enum class TransformSpace
	{
		World, Local
	};

	class Transform : public Component
	{
	public:
		NEXUS_ENGINE_API static NxFr::Vector3f TransformPosition(const NxFr::Matrix4x4f& Space, NxFr::Vector3f Position);
		NEXUS_ENGINE_API static NxFr::Vector3f TransformVector(const NxFr::Matrix4x4f& Space, NxFr::Vector3f Vector);
		NEXUS_ENGINE_API static NxFr::Vector3f TransformDirection(const NxFr::Matrix4x4f& Space, NxFr::Vector3f Direction);
		NEXUS_ENGINE_API static NxFr::Quaternion TransformRotation(const NxFr::Matrix4x4f& Space, NxFr::Quaternion Rotation);

		NEXUS_COMPONENT_DECLARATION(NEXUS_ENGINE_API, Transform)

		NEXUS_ENGINE_API Transform();
		NEXUS_ENGINE_API ~Transform();

		NEXUS_ENGINE_API void Translate(NxFr::Vector3f Delta, TransformSpace Space = TransformSpace::World);
		NEXUS_ENGINE_API void Rotate(NxFr::Quaternion Delta, TransformSpace Space = TransformSpace::World);
		NEXUS_ENGINE_API void Scale(NxFr::Vector3f Factor, TransformSpace Space = TransformSpace::World);
		NEXUS_ENGINE_API void LookAtPosition(NxFr::Vector3f Target, NxFr::Vector3f Up = NxFr::Vector3f::Up, TransformSpace Space = TransformSpace::World);
		NEXUS_ENGINE_API void LookAtDirection(NxFr::Vector3f Direction, NxFr::Vector3f Up = NxFr::Vector3f::Up, TransformSpace Space = TransformSpace::World);

		NEXUS_ENGINE_API NxFr::Vector3f GetPosition(TransformSpace Space = TransformSpace::World) const;
		NEXUS_ENGINE_API void SetPosition(NxFr::Vector3f Position, TransformSpace Space = TransformSpace::World);
		NEXUS_ENGINE_API NxFr::Quaternion GetRotation(TransformSpace Space = TransformSpace::World) const;
		NEXUS_ENGINE_API void SetRotation(NxFr::Quaternion Rotation, TransformSpace Space = TransformSpace::World);
		NEXUS_ENGINE_API NxFr::Vector3f GetScale(TransformSpace Space = TransformSpace::World) const;
		NEXUS_ENGINE_API void SetScale(NxFr::Vector3f Scale, TransformSpace Space = TransformSpace::World);
		NEXUS_ENGINE_API NxFr::Matrix4x4f GetMatrix() const;
		NEXUS_ENGINE_API void SetMatrix(NxFr::Matrix4x4f Matrix);

		NEXUS_ENGINE_API NxFr::Vector3f Right(TransformSpace Space = TransformSpace::World) const;
		NEXUS_ENGINE_API NxFr::Vector3f Up(TransformSpace Space = TransformSpace::World) const;
		NEXUS_ENGINE_API NxFr::Vector3f Forward(TransformSpace Space = TransformSpace::World) const;

		NEXUS_ENGINE_API NxFr::Matrix4x4f LocalToWorld() const;
		NEXUS_ENGINE_API NxFr::Matrix4x4f WorldToLocal() const;

	protected:
		NEXUS_ENGINE_API void OnGui(float TimeStep) override;
		NEXUS_ENGINE_API void OnClone(const Object& Other) override;
		NEXUS_ENGINE_API void OnSave(YAML::Node& Node) override;
		NEXUS_ENGINE_API void OnLoad(const YAML::Node& Node) override;

	private:
		NEXUS_ENGINE_API NxFr::Handle<Transform> GetParent() const;

	private:
		NxFr::Vector3f Position;
		NxFr::Quaternion Rotation;
		NxFr::Vector3f Scaling;
	};
}

