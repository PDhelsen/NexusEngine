#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Assets/Asset.h"
#include "NexusEngine/Systems/Assets/AssetMetadata.h"

namespace NxEn
{
	class NX_ENGINE_API AssetsSystem : public System
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

		NX_OBJECT_DECLARATION(AssetsSystem)

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

		Asset* Create(NxFr::StringId Type, NxFr::StringView Path = "", NxFr::StringView Extension = "");
		void Rename(NxFr::GUID Id, NxFr::StringView Name);
		void Move(NxFr::GUID Id, NxFr::StringView Path);
		void Copy(NxFr::GUID Id, NxFr::StringView Path);
		void Delete(NxFr::GUID Id);
		void Save(NxFr::GUID Id, bool Force = false);
		void SaveDirty();

		void Track(Asset* Instance, NxFr::StringView Path = "", NxFr::StringView Extension = "");
		Asset* Acquire(NxFr::GUID Id);
		void Release(NxFr::GUID Id, bool Keep = false);
		Asset* Load(NxFr::GUID Id);
		void Reload(NxFr::GUID Id);
		void Unload(NxFr::GUID Id);
		void Purge(bool SaveIfDirty);

		Asset* Import(NxFr::StringId Type, const YAML::Node& Node, NxFr::StringView Path, NxFr::StringView Extension);
		Asset* Reimport(NxFr::GUID Id, const YAML::Node& Node);

		NxFr::Array<NxFr::GUID> Find(NxFr::StringView Filter) const;
		NxFr::GUID PathToId(NxFr::StringView Path) const;
		NxFr::String IdToPath(NxFr::GUID Id) const;

		Asset* GetAsset(NxFr::GUID Id);
		AssetHandle& GetHandle(NxFr::GUID Id);
		AssetMetadata& GetMetadata(NxFr::GUID Id);
		YAML::Node GetImportData(NxFr::GUID Id);
		NxFr::Array<NxFr::GUID> GetDependencies(NxFr::GUID Id, bool Recusive = false);

		bool IsTracked(NxFr::GUID Id) const;
		bool IsLoaded(NxFr::GUID Id) const;

		NxFr::Event<NxFr::StringId, NxFr::GUID>& GetOnEvent() { return OnEvent; }
		NxFr::Event<Asset*>& GetOnSave() { return OnSave; }

	private:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnTick(float TimeStep = 0.0f) override;

		void RecordStats() const;
		void UpdateStats() const;

		Asset* Reset(NxFr::GUID Id);
		void FetchDependencies(NxFr::GUID Id, bool Recusive, NxFr::Set<NxFr::GUID>& Result);

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
