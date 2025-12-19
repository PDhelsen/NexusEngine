#pragma once

#include "NexusEngine/Application/Object.h"

namespace NxEn
{
	class GameObject : public Object
	{
		friend class World;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, GameObject)

		NEXUS_ENGINE_API GameObject(NxFr::StringId WorldId, NxFr::StringView Name);
		NEXUS_ENGINE_API ~GameObject();

		NEXUS_ENGINE_API World* GetWorld() const;
		NEXUS_ENGINE_API GameObject* GetParent() const;
		NEXUS_ENGINE_API GameObject* GetPrevious() const;
		NEXUS_ENGINE_API GameObject* GetNext() const;
		NEXUS_ENGINE_API GameObject* GetSibling() const;
		NEXUS_ENGINE_API GameObject* GetChild() const;
		NEXUS_ENGINE_API GameObject* GetSibling(uint64 Index) const;
		NEXUS_ENGINE_API GameObject* GetChild(uint64 Index) const;
		NEXUS_ENGINE_API uint64 GetSiblingCount() const;
		NEXUS_ENGINE_API uint64 GetChildCount(bool Recursive = false) const;

		NEXUS_ENGINE_API GameObject* GetIterator() const;
		NEXUS_ENGINE_API uint64 GetOrderIndex() const;

		NEXUS_ENGINE_API void SetName(NxFr::StringView Name) { this->Name = Name; };
		NEXUS_ENGINE_API NxFr::StringView GetName() const override { return Name; };
		NEXUS_ENGINE_API NxFr::GUID GetId() const override { return Id; };

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

	private:
		NxFr::GUID Id;
		NxFr::StringId WorldId;
		NxFr::String Name;

		GameObject* Parent;
		GameObject* Prev;
		GameObject* Next;
		GameObject* Child;
	};
}
