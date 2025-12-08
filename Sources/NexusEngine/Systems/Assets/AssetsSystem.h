#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Assets/Asset.h"
#include "NexusEngine/Systems/Assets/AssetMetadata.h"

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
		const NxFr::StringId EventReloadId = "Reload"_Sid;
		const NxFr::StringId EventReleaseId = "Release"_Sid;
		const NxFr::StringId EventImportId = "Import"_Sid;

		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, AssetsSystem)

		template<typename T>
		T* Create(NxFr::StringView Path = "", NxFr::StringView Extension = "");
		NEXUS_ENGINE_API void Rename(NxFr::GUID Id, NxFr::StringView Name);
		NEXUS_ENGINE_API void Move(NxFr::GUID Id, NxFr::StringView Path);
		NEXUS_ENGINE_API void Delete(NxFr::GUID Id);
		NEXUS_ENGINE_API void Save(NxFr::GUID Id);
		NEXUS_ENGINE_API void Save();

		template<typename T>
		T* Acquire(NxFr::GUID Id);
		NEXUS_ENGINE_API void Track(Asset* Instance, NxFr::StringView Path = "", NxFr::StringView Extension = "");
		NEXUS_ENGINE_API void Reload(NxFr::GUID Id);
		NEXUS_ENGINE_API void Release(NxFr::GUID Id, bool Keep = false);
		NEXUS_ENGINE_API void Purge();

		template<typename T>
		T* Import(const YAML::Node& Node, NxFr::StringView Path, NxFr::StringView Extension);
		template<typename T>
		T* Reimport(const YAML::Node& Node, NxFr::GUID Id);

		NEXUS_ENGINE_API NxFr::Array<NxFr::GUID> Find(NxFr::StringView Filter) const;
		NEXUS_ENGINE_API NxFr::GUID PathToId(NxFr::StringView Path) const;
		NEXUS_ENGINE_API NxFr::String IdToPath(NxFr::GUID Id) const;

		NEXUS_ENGINE_API Asset* GetAsset(NxFr::GUID Id);
		NEXUS_ENGINE_API AssetHandle& GetHandle(NxFr::GUID Id);
		NEXUS_ENGINE_API AssetMetadata& GetMetadata(NxFr::GUID Id);
		NEXUS_ENGINE_API YAML::Node GetImportData(NxFr::GUID Id);
		NEXUS_ENGINE_API NxFr::Array<NxFr::GUID> GetDependencies(NxFr::GUID Id, bool Recusive = false);

		NEXUS_ENGINE_API bool IsTracked(NxFr::GUID Id) const;
		NEXUS_ENGINE_API bool IsLoaded(NxFr::GUID Id) const;

		NEXUS_ENGINE_API NxFr::Event<NxFr::StringId, NxFr::GUID>& GetOnEvent() { return OnEvent; }
		NEXUS_ENGINE_API NxFr::Event<Asset*>& GetOnSave() { return OnSave; }

	private:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

		NEXUS_ENGINE_API void RecordStats() const;
		NEXUS_ENGINE_API void UpdateStats() const;

		NEXUS_ENGINE_API void FetchDependencies(NxFr::GUID Id, bool Recusive, NxFr::Set<NxFr::GUID>& Result);

		NEXUS_ENGINE_API void Create_Append(Asset* Instance, NxFr::StringView Path, NxFr::StringView Extension);
		NEXUS_ENGINE_API Asset* Acquire_Check(NxFr::GUID Id);
		NEXUS_ENGINE_API void Acquire_Load(NxFr::GUID Id, Asset* Instance);
#if NEXUS_EDITOR
		NEXUS_ENGINE_API void Import_Append(Asset* Instance, const YAML::Node& Node, NxFr::StringView Path, NxFr::StringView Extension);
		NEXUS_ENGINE_API void Reimport_Load(Asset* Instance, const YAML::Node& Node, NxFr::GUID Id);
#endif

	private:
		NxFr::Event<NxFr::StringId, NxFr::GUID> OnEvent;
		NxFr::Event<Asset*> OnSave;

		class AssetsRegistry* Registry;
		class AssetsManager* Manager;
	};

	template<typename T>
	inline T* AssetsSystem::Create(NxFr::StringView Path, NxFr::StringView Extension)
	{
		T* Instance = new T();
		Create_Append(Instance, Path, Extension);
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

	template<typename T>
	inline T* AssetsSystem::Import(const YAML::Node& Node, NxFr::StringView Path, NxFr::StringView Extension)
	{
#if NEXUS_EDITOR
		T* Instance = new T();
		Import_Append(Instance, Node, Path, Extension);
		return Instance;
#else
		return nullptr;
#endif
	}

	template<typename T>
	inline T* AssetsSystem::Reimport(const YAML::Node& Node, NxFr::GUID Id)
	{
#if NEXUS_EDITOR
		T* Instance = static_cast<T*>(GetAsset(Id));
		if (!Instance)
		{
			Instance = new T();
		}

		Reimport_Load(Instance, Node, Id);
		return Instance;
#else
		return nullptr;
#endif
	}
}
