#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Assets/Asset.h"

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
		struct Info
		{
			NxFr::GUID Guid;
			NxFr::Path Path;
			uint64 Count;
			Asset* Instance;
		};

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, AssetsSystem)

		NEXUS_ENGINE_API Asset* Get(NxFr::GUID Guid, bool CreateIfDontExist = false);
		NEXUS_ENGINE_API void Release(NxFr::GUID Guid);

		NEXUS_ENGINE_API Asset* Create(NxFr::GUID Guid);
		NEXUS_ENGINE_API void Delete(NxFr::GUID Guid);
		NEXUS_ENGINE_API void Move(NxFr::GUID Guid, NxFr::StringView Target);
		NEXUS_ENGINE_API void Save(NxFr::GUID Guid);
		NEXUS_ENGINE_API void SaveAll();
		NEXUS_ENGINE_API Asset* Load(NxFr::GUID Guid);
		NEXUS_ENGINE_API void Unload(NxFr::GUID Guid);
		NEXUS_ENGINE_API void Purge();

		NEXUS_ENGINE_API NxFr::StringView GuidToPath(NxFr::GUID Guid);
		NEXUS_ENGINE_API NxFr::GUID PathToGuid(NxFr::StringView Path);
		NEXUS_ENGINE_API NxFr::List<NxFr::StringView> Find(NxFr::StringView Filter);

	private:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

		NEXUS_ENGINE_API void StatsRegister() const;
		NEXUS_ENGINE_API void StatsUpdate() const;

		NEXUS_ENGINE_API void LoadDatabase();
		NEXUS_ENGINE_API void SaveDatabase();

		NEXUS_ENGINE_API Info* GetAsset(NxFr::GUID Guid);
		NEXUS_ENGINE_API Info* AddAsset(NxFr::GUID Guid, NxFr::StringView Path = "");
		NEXUS_ENGINE_API void RemoveAsset(NxFr::GUID Guid);
		NEXUS_ENGINE_API void MoveAsset(NxFr::GUID Guid, NxFr::StringView Path);

		NEXUS_ENGINE_API static NxFr::Path GetFilePath(NxFr::StringView Path);

	private:
		NxFr::Dictionary<NxFr::GUID, Info> Assets;
		NxFr::Dictionary<NxFr::String, NxFr::GUID> Paths;
	};
}
