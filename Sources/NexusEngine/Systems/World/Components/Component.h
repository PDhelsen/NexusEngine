#pragma once

#include "NexusEngine/Application/Object/Object.h"

#define NX_COMPONENT_DECLARATION(Type) \
NX_OBJECT(Type)

#define NX_COMPONENT_IMPLEMENTATION(Type) \
NxEn::WorldObjectStorage* Storage##Type = NxEn::WorldObjectStorage::Register<Type>();

namespace NxEn
{
	class NX_ENGINE_API Component : public Object
	{
		friend class WorldObjectFactory;
		friend class GameObject;

	public:
		NX_OBJECT(Component)

		Component();
		virtual ~Component();

		bool IsTicking() const override;

		NxFr::StringView GetName() const override;
		NxFr::GUID GetId() const override;
		NxFr::Handle<GameObject> GetGameObject() const;

	protected:
		void OnUpdateHierarchy() override;
		void OnDraw() override;
		void OnClone(const Object& Other) override;
		void OnSerialize(YAML::Node& Node) override;
		void OnDeserialize(const YAML::Node& Node) override;
		void OnPatchReferences() override;

	private:
		NxFr::GUID ComponentId;
		NxFr::Handle<GameObject> Target;
	};
}
