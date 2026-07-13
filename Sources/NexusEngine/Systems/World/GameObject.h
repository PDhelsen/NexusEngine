#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Object/Object.h"
#include "NexusEngine/Systems/World/Behaviour.h"
#include "NexusEngine/Systems/World/Component.h"
#include "NexusEngine/Systems/World/Misc/Iterator.h"

namespace NxEn
{
	class World;

	class NX_ENGINE_API GameObject : public Object
	{
		friend class WorldManager;

	public:
		NX_OBJECT(GameObject)

		GameObject();
		~GameObject();
		

		void Initialize() override;
		void Shutdown() override;
		void UpdateHierarchy() override;
		void Tick(float TimeStep = 0.0f) override;
		void Draw() override;

		NxFr::GUID GetId() const override;
		NxFr::GUID GetWorldId() const;
		NxFr::GUID GetGameObjectId() const;
		NxFr::StringView GetName() const;
		void SetName(NxFr::StringView Name);
		bool IsRoot() const;

		World* GetWorld() const;
		NxFr::Handle<GameObject> GetThis() const;
		NxFr::Handle<GameObject> GetParent() const;
		NxFr::Handle<GameObject> GetPrevious() const;
		NxFr::Handle<GameObject> GetNext() const;
		NxFr::Handle<GameObject> GetSibling() const;
		NxFr::Handle<GameObject> GetChild() const;
		NxFr::Handle<GameObject> GetSibling(uint64 Index) const;
		NxFr::Handle<GameObject> GetChild(uint64 Index) const;
		uint64 GetSiblingCount() const;
		uint64 GetChildCount(bool Recursive = false) const;
		uint64 GetOrderIndex() const;

		NxFr::Handle<GameObject> GetIterator() const;
		Iterator::WorldHierarchy BeginChild() const;
		Iterator::WorldHierarchy EndChild() const;

		template<typename T> NxFr::Handle<T> GetBehaviour() { return static_cast<NxFr::Handle<T>>(GetBehaviourByType(T::GetClassType())); }
		template<typename T> NxFr::Array<NxFr::Handle<T>> GetBehaviours() { return static_cast<NxFr::Array<NxFr::Handle<T>>>(GetBehavioursByType(T::GetClassType())); }
		template<typename T> NxFr::Array<NxFr::Handle<T>> GetBehavioursInChildren() { return static_cast<NxFr::Array<NxFr::Handle<T>>>(GetBehavioursInChildrenByType(T::GetClassType())); }
		NxFr::Handle<Behaviour> GetBehaviourById(NxFr::GUID BehaviourId);
		NxFr::Handle<Behaviour> GetBehaviourByType(NxFr::StringId BehaviourType);
		NxFr::Array<NxFr::Handle<Behaviour>> GetBehavioursByType(NxFr::StringId BehaviourType);
		void GetBehavioursByType(NxFr::StringId Id, NxFr::List<NxFr::Handle<Behaviour>>& Result);
		NxFr::Array<NxFr::Handle<Behaviour>> GetBehavioursInChildrenByType(NxFr::StringId BehaviourType);
		void GetBehavioursInChildrenByType(NxFr::StringId Id, NxFr::List<NxFr::Handle<Behaviour>>& Result);

		template<typename T> NxFr::Handle<T> GetComponent() { return static_cast<NxFr::Handle<T>>(GetComponentByType(T::GetClassType())); }
		template<typename T> NxFr::Array<NxFr::Handle<T>> GetComponents() { return static_cast<NxFr::Array<NxFr::Handle<T>>>(GetComponentsByType(T::GetClassType())); }
		template<typename T> NxFr::Array<NxFr::Handle<T>> GetComponentsInChildren() { return static_cast<NxFr::Array<NxFr::Handle<T>>>(GetComponentsInChildrenByType(T::GetClassType())); }
		NxFr::Handle<Component> GetComponentById(NxFr::GUID ComponentId);
		NxFr::Handle<Component> GetComponentByType(NxFr::StringId ComponentType);
		NxFr::Array<NxFr::Handle<Component>> GetComponentsByType(NxFr::StringId ComponentType);
		void GetComponentsByType(NxFr::StringId Id, NxFr::List<NxFr::Handle<Component>>& Result);
		NxFr::Array<NxFr::Handle<Component>> GetComponentsInChildrenByType(NxFr::StringId ComponentType);
		void GetComponentsInChildrenByType(NxFr::StringId Id, NxFr::List<NxFr::Handle<Component>>& Result);

	protected:
		void OnUpdateHierarchy() override;
		void OnDraw() override;
		void OnClone(const Object& Other) override;

	private:
		NxFr::GUID GameObjectId;
		NxFr::GUID WorldId;

		NxFr::String Name;

		NxFr::Handle<GameObject> Parent;
		NxFr::Handle<GameObject> Prev;
		NxFr::Handle<GameObject> Next;
		NxFr::Handle<GameObject> Child;

		NxFr::List<NxFr::Handle<Behaviour>> Behaviours;
		NxFr::List<NxFr::Handle<Component>> Components;
	};
}
