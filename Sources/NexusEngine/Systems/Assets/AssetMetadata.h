#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	struct AssetHandle
	{
		friend class AssetsSystem;
		friend class AssetsManager;

	public:
		NX_ENGINE_API AssetHandle(Asset* Instance);
		NX_ENGINE_API ~AssetHandle();

		NX_ENGINE_API Asset* GetInstance() { return Instance; }
		NX_ENGINE_API uint64 GetCount() { return Count; }
		NX_ENGINE_API bool IsUsed() const { return Count > 0; }

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

		NX_ENGINE_API AssetMetadata();
		NX_ENGINE_API AssetMetadata(Asset* Instance, NxFr::StringView Path = "", NxFr::StringView Extension = "");
		NX_ENGINE_API ~AssetMetadata();

		NX_ENGINE_API NxFr::StringView GetName() const { return NxFr::Path::GetName(Path); }
		NX_ENGINE_API NxFr::GUID GetId() const { return Id; }
		NX_ENGINE_API NxFr::StringId GetType() const { return Type; }
		NX_ENGINE_API bool HasFile() const { return !Path.IsEmpty(); }
		NX_ENGINE_API NxFr::StringView GetPath() const { return Path; }
		NX_ENGINE_API NxFr::String GetAssetPath() const { return ComputeAssetPath(Path); }
		NX_ENGINE_API NxFr::String GetContentPath() const { return ComputeContentPath(Path, Extension); }
		NX_ENGINE_API NxFr::StringView GetExtension() const { return Extension; }
		NX_ENGINE_API const NxFr::Array<NxFr::GUID>& GetDependencies() const { return Dependencies; }
		NX_ENGINE_API NxFr::Dictionary<NxFr::String, NxFr::String>& GetData() { return Data; }

		NX_ENGINE_API static NxFr::String ComputeAssetPath(NxFr::StringView Path) { return NxFr::Path::ChangeExtension(Path, AssetExtension); }
		NX_ENGINE_API static NxFr::String ComputeContentPath(NxFr::StringView Path, NxFr::StringView Extension) { return NxFr::Path::ChangeExtension(Path, Extension); }

	private:
		YAML::Node Serialize();
		void Deserialize(const YAML::Node& Node);

	private:
		NxFr::GUID Id;
		NxFr::StringId Type;
		NxFr::String Path;
		NxFr::String Extension;
		NxFr::Array<NxFr::GUID> Dependencies;
		NxFr::Dictionary<NxFr::String, NxFr::String> Data;
	};	
}
