#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class Resource
	{
		friend class ResourcesSystem;

	public:
		NX_ENGINE_API Resource(NxFr::StringView Path);
		NX_ENGINE_API virtual ~Resource();

		NX_ENGINE_API NxFr::StringView GetPath() const { return Path; }
		NX_ENGINE_API NxFr::StringView GetName() const { return NxFr::Path::GetName(Path); }
		NX_ENGINE_API bool IsLoaded() const { return Loaded; };

	protected:
		NX_ENGINE_API virtual void Save(NxFr::StringView FilePath);
		NX_ENGINE_API virtual void Load(NxFr::StringView FilePath);
		NX_ENGINE_API virtual void Unload();

	private:
		NxFr::String Path;
		bool Loaded;
	};
}


