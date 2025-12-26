#pragma once

#include "NexusEngine/Application/Object.h"
#include "NexusEngine/Systems/World/Behaviour/BehavioursFactory.h"

#define NEXUS_BEHAVIOUR_DECLARATION(Dll, Type) \
NEXUS_OBJECT_DECLARATION(Dll, Type)

#define NEXUS_BEHAVIOUR_IMPLEMENTATION(Type) \
NEXUS_OBJECT_IMPLEMENTATION(Type)\
NxEn::BehavioursFactory::Factory* Factory##Type = NxEn::BehavioursFactory::Register<Type>();

namespace NxEn
{
	class Behaviour : public Object
	{
		friend class ObjectFactory;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Behaviour)

		NEXUS_ENGINE_API Behaviour();
		NEXUS_ENGINE_API ~Behaviour();

		NEXUS_ENGINE_API void Tick(float TimeStep = 0.0f) override;

		NEXUS_ENGINE_API NxFr::GUID GetId() const override { return BehaviourId; };
		NEXUS_ENGINE_API NxFr::Handle<GameObject> GetGameObject() const { return Target; };

	private:
		NxFr::GUID BehaviourId;
		NxFr::Handle<GameObject> Target;
	};
}
