#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Assets/Asset.h"
#include "AssetsManager.h"

namespace NxFr
{
	namespace StatsHeader
	{
		NEXUS_ENGINE_API extern const NxFr::StringId AssetsTrackedId;
		NEXUS_ENGINE_API extern const NxFr::StringId AssetsLoadedId;
	}
}
namespace NxEn
{
	class AssetsSystem : public System
	{
	public:
		const NxFr::StringId EventCreateId = "Create"_Sid;
		const NxFr::StringId EventRenameId = "Rename"_Sid;
		const NxFr::StringId EventMoveId = "Move"_Sid;
		const NxFr::StringId EventDeleteId = "Delete"_Sid;
		const NxFr::StringId EventSaveId = "Save"_Sid;
		const NxFr::StringId EventAcquireId = "Acquire"_Sid;
		const NxFr::StringId EventTrackId = "Track"_Sid;
		const NxFr::StringId EventReleaseId = "Release"_Sid;

		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, AssetsSystem)

		template<typename T>
		T* Create(NxFr::StringView Path);
		NEXUS_ENGINE_API void Rename(NxFr::GUID Id, NxFr::StringView Name);
		NEXUS_ENGINE_API void Move(NxFr::GUID Id, NxFr::StringView Path);
		NEXUS_ENGINE_API void Delete(NxFr::GUID Id);
		NEXUS_ENGINE_API void Save(NxFr::GUID Id);
		NEXUS_ENGINE_API void Save();

		template<typename T>
		T* Acquire(NxFr::GUID Id);
		NEXUS_ENGINE_API Asset* Get(NxFr::GUID Id);
		NEXUS_ENGINE_API void Track(Asset* Instance, NxFr::StringView Path = "");
		NEXUS_ENGINE_API void Release(NxFr::GUID Id, bool Keep = false);
		NEXUS_ENGINE_API void Purge();

		NEXUS_ENGINE_API NxFr::List<NxFr::GUID> Find(NxFr::StringView Filter) const;
		NEXUS_ENGINE_API NxFr::String IdToPath(NxFr::GUID Id) const;
		NEXUS_ENGINE_API NxFr::GUID PathToId(NxFr::StringView Path) const;

		NEXUS_ENGINE_API NxFr::Event<NxFr::StringId, NxFr::GUID>& GetOnEvent() { return OnEvent; }
		NEXUS_ENGINE_API NxFr::Event<Asset*>& GetOnSave() { return OnSave; }

	private:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

		void RecordStats() const;
		void UpdateStats() const;

		void Create_Append(NxFr::StringView Path, Asset* Instance);
		Asset* Acquire_Check(NxFr::GUID Id);
		void Acquire_Load(NxFr::GUID Id, Asset* Instance);

	private:
		NxFr::Event<NxFr::StringId, NxFr::GUID> OnEvent;
		NxFr::Event<Asset*> OnSave;

		class AssetsRegistry* Registry;
		class AssetsManager* Manager;
	};

	template<typename T>
	inline T* AssetsSystem::Create(NxFr::StringView Path)
	{
		T* Instance = new T();
		Create_Append(Path, Instance);
		return Instance;
	}

	template<typename T>
	inline T* AssetsSystem::Acquire(NxFr::GUID Id)
	{
		T* Instance = static_cast<T*>(Acquire_Check(Id));
		if (!Instance)
		{
			Instance = new T();
			Acquire_Load(Id, Instance);
		}

		return Instance;
	}
}
