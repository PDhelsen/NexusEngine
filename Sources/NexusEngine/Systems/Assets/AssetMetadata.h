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
		static inline const NxFr::String Extension = "asset";

		NEXUS_ENGINE_API AssetMetadata();
		NEXUS_ENGINE_API AssetMetadata(Asset* Instance, NxFr::StringView Path = "", NxFr::StringView Extension = "");
		NEXUS_ENGINE_API ~AssetMetadata();

		NEXUS_ENGINE_API NxFr::GUID GetId() { return Id; }
		NEXUS_ENGINE_API NxFr::StringId GetType() { return Type; }
		NEXUS_ENGINE_API const NxFr::Path& GetPath() const { return Path; }
		NEXUS_ENGINE_API const NxFr::Path& GetContent() const { return Content; }
		NEXUS_ENGINE_API const NxFr::Array<NxFr::GUID>& GetDependencies() { return Dependencies; }
		NEXUS_ENGINE_API NxFr::Dictionary<NxFr::String, NxFr::String>& GetData() { return Data; }

	private:
		YAML::Node Serialize();
		void Deserialize(const YAML::Node& Node);

	private:
		NxFr::GUID Id;
		NxFr::StringId Type;
		NxFr::Path Path;
		NxFr::Path Content;
		NxFr::Array<NxFr::GUID> Dependencies;
		NxFr::Dictionary<NxFr::String, NxFr::String> Data;
	};	
}
