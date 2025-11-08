#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class Resource
	{
		friend class ResourcesSystem;

	public:
		NEXUS_ENGINE_API Resource(NxFr::StringView Path);
		NEXUS_ENGINE_API virtual ~Resource();

		NEXUS_ENGINE_API NxFr::StringView GetPath() const { return Path; }
		NEXUS_ENGINE_API NxFr::StringView GetName() const { return NxFr::Path::GetFileName(Path); }
		NEXUS_ENGINE_API bool IsLoaded() const { return Loaded; };

	protected:
		NEXUS_ENGINE_API virtual void Save(NxFr::StringView FilePath);
		NEXUS_ENGINE_API virtual void Load(NxFr::StringView FilePath);
		NEXUS_ENGINE_API virtual void Unload();

	private:
		NxFr::String Path;
		bool Loaded;
	};
}


