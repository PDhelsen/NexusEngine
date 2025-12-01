#pragma once

#include "NexusEngine/Systems/Assets/Asset.h"

namespace NxEn
{
	struct AssetHandle
	{
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
		friend class AssetsRegistry;

	public:
		NEXUS_ENGINE_API AssetMetadata();
		NEXUS_ENGINE_API AssetMetadata(Asset* Instance, NxFr::StringView Path);
		NEXUS_ENGINE_API ~AssetMetadata();

		NEXUS_ENGINE_API NxFr::GUID GetId() { return Id; }
		NEXUS_ENGINE_API NxFr::StringId GetType() { return Type; }
		NEXUS_ENGINE_API const NxFr::Path& GetPath() const { return Path; }
		NEXUS_ENGINE_API NxFr::Dictionary<NxFr::String, NxFr::String>& GetData() { return Data; }

	private:
		void Serialize(YAML::Node& Node);
		void Deserialize(YAML::Node& Node);

	private:
		NxFr::GUID Id;
		NxFr::StringId Type;
		NxFr::Path Path;
		NxFr::Dictionary<NxFr::String, NxFr::String> Data;
	};	
}
