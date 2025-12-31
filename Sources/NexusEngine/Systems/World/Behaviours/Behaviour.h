#pragma once

#include "NexusEngine/Application/Object.h"
#include "NexusEngine/Systems/World/Behaviours/BehavioursFactory.h"

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
		friend class GameObject;

	protected:
		template<typename T>
		union SerializedReference
		{
			NxFr::GUID Id;
			NxFr::Handle<T> Handle;
		};

		const static inline uint8 ObjectFlag_EnabledInHierarchy = (uint8)ObjectFlags::Flag_1;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Behaviour)

		NEXUS_ENGINE_API Behaviour();
		NEXUS_ENGINE_API virtual ~Behaviour();

		NEXUS_ENGINE_API void SetEnabled(bool Enabled) override;
		NEXUS_ENGINE_API bool IsEnabledInHierarchy() const;
		NEXUS_ENGINE_API bool IsTicking() const override;

		NEXUS_ENGINE_API NxFr::GUID GetId() const override { return BehaviourId; };
		NEXUS_ENGINE_API NxFr::Handle<GameObject> GetGameObject() const { return Target; };

	protected:
		NEXUS_ENGINE_API void OnGui(float TimeStep) override;
		NEXUS_ENGINE_API void OnClone(const Object& Other) override;
		NEXUS_ENGINE_API void OnSave(YAML::Node& Node) override;
		NEXUS_ENGINE_API void OnLoad(const YAML::Node& Node) override;

	private:
		NEXUS_ENGINE_API void UpdateEnabledInHierarchy();
		NEXUS_ENGINE_API virtual void PatchReferences() {};

	private:
		NxFr::GUID BehaviourId;
		NxFr::Handle<GameObject> Target;
	};
}
