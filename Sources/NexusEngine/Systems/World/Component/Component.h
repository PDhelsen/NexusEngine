#pragma once

#include "NexusEngine/Application/Object.h"
#include "NexusEngine/Systems/World/Component/ComponentsFactory.h"

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

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Component)

		NEXUS_ENGINE_API Component();
		NEXUS_ENGINE_API virtual ~Component();

		NEXUS_ENGINE_API void DrawGui(float TimeStep = 0.0f);

		NEXUS_ENGINE_API NxFr::GUID GetId() const override { return ComponentId; };
		NEXUS_ENGINE_API NxFr::Handle<GameObject> GetGameObject() const { return Target; };

	protected:
		NEXUS_ENGINE_API virtual void OnGui(float TimeStep);
		NEXUS_ENGINE_API void OnClone(const Object& Other) override;
		NEXUS_ENGINE_API virtual void OnSave(YAML::Node& Node) {};
		NEXUS_ENGINE_API virtual void OnLoad(const YAML::Node& Node) {};
		NEXUS_ENGINE_API virtual void OnUnload() {};

	private:
		NEXUS_ENGINE_API YAML::Node Save();
		NEXUS_ENGINE_API void Load(const YAML::Node& Node);
		NEXUS_ENGINE_API void Unload();

		NEXUS_ENGINE_API virtual void PatchReferences() {};

		static inline NxFr::GUID ReadIdFromYaml(const YAML::Node& Node) { return Node["Id"].as<NxFr::GUID>(); }
		static inline NxFr::StringId ReadTypeFromYaml(const YAML::Node& Node) { return Node["Type"].as<NxFr::StringId>(); }

	private:
		NxFr::GUID ComponentId;
		NxFr::Handle<GameObject> Target;
	};
}
