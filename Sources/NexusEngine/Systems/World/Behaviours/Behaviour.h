#pragma once

#include "NexusEngine/Application/Object.h"

#define NX_BEHAVIOUR_DECLARATION(Dll, Type) \
NX_OBJECT_DECLARATION(Dll, Type)

#define NX_BEHAVIOUR_IMPLEMENTATION(Type) \
NX_OBJECT_IMPLEMENTATION(Type)\
NxEn::WorldObjectStorage* Storage##Type = NxEn::WorldObjectStorage::Register<Type>();

namespace NxEn
{
	class Behaviour : public Object
	{
		friend class WorldObjectFactory;
		friend class GameObject;

		const static inline uint8 ObjectFlag_EnabledInHierarchy = (uint8)ObjectFlags::Flag_1;

	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, Behaviour)

		NX_ENGINE_API Behaviour();
		NX_ENGINE_API virtual ~Behaviour();

		NX_ENGINE_API void SetEnabled(bool Enabled) override;
		NX_ENGINE_API bool IsEnabledInHierarchy() const;
		NX_ENGINE_API bool IsTicking() const override;

		NX_ENGINE_API void PatchReferences();
		NX_ENGINE_API void UpdateHierarchy();

		NX_ENGINE_API NxFr::StringView GetName() const override;
		NX_ENGINE_API NxFr::GUID GetId() const override;
		NX_ENGINE_API NxFr::Handle<GameObject> GetGameObject() const;

	protected:
		NX_ENGINE_API void OnGui(float TimeStep) override;
		NX_ENGINE_API void OnClone(const Object& Other) override;
		NX_ENGINE_API void OnSave(YAML::Node& Node) override;
		NX_ENGINE_API void OnLoad(const YAML::Node& Node) override;

		NX_ENGINE_API virtual void OnPatchReferences();
		NX_ENGINE_API virtual void OnUpdateHierarchy();

	private:
		NxFr::GUID BehaviourId;
		NxFr::Handle<GameObject> Target;
	};
}
