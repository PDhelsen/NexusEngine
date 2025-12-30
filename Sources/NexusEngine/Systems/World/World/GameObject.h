#pragma once

#include "NexusEngine/Application/Object.h"

namespace NxEn
{
	class World;
	class Behaviour;
	class Component;

	class GameObject : public Object
	{
		friend class WorldSystem;
		friend class ObjectFactory;
		friend class World;
		friend class Scene;
		friend class Prefab;

		const uint8 ObjectFlag_EnabledInHierarchy = (uint8)ObjectFlags::Flag_1;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, GameObject)

		NEXUS_ENGINE_API GameObject(NxFr::GUID WorldId);
		NEXUS_ENGINE_API ~GameObject();

		NEXUS_ENGINE_API void Initialize() override;
		NEXUS_ENGINE_API void Shutdown() override;
		NEXUS_ENGINE_API void Tick(float TimeStep = 0.0f) override;
		NEXUS_ENGINE_API void DrawGui(float TimeStep = 0.0f);

		NEXUS_ENGINE_API void SetEnabled(bool Enabled) override;
		NEXUS_ENGINE_API bool IsEnabledInHierarchy() const;
		NEXUS_ENGINE_API bool IsTicking() const override;

		NEXUS_ENGINE_API YAML::Node Save() override;
		NEXUS_ENGINE_API void Load(const YAML::Node& Node) override;
		NEXUS_ENGINE_API void Unload() override;

		NEXUS_ENGINE_API World* GetWorld() const;
		NEXUS_ENGINE_API NxFr::Handle<GameObject> GetParent() const;
		NEXUS_ENGINE_API NxFr::Handle<GameObject> GetPrevious() const;
		NEXUS_ENGINE_API NxFr::Handle<GameObject> GetNext() const;
		NEXUS_ENGINE_API NxFr::Handle<GameObject> GetSibling() const;
		NEXUS_ENGINE_API NxFr::Handle<GameObject> GetChild() const;
		NEXUS_ENGINE_API NxFr::Handle<GameObject> GetSibling(uint64 Index) const;
		NEXUS_ENGINE_API NxFr::Handle<GameObject> GetChild(uint64 Index) const;
		NEXUS_ENGINE_API uint64 GetSiblingCount() const;
		NEXUS_ENGINE_API uint64 GetChildCount(bool Recursive = false) const;

		NEXUS_ENGINE_API NxFr::Handle<GameObject> GetIterator() const;
		NEXUS_ENGINE_API uint64 GetOrderIndex() const;

		template<typename T> NxFr::Handle<T> GetBehaviour();
		NEXUS_ENGINE_API NxFr::Handle<Behaviour> GetBehaviourById(NxFr::GUID Id);
		NEXUS_ENGINE_API NxFr::Handle<Behaviour> GetBehaviourByType(NxFr::StringId Id);

		template<typename T> NxFr::Handle<T> GetComponent();
		NEXUS_ENGINE_API NxFr::Handle<Component> GetComponentById(NxFr::GUID Id);
		NEXUS_ENGINE_API NxFr::Handle<Component> GetComponentByType(NxFr::StringId Id);

		NEXUS_ENGINE_API NxFr::GUID GetId() const override { return GetGameObjectId(); };
		NEXUS_ENGINE_API NxFr::GUID GetWorldId() const { return WorldId; };
		NEXUS_ENGINE_API NxFr::GUID GetGameObjectId() const { return GameObjectId; };
		NEXUS_ENGINE_API NxFr::GUID GetReferenceId() const { return ReferenceId; };
		NEXUS_ENGINE_API NxFr::StringView GetName() const override { return Name; };
		NEXUS_ENGINE_API void SetName(NxFr::StringView Name) { this->Name = Name; };
		NEXUS_ENGINE_API bool IsRoot() const { return !Parent && !Prev && !Next; };

	protected:
		NEXUS_ENGINE_API void OnGui(float TimeStep) override;
		NEXUS_ENGINE_API void OnClone(const Object& Other) override;
		NEXUS_ENGINE_API void OnSave(YAML::Node& Node) override;
		NEXUS_ENGINE_API void OnLoad(const YAML::Node& Node) override;
		NEXUS_ENGINE_API void OnUnload() override;
		NEXUS_ENGINE_API void OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids) override;

	private:
		NEXUS_ENGINE_API void UpdateEnabledInHierarchy();
		NEXUS_ENGINE_API void PatchReferences();

		static NxFr::GUID ReadIdFromYaml(const YAML::Node& Node);

	private:
		NxFr::GUID WorldId;
		NxFr::GUID GameObjectId;
		NxFr::GUID ReferenceId;

		NxFr::String Name;

		NxFr::Handle<GameObject> Parent;
		NxFr::Handle<GameObject> Prev;
		NxFr::Handle<GameObject> Next;
		NxFr::Handle<GameObject> Child;

		NxFr::List<NxFr::Handle<Behaviour>> Behaviours;
		NxFr::List<NxFr::Handle<Component>> Components;
	};

	template<typename T>
	inline NxFr::Handle<T> GameObject::GetBehaviour()
	{
		return static_cast<NxFr::Handle<T>>(GetBehaviourByType(T::GetClassType()));
	}

	template<typename T>
	inline NxFr::Handle<T> GameObject::GetComponent()
	{
		return static_cast<NxFr::Handle<T>>(GetComponentByType(T::GetClassType()));
	}
}
