#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	struct AssetHandle
	{
		friend class AssetsSystem;
		friend class AssetsManager;

	public:
		NEXUS_ENGINE_API AssetHandle(Asset* Instance);
		NEXUS_ENGINE_API ~AssetHandle();

		NEXUS_ENGINE_API Asset* GetInstance() { return Instance; }
		NEXUS_ENGINE_API uint64 GetCount() { return Count; }
		NEXUS_ENGINE_API bool IsUsed() const { return Count > 0; }

	private:
		void Acquire();
		void Release();

	private:
		Asset* Instance;
		uint64 Count;
	};

	struct AssetMetadata
	{
		friend class AssetsSystem;
		friend class AssetsRegistry;

	public:
		static inline const NxFr::String AssetExtension = "asset";

		NEXUS_ENGINE_API AssetMetadata();
		NEXUS_ENGINE_API AssetMetadata(Asset* Instance, NxFr::StringView Path = "", NxFr::StringView Extension = "");
		NEXUS_ENGINE_API ~AssetMetadata();

		NEXUS_ENGINE_API NxFr::StringView GetName() const { return NxFr::Path::Split(Path.Data).Last(); }
		NEXUS_ENGINE_API NxFr::GUID GetId() const { return Id; }
		NEXUS_ENGINE_API NxFr::StringId GetType() const { return Type; }
		NEXUS_ENGINE_API bool HasFile() const { return !Path.IsEmpty(); }
		NEXUS_ENGINE_API NxFr::StringView GetPath() const { return Path; }
		NEXUS_ENGINE_API NxFr::StringView GetExtension() const { return Extension; }
		NEXUS_ENGINE_API const NxFr::Array<NxFr::GUID>& GetDependencies() const { return Dependencies; }
		NEXUS_ENGINE_API NxFr::Dictionary<NxFr::String, NxFr::String>& GetData() { return Data; }

	private:
		NxFr::String GetAssetPath() const { return ComputeAssetPath(Path); }
		NxFr::String GetContentPath() const { return ComputeContentPath(Path, Extension); }

		static NxFr::String ComputeAssetPath(NxFr::StringView Path) { return Path + "." + AssetExtension; }
		static NxFr::String ComputeContentPath(NxFr::StringView Path, NxFr::StringView Extension) { return Path + "." + Extension; }

	private:
		YAML::Node Serialize();
		void Deserialize(const YAML::Node& Node);

	private:
		NxFr::GUID Id;
		NxFr::StringId Type;
		NxFr::Path Path;
		NxFr::String Extension;
		NxFr::Array<NxFr::GUID> Dependencies;
		NxFr::Dictionary<NxFr::String, NxFr::String> Data;
	};	
}
