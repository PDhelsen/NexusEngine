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
			Asset* Instance;
			NxFr::Path Path;
			uint64 Count;

			NEXUS_ENGINE_API Info();
		};

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, AssetsSystem)

		NEXUS_ENGINE_API Asset* Get(NxFr::StringView Path, bool CreateIfDontExist = true);
		NEXUS_ENGINE_API void Release(NxFr::StringView Path);

		NEXUS_ENGINE_API Asset* Create(NxFr::StringView Path);
		NEXUS_ENGINE_API void Delete(NxFr::StringView Path);
		NEXUS_ENGINE_API void Move(NxFr::StringView Path, NxFr::StringView Target);

		NEXUS_ENGINE_API void Save(NxFr::StringView Path);
		NEXUS_ENGINE_API void SaveAll();

		NEXUS_ENGINE_API Asset* Load(NxFr::StringView Path);
		NEXUS_ENGINE_API void Unload(NxFr::StringView Path);
		NEXUS_ENGINE_API void Purge();

		NEXUS_ENGINE_API NxFr::List<NxFr::StringView> Find(NxFr::String Filter);

	private:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

		NEXUS_ENGINE_API void LoadDatabase();
		NEXUS_ENGINE_API void SaveDatabase();

		NEXUS_ENGINE_API Info* GetAsset(NxFr::StringView Path);
		NEXUS_ENGINE_API NxFr::Path GetFilePath(NxFr::StringView Path);

	private:
		NxFr::Dictionary<NxFr::String, Info> Assets;
	};
}
