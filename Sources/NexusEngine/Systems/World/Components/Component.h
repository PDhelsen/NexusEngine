#pragma once

#include "NexusEngine/Application/Object.h"
#include "NexusEngine/Systems/World/Components/ComponentsFactory.h"

#define NEXUS_COMPONENT_DECLARATION(Dll, Type) \
NEXUS_OBJECT_DECLARATION(Dll, Type)

#define NEXUS_COMPONENT_IMPLEMENTATION(Type) \
NEXUS_OBJECT_IMPLEMENTATION(Type)\
NxEn::ComponentsFactory* Factory##Type = NxEn::ComponentsFactory::Register<Type>();

namespace NxEn
{
	class Component : public Object
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
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Component)

		NEXUS_ENGINE_API Component();
		NEXUS_ENGINE_API virtual ~Component();

		NEXUS_ENGINE_API void SetEnabled(bool Enabled) override;
		NEXUS_ENGINE_API bool IsEnabledInHierarchy() const;
		NEXUS_ENGINE_API bool IsTicking() const override;

		NEXUS_ENGINE_API NxFr::GUID GetId() const override { return ComponentId; };
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
		NxFr::GUID ComponentId;
		NxFr::Handle<GameObject> Target;
	};
}
