#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class Asset;

	struct NX_ENGINE_API AssetHandle
	{
		friend class AssetsSystem;
		friend class AssetsManager;

	public:
		AssetHandle(Asset* Instance);
		~AssetHandle();

		Asset* GetInstance() { return Instance; }
		uint64 GetCount() { return Count; }
		bool IsUsed() const { return Count > 0; }

	private:
		void Acquire();
		void Release();

		Asset* Instance;
		uint64 Count;
	};

	struct NX_ENGINE_API AssetMetadata
	{
		friend class AssetsSystem;
		friend class AssetsRegistry;

	public:
		static inline const NxFr::String AssetExtension = "asset";

		AssetMetadata();
		~AssetMetadata();

		NxFr::GUID GetId() const { return Id; }
		NxFr::StringId GetType() const { return Type; }
		NxFr::StringView GetName() const { return Name; }
		bool HasFile() const { return !Path.IsEmpty(); }
		NxFr::StringView GetPath() const { return Path; }
		NxFr::StringView GetExtension() const { return Extension; }
		const NxFr::Array<NxFr::GUID>& GetDependencies() const { return Dependencies; }
		NxFr::Dictionary<NxFr::String, NxFr::String>& GetData() { return Data; }

	private:
		NxFr::Yaml::Node Serialize() const;
		void Deserialize(const NxFr::Yaml::Node& Node);

		NxFr::GUID Id;
		NxFr::StringId Type;
		NxFr::String Name;
		NxFr::String Path;
		NxFr::String Extension;
		NxFr::Array<NxFr::GUID> Dependencies;
		NxFr::Dictionary<NxFr::String, NxFr::String> Data;
	};	
}
