#pragma once

#include "NexusEngine/Application/Object.h"

#define NEXUS_BEHAVIOUR_DECLARATION(Dll, Type) \
NEXUS_OBJECT_DECLARATION(Dll, Type)

#define NEXUS_BEHAVIOUR_IMPLEMENTATION(Type) \
NEXUS_OBJECT_IMPLEMENTATION(Type)\
NxEn::WorldObjectStorage* Storage##Type = NxEn::WorldObjectStorage::Register<Type>();

namespace NxEn
{
	class Behaviour : public Object
	{
		friend class WorldObjectFactory;
		friend class GameObject;

		const static inline uint8 ObjectFlag_EnabledInHierarchy = (uint8)ObjectFlags::Flag_1;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Behaviour)

		NEXUS_ENGINE_API Behaviour();
		NEXUS_ENGINE_API virtual ~Behaviour();

		NEXUS_ENGINE_API void SetEnabled(bool Enabled) override;
		NEXUS_ENGINE_API bool IsEnabledInHierarchy() const;
		NEXUS_ENGINE_API bool IsTicking() const override;

		NEXUS_ENGINE_API void PatchReferences();
		NEXUS_ENGINE_API void UpdateHierarchy();

		NEXUS_ENGINE_API NxFr::StringView GetName() const override;
		NEXUS_ENGINE_API NxFr::GUID GetId() const override;
		NEXUS_ENGINE_API NxFr::Handle<GameObject> GetGameObject() const;

	protected:
		NEXUS_ENGINE_API void OnGui(float TimeStep) override;
		NEXUS_ENGINE_API void OnClone(const Object& Other) override;
		NEXUS_ENGINE_API void OnSave(YAML::Node& Node) override;
		NEXUS_ENGINE_API void OnLoad(const YAML::Node& Node) override;

		NEXUS_ENGINE_API virtual void OnPatchReferences();
		NEXUS_ENGINE_API virtual void OnUpdateHierarchy();

	private:
		NxFr::GUID BehaviourId;
		NxFr::Handle<GameObject> Target;
	};
}
