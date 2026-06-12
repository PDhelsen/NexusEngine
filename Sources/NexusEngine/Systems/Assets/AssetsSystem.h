#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Assets/Asset.h"
#include "NexusEngine/Systems/Assets/AssetInfo.h"

namespace NxEn
{
	class NX_ENGINE_API AssetsSystem : public System
	{
	public:
		inline static const NxFr::StringId EventCreatedId = "Created"_Sid;
		inline static const NxFr::StringId EventMovedId = "Renamed"_Sid;
		inline static const NxFr::StringId EventCopiedId = "Copied"_Sid;
		inline static const NxFr::StringId EventDeletedId = "Deleted"_Sid;
		inline static const NxFr::StringId EventSaveId = "Save"_Sid;
		inline static const NxFr::StringId EventSavedId = "Saved"_Sid;
		inline static const NxFr::StringId EventLoadedId = "Loaded"_Sid;
		inline static const NxFr::StringId EventUnloadedId = "Unloaded"_Sid;
		inline static const NxFr::StringId EventImportedId = "Imported"_Sid;

		static NxFr::Factory<Asset>& GetFactory();

		NX_OBJECT(AssetsSystem)

		template<typename T>
		T* Create(NxFr::StringView Path = "", NxFr::StringView Extension = "")
		{
			return static_cast<T*>(Create(T::GetClassType(), Path, Extension));
		}
		template<typename T>
		T* Acquire(NxFr::GUID Id)
		{
			return static_cast<T*>(Acquire(Id));
		}
		template<typename T>
		T* Load(NxFr::GUID Id)
		{
			return static_cast<T*>(Load(Id));
		}
		template<typename T>
		T* Import(const YAML::Node& Node, NxFr::StringView Path, NxFr::StringView Extension)
		{
			return static_cast<T*>(Import(T::GetClassType(), Node, Path, Extension));
		}
		template<typename T>
		T* Reimport(const YAML::Node& Node, NxFr::GUID Id)
		{
			return static_cast<T*>(Reimport(Id, Node));
		}
		template<typename T>
		T* GetAsset(NxFr::GUID Id)
		{
			return static_cast<T*>(GetAsset(Id));
		}

		Asset* Create(NxFr::StringId Type, NxFr::StringView Path = "", NxFr::StringView Extension = "");
		void Rename(NxFr::GUID Id, NxFr::StringView Name);
		void Move(NxFr::GUID Id, NxFr::StringView Path);
		void Copy(NxFr::GUID Id, NxFr::StringView Path);
		void Delete(NxFr::GUID Id);
		void Save(NxFr::GUID Id, bool Force = false);
		void SaveMetadata(NxFr::GUID Id);
		void SaveDirty();

		void Track(Asset* Instance, NxFr::StringView Path = "", NxFr::StringView Extension = "");
		Asset* Clone(NxFr::GUID Id);
		Asset* Acquire(NxFr::GUID Id);
		void Release(NxFr::GUID Id, bool Keep = false);
		Asset* Load(NxFr::GUID Id);
		void Reload(NxFr::GUID Id);
		void Unload(NxFr::GUID Id);
		void Purge(bool SaveIfDirty);

		Asset* Import(NxFr::StringId Type, const YAML::Node& Node, NxFr::StringView Path, NxFr::StringView Extension);
		Asset* Reimport(NxFr::GUID Id, const YAML::Node& Node);

		Asset* GetAsset(NxFr::GUID Id);
		AssetHandle& GetHandle(NxFr::GUID Id);
		AssetMetadata& GetMetadata(NxFr::GUID Id);
		YAML::Node GetImportData(NxFr::GUID Id);
		NxFr::Array<NxFr::GUID> GetDependencies(NxFr::GUID Id, bool Recursive = false);
		void GetDependencies(NxFr::GUID Id, bool Recursive, NxFr::Set<NxFr::GUID>& Result);

		NxFr::Array<NxFr::GUID> Find(NxFr::StringView Filter) const;
		NxFr::GUID PathToId(NxFr::StringView Path) const;
		NxFr::String IdToPath(NxFr::GUID Id) const;

		bool IsTracked(NxFr::GUID Id) const;
		bool IsLoaded(NxFr::GUID Id) const;

		NxFr::Event<NxFr::StringId, NxFr::GUID>& GetOnEvent() { return OnEvent; }

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnTick(float TimeStep = 0.0f) override;

	private:
		NxFr::Event<NxFr::StringId, NxFr::GUID> OnEvent;

		class AssetsRegistry* Registry;
		class AssetsManager* Manager;
	};
}
