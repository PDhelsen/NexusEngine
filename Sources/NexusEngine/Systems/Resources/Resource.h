#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class NX_ENGINE_API Resource
	{
		friend class ResourcesSystem;

	public:
		Resource(NxFr::StringView Path);
		virtual ~Resource();

		NxFr::StringView GetPath() const { return Path; }
		NxFr::StringView GetName() const { return NxFr::Path::GetName(Path); }

	protected:
		virtual void Save(NxFr::StringView FilePath) {};
		virtual void Load(NxFr::StringView FilePath) {};
		virtual void Unload() {};

	private:
		NxFr::String Path;
	};
}


