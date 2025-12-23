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
		const NxFr::StringId EventCreatedId = "Created"_Sid;
		const NxFr::StringId EventMovedId = "Renamed"_Sid;
		const NxFr::StringId EventCopiedId = "Copied"_Sid;
		const NxFr::StringId EventDeletedId = "Deleted"_Sid;
		const NxFr::StringId EventSavedId = "Saved"_Sid;
		const NxFr::StringId EventLoadedId = "Loaded"_Sid;
		const NxFr::StringId EventUnloadedId = "Unloaded"_Sid;
		const NxFr::StringId EventImportedId = "Imported"_Sid;

		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, AssetsSystem)

		template<typename T>
		T* Create(NxFr::StringView Path = "", NxFr::StringView Extension = "");
		template<typename T>
		T* Acquire(NxFr::GUID Id);
		template<typename T>
		T* Load(NxFr::GUID Id);
		template<typename T>
		T* Import(const YAML::Node& Node, NxFr::StringView Path, NxFr::StringView Extension);
		template<typename T>
		T* Reimport(const YAML::Node& Node, NxFr::GUID Id);
		template<typename T>
		T* GetAsset(NxFr::GUID Id);

		NEXUS_ENGINE_API Asset* Create(NxFr::StringId Type, NxFr::StringView Path = "", NxFr::StringView Extension = "");
		NEXUS_ENGINE_API void Rename(NxFr::GUID Id, NxFr::StringView Name);
		NEXUS_ENGINE_API void Move(NxFr::GUID Id, NxFr::StringView Path);
		NEXUS_ENGINE_API void Copy(NxFr::GUID Id, NxFr::StringView Path);
		NEXUS_ENGINE_API void Delete(NxFr::GUID Id);
		NEXUS_ENGINE_API void Save(NxFr::GUID Id, bool Force = false);
		NEXUS_ENGINE_API void Save();

		NEXUS_ENGINE_API void Track(Asset* Instance, NxFr::StringView Path = "", NxFr::StringView Extension = "");
		NEXUS_ENGINE_API Asset* Acquire(NxFr::GUID Id);
		NEXUS_ENGINE_API void Release(NxFr::GUID Id, bool Keep = false);
		NEXUS_ENGINE_API Asset* Load(NxFr::GUID Id);
		NEXUS_ENGINE_API void Reload(NxFr::GUID Id);
		NEXUS_ENGINE_API void Unload(NxFr::GUID Id);
		NEXUS_ENGINE_API void Purge(bool SaveIfDirty);

		NEXUS_ENGINE_API Asset* Import(NxFr::StringId Type, const YAML::Node& Node, NxFr::StringView Path, NxFr::StringView Extension);
		NEXUS_ENGINE_API Asset* Reimport(NxFr::GUID Id, const YAML::Node& Node);

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

		NEXUS_ENGINE_API Asset* Reset(NxFr::GUID Id);
		NEXUS_ENGINE_API void FetchDependencies(NxFr::GUID Id, bool Recusive, NxFr::Set<NxFr::GUID>& Result);

	private:
		NxFr::Event<NxFr::StringId, NxFr::GUID> OnEvent;
		NxFr::Event<Asset*> OnSave;

		class AssetsRegistry* Registry;
		class AssetsManager* Manager;
	};

	template<typename T>
	inline T* AssetsSystem::Create(NxFr::StringView Path, NxFr::StringView Extension)
	{
		return static_cast<T*>(Create(T::GetClassType(), Path, Extension));
	}

	template<typename T>
	inline T* AssetsSystem::Acquire(NxFr::GUID Id)
	{
		return static_cast<T*>(Acquire(Id));
	}

	template<typename T>
	inline T* AssetsSystem::Load(NxFr::GUID Id)
	{
		return static_cast<T*>(Load(Id));
	}

	template<typename T>
	inline T* AssetsSystem::Import(const YAML::Node& Node, NxFr::StringView Path, NxFr::StringView Extension)
	{
		return static_cast<T*>(Import(T::GetClassType(), Node, Path, Extension));
	}

	template<typename T>
	inline T* AssetsSystem::Reimport(const YAML::Node& Node, NxFr::GUID Id)
	{
		return static_cast<T*>(Reimport(Id, Node));
	}

	template<typename T>
	inline T* AssetsSystem::GetAsset(NxFr::GUID Id)
	{
		return static_cast<T*>(GetAsset(Id));
	}
}
