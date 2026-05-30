#pragma once

#include "NexusEngine/Application/Object/Object.h"
#include "NexusEngine/Systems/World/Behaviours/Behaviour.h"
#include "NexusEngine/Systems/World/Components/Component.h"

namespace NxEn
{
	class NX_ENGINE_API GameObject : public Object
	{
		friend class WorldObjectFactory;
		friend class WorldSystem;
		friend class World;
		friend class Scene;
		friend class Prefab;

	public:
		NX_OBJECT(GameObject)

		GameObject();
		~GameObject();

		void Initialize() override;
		void Shutdown() override;
		void UpdateHierarchy() override;
		void Tick(float TimeStep = 0.0f) override;
		void Draw() override;

		YAML::Node Save() override;
		void Load(const YAML::Node& Node) override;
		void Unload() override;
		void PatchReferences() override;
		NxFr::Array<NxFr::GUID> GetDependencies() override;

		World* GetWorld() const;
		NxFr::Handle<GameObject> GetParent() const;
		NxFr::Handle<GameObject> GetPrevious() const;
		NxFr::Handle<GameObject> GetNext() const;
		NxFr::Handle<GameObject> GetSibling() const;
		NxFr::Handle<GameObject> GetChild() const;
		NxFr::Handle<GameObject> GetSibling(uint64 Index) const;
		NxFr::Handle<GameObject> GetChild(uint64 Index) const;
		uint64 GetSiblingCount() const;
		uint64 GetChildCount(bool Recursive = false) const;

		NxFr::Handle<GameObject> GetIterator() const;
		uint64 GetOrderIndex() const;

		template<typename T> NxFr::Handle<T> GetBehaviour();
		NxFr::Handle<Behaviour> GetBehaviourById(NxFr::GUID Id);
		NxFr::Handle<Behaviour> GetBehaviourByType(NxFr::StringId Id);
		template<typename T> NxFr::Array<NxFr::Handle<T>> GetBehaviours();
		NxFr::Array<NxFr::Handle<Behaviour>> GetBehavioursByType(NxFr::StringId Id);
		void GetBehavioursByType(NxFr::StringId Id, NxFr::List<NxFr::Handle<Behaviour>>& Result);
		template<typename T> NxFr::Array<NxFr::Handle<T>> GetBehavioursInChildren();
		NxFr::Array<NxFr::Handle<Behaviour>> GetBehavioursInChildrenByType(NxFr::StringId Id);
		void GetBehavioursInChildrenByType(NxFr::StringId Id, NxFr::List<NxFr::Handle<Behaviour>>& Result);

		template<typename T> NxFr::Handle<T> GetComponent();
		NxFr::Handle<Component> GetComponentById(NxFr::GUID Id);
		NxFr::Handle<Component> GetComponentByType(NxFr::StringId Id);
		template<typename T> NxFr::Array<NxFr::Handle<T>> GetComponents();
		NxFr::Array<NxFr::Handle<Component>> GetComponentsByType(NxFr::StringId Id);
		void GetComponentsByType(NxFr::StringId Id, NxFr::List<NxFr::Handle<Component>>& Result);
		template<typename T> NxFr::Array<NxFr::Handle<T>> GetComponentsInChildren();
		NxFr::Array<NxFr::Handle<Component>> GetComponentsInChildrenByType(NxFr::StringId Id);
		void GetComponentsInChildrenByType(NxFr::StringId Id, NxFr::List<NxFr::Handle<Component>>& Result);

		NxFr::GUID GetId() const override { return GetGameObjectId(); };
		NxFr::GUID GetWorldId() const { return WorldId; };
		NxFr::GUID GetGameObjectId() const { return GameObjectId; };
		NxFr::GUID GetReferenceId() const { return ReferenceId; };
		NxFr::StringView GetName() const override { return Name; };
		void SetName(NxFr::StringView Name) { this->Name = Name; };
		bool IsRoot() const { return !Parent && !Prev && !Next; };

	protected:
		void OnUpdateHierarchy() override;
		void OnDraw() override;
		void OnClone(const Object& Other) override;
		void OnSave(YAML::Node& Node) override;
		void OnLoad(const YAML::Node& Node) override;
		void OnUnload() override;
		void OnPatchReferences() override;
		void OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids) override;

		static NxFr::GUID ReadIdFromYaml(const YAML::Node& Node);
		static NxFr::Handle<GameObject> GetThis(GameObject* Instance);

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
