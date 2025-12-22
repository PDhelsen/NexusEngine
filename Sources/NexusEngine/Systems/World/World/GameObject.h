#pragma once

#include "NexusEngine/Application/Object.h"

namespace NxEn
{
	class World;

	class GameObject : public Object
	{
		friend class ObjectFactory;
		friend class World;
		friend class Prefab;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, GameObject)

		NEXUS_ENGINE_API GameObject(NxFr::GUID WorldId);
		NEXUS_ENGINE_API ~GameObject();

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

		NEXUS_ENGINE_API NxFr::GUID GetId() const override { return GetGameObjectId(); };
		NEXUS_ENGINE_API NxFr::GUID GetWorldId() const { return WorldId; };
		NEXUS_ENGINE_API NxFr::GUID GetGameObjectId() const { return GameObjectId; };
		NEXUS_ENGINE_API NxFr::GUID GetReferenceId() const { return ReferenceId; };
		NEXUS_ENGINE_API NxFr::StringView GetName() const override { return Name; };
		NEXUS_ENGINE_API void SetName(NxFr::StringView Name) { this->Name = Name; };

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnEnable() override;
		NEXUS_ENGINE_API void OnDisable() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

	private:
		NxFr::GUID WorldId;
		NxFr::GUID GameObjectId;
		NxFr::GUID ReferenceId;
		NxFr::String Name;

		NxFr::Handle<GameObject> Parent;
		NxFr::Handle<GameObject> Prev;
		NxFr::Handle<GameObject> Next;
		NxFr::Handle<GameObject> Child;
	};
}
