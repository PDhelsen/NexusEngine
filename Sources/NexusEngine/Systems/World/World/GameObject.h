#pragma once

#include "NexusEngine/Application/Object.h"

namespace NxEn
{
	class World;

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

		NEXUS_ENGINE_API void SetEnabled(bool Enabled) override;
		NEXUS_ENGINE_API bool IsEnabledInHierarchy() const;

		NEXUS_ENGINE_API Object* Clone() const override;
		NEXUS_ENGINE_API void Clone(Object* Target) override;
		NEXUS_ENGINE_API void Clone(const Object* Target) override;

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
		NEXUS_ENGINE_API bool IsRoot() const { return !Parent && !Prev && !Next; };

	protected:
		NEXUS_ENGINE_API virtual void OnSave(YAML::Node& Node);
		NEXUS_ENGINE_API virtual void OnLoad(const YAML::Node& Node);
		NEXUS_ENGINE_API virtual void OnUnload();

	private:
		NEXUS_ENGINE_API bool UpdateEnabledInHierarchy();

		NEXUS_ENGINE_API YAML::Node Save();
		NEXUS_ENGINE_API void Load(const YAML::Node& Node);
		NEXUS_ENGINE_API void Unload();

		NEXUS_ENGINE_API void GatherDependencies(NxFr::Set<NxFr::GUID>& Result);

		static inline NxFr::GUID ReadIdFromYaml(const YAML::Node& Node) { return Node["Instance"]["Id"].as<NxFr::GUID>(); }
		static inline NxFr::GUID ReadReferenceFromYaml(const YAML::Node& Node) { return Node["Instance"]["Reference"].as<NxFr::GUID>(); }

	private:
		NxFr::GUID WorldId;
		NxFr::GUID GameObjectId;
		NxFr::GUID ReferenceId;

		NxFr::Handle<GameObject> Parent;
		NxFr::Handle<GameObject> Prev;
		NxFr::Handle<GameObject> Next;
		NxFr::Handle<GameObject> Child;

		NxFr::String Name;
	};
}
