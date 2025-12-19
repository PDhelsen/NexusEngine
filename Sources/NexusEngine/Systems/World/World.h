#pragma once

#include "NexusEngine/Application/Object.h"
#include "NexusEngine/Systems/World/GameObject.h"

namespace NxEn
{
	class World : public Object
	{
		struct Info
		{
			uint64 Index;
			NxFr::Handle<GameObject> Handle;
		};

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, World)

		NEXUS_ENGINE_API World(NxFr::StringId Id);
		NEXUS_ENGINE_API ~World();

		NEXUS_ENGINE_API NxFr::Handle<GameObject> GetGameObject(NxFr::GUID Id);
		NEXUS_ENGINE_API NxFr::Handle<GameObject> CreateGameObject(NxFr::StringView Name, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>());
		NEXUS_ENGINE_API NxFr::Handle<GameObject> DuplicateGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent = NxFr::Handle<GameObject>());
		NEXUS_ENGINE_API void DestroyGameObject(NxFr::Handle<GameObject> Instance);
		NEXUS_ENGINE_API void AttachGameObject(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, int64 Index = -1);

		NEXUS_ENGINE_API bool Belong(NxFr::Handle<GameObject> Instance);
		NEXUS_ENGINE_API NxFr::Array<NxFr::Handle<GameObject>> Find(NxFr::StringView Filter);

		NEXUS_ENGINE_API uint64 GetObjectCount() const { return Objects.GetCount(); }
		NEXUS_ENGINE_API NxFr::Handle<GameObject> GetRoot() { return Root; };

		NEXUS_ENGINE_API NxFr::StringView GetName() const override { return Name; };
		NEXUS_ENGINE_API NxFr::GUID GetId() const override { return Id; };

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

	private:
		NEXUS_ENGINE_API NxFr::Handle<GameObject> Allocate();
		NEXUS_ENGINE_API void Free(NxFr::Handle<GameObject> Instance);
		NEXUS_ENGINE_API void Attach(NxFr::Handle<GameObject> Instance, NxFr::Handle<GameObject> Parent, uint64 Index);
		NEXUS_ENGINE_API void Detach(NxFr::Handle<GameObject> Instance);

	private:
		NxFr::StringId Id;
		NxFr::String Name;

		NxFr::List<GameObject> Objects;
		NxFr::Handle<GameObject> Root;

		NxFr::Dictionary<NxFr::GUID, Info> Infos;
		NxFr::Stack<uint64> Availables;
		HandleManager Handles;
	};
}

