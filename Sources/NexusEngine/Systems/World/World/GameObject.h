#pragma once

#include "NexusEngine/Application/Object.h"
#include "NexusEngine/Systems/World/Behaviours/Behaviour.h"
#include "NexusEngine/Systems/World/Components/Component.h"

namespace NxEn
{
	class GameObject : public Object
	{
		friend class WorldObjectFactory;
		friend class WorldSystem;
		friend class World;
		friend class Scene;
		friend class Prefab;

		const static inline uint8 ObjectFlag_EnabledInHierarchy = (uint8)ObjectFlags::Flag_1;

	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, GameObject)

		NX_ENGINE_API GameObject();
		NX_ENGINE_API ~GameObject();

		NX_ENGINE_API void Initialize() override;
		NX_ENGINE_API void Shutdown() override;
		NX_ENGINE_API void Tick(float TimeStep = 0.0f) override;
		NX_ENGINE_API void DrawGui(float TimeStep = 0.0f);

		NX_ENGINE_API void SetEnabled(bool Enabled) override;
		NX_ENGINE_API bool IsEnabledInHierarchy() const;
		NX_ENGINE_API bool IsTicking() const override;

		NX_ENGINE_API YAML::Node Save() override;
		NX_ENGINE_API void Load(const YAML::Node& Node) override;
		NX_ENGINE_API void Unload() override;
		NX_ENGINE_API NxFr::Array<NxFr::GUID> GetDependencies() override;

		NX_ENGINE_API void PatchReferences();
		NX_ENGINE_API void UpdateHierarchy();

		NX_ENGINE_API World* GetWorld() const;
		NX_ENGINE_API NxFr::Handle<GameObject> GetParent() const;
		NX_ENGINE_API NxFr::Handle<GameObject> GetPrevious() const;
		NX_ENGINE_API NxFr::Handle<GameObject> GetNext() const;
		NX_ENGINE_API NxFr::Handle<GameObject> GetSibling() const;
		NX_ENGINE_API NxFr::Handle<GameObject> GetChild() const;
		NX_ENGINE_API NxFr::Handle<GameObject> GetSibling(uint64 Index) const;
		NX_ENGINE_API NxFr::Handle<GameObject> GetChild(uint64 Index) const;
		NX_ENGINE_API uint64 GetSiblingCount() const;
		NX_ENGINE_API uint64 GetChildCount(bool Recursive = false) const;

		NX_ENGINE_API NxFr::Handle<GameObject> GetIterator() const;
		NX_ENGINE_API uint64 GetOrderIndex() const;

		template<typename T> NxFr::Handle<T> GetBehaviour();
		NX_ENGINE_API NxFr::Handle<Behaviour> GetBehaviourById(NxFr::GUID Id);
		NX_ENGINE_API NxFr::Handle<Behaviour> GetBehaviourByType(NxFr::StringId Id);
		template<typename T> NxFr::Array<NxFr::Handle<T>> GetBehaviours();
		NX_ENGINE_API NxFr::Array<NxFr::Handle<Behaviour>> GetBehavioursByType(NxFr::StringId Id);
		NX_ENGINE_API void GetBehavioursByType(NxFr::StringId Id, NxFr::List<NxFr::Handle<Behaviour>>& Result);
		template<typename T> NxFr::Array<NxFr::Handle<T>> GetBehavioursInChildren();
		NX_ENGINE_API NxFr::Array<NxFr::Handle<Behaviour>> GetBehavioursInChildrenByType(NxFr::StringId Id);
		NX_ENGINE_API void GetBehavioursInChildrenByType(NxFr::StringId Id, NxFr::List<NxFr::Handle<Behaviour>>& Result);

		template<typename T> NxFr::Handle<T> GetComponent();
		NX_ENGINE_API NxFr::Handle<Component> GetComponentById(NxFr::GUID Id);
		NX_ENGINE_API NxFr::Handle<Component> GetComponentByType(NxFr::StringId Id);
		template<typename T> NxFr::Array<NxFr::Handle<T>> GetComponents();
		NX_ENGINE_API NxFr::Array<NxFr::Handle<Component>> GetComponentsByType(NxFr::StringId Id);
		NX_ENGINE_API void GetComponentsByType(NxFr::StringId Id, NxFr::List<NxFr::Handle<Component>>& Result);
		template<typename T> NxFr::Array<NxFr::Handle<T>> GetComponentsInChildren();
		NX_ENGINE_API NxFr::Array<NxFr::Handle<Component>> GetComponentsInChildrenByType(NxFr::StringId Id);
		NX_ENGINE_API void GetComponentsInChildrenByType(NxFr::StringId Id, NxFr::List<NxFr::Handle<Component>>& Result);

		NX_ENGINE_API NxFr::GUID GetId() const override { return GetGameObjectId(); };
		NX_ENGINE_API NxFr::GUID GetWorldId() const { return WorldId; };
		NX_ENGINE_API NxFr::GUID GetGameObjectId() const { return GameObjectId; };
		NX_ENGINE_API NxFr::GUID GetReferenceId() const { return ReferenceId; };
		NX_ENGINE_API NxFr::StringView GetName() const override { return Name; };
		NX_ENGINE_API void SetName(NxFr::StringView Name) { this->Name = Name; };
		NX_ENGINE_API bool IsRoot() const { return !Parent && !Prev && !Next; };

	protected:
		NX_ENGINE_API void OnGui(float TimeStep) override;
		NX_ENGINE_API void OnClone(const Object& Other) override;
		NX_ENGINE_API void OnSave(YAML::Node& Node) override;
		NX_ENGINE_API void OnLoad(const YAML::Node& Node) override;
		NX_ENGINE_API void OnUnload() override;
		NX_ENGINE_API void OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids) override;

		NX_ENGINE_API virtual void OnPatchReferences();
		NX_ENGINE_API virtual void OnUpdateHierarchy();

		NX_ENGINE_API static NxFr::GUID ReadIdFromYaml(const YAML::Node& Node);
		NX_ENGINE_API static NxFr::Handle<GameObject> GetThis(GameObject* Instance);

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
	inline NxFr::Array<NxFr::Handle<T>> GameObject::GetBehaviours()
	{
		return static_cast<NxFr::Array<NxFr::Handle<T>>>(GetBehavioursByType(T::GetClassType()));
	}

	template<typename T>
	inline NxFr::Array<NxFr::Handle<T>> GameObject::GetBehavioursInChildren()
	{
		return static_cast<NxFr::Array<NxFr::Handle<T>>>(GetBehavioursInChildrenByType(T::GetClassType()));
	}

	template<typename T>
	inline NxFr::Handle<T> GameObject::GetComponent()
	{
		return static_cast<NxFr::Handle<T>>(GetComponentByType(T::GetClassType()));
	}

	template<typename T>
	inline NxFr::Array<NxFr::Handle<T>> GameObject::GetComponents()
	{
		return static_cast<NxFr::Array<NxFr::Handle<T>>>(GetComponentsByType(T::GetClassType()));
	}

	template<typename T>
	inline NxFr::Array<NxFr::Handle<T>> GameObject::GetComponentsInChildren()
	{
		return static_cast<NxFr::Array<NxFr::Handle<T>>>(GetComponentsInChildrenByType(T::GetClassType()));
	}
}
