#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Object/Object.h"

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

		NxFr::GUID GetId() const override;
		NxFr::GUID GetWorldId() const;
		NxFr::GUID GetGameObjectId() const;
		NxFr::StringView GetName() const;
		void SetName(NxFr::StringView Name);
		bool IsRoot() const;

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

		NxFr::GUID GetId() const override { return GetGameObjectId(); };
		NxFr::GUID GetWorldId() const { return WorldId; };
		NxFr::GUID GetGameObjectId() const { return GameObjectId; };
		NxFr::StringView GetName() const override { return Name; };
		void SetName(NxFr::StringView Name) { this->Name = Name; };
		bool IsRoot() const { return !Parent && !Prev && !Next; }

	protected:
		void OnUpdateHierarchy() override;
		void OnClone(const Object& Other) override;

	private:
		NxFr::GUID GameObjectId;
		NxFr::GUID WorldId;

		NxFr::String Name;

		NxFr::Handle<GameObject> Parent;
		NxFr::Handle<GameObject> Prev;
		NxFr::Handle<GameObject> Next;
		NxFr::Handle<GameObject> Child;
	};
}
