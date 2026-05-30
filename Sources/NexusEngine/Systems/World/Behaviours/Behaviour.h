#pragma once

#include "NexusEngine/Application/Object/Object.h"

#define NX_BEHAVIOUR_DECLARATION(Type) \
NX_OBJECT(Type)

#define NX_BEHAVIOUR_IMPLEMENTATION(Type) \
NxEn::WorldObjectStorage* Storage##Type = NxEn::WorldObjectStorage::Register<Type>();

namespace NxEn
{
	class NX_ENGINE_API Behaviour : public Object
	{
		friend class WorldObjectFactory;
		friend class GameObject;

	public:
		NX_OBJECT(Behaviour)

		Behaviour();
		virtual ~Behaviour();

		bool IsTicking() const override;

		NxFr::StringView GetName() const override;
		NxFr::GUID GetId() const override;
		NxFr::Handle<GameObject> GetGameObject() const;

	protected:
		void OnUpdateHierarchy() override;
		void OnDraw() override;
		void OnClone(const Object& Other) override;
		void OnSerialize(YAML::Node& Node) override;
		void OnDeserialize(const YAML::Node& Node);
		void OnPatchReferences() override;

	private:
		NxFr::GUID BehaviourId;
		NxFr::Handle<GameObject> Target;
	};
}
