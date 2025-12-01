#pragma once

#include "NexusEngine/Application/Object.h"

namespace NxEn
{
	class Asset : public Object
	{
		friend class AssetsSystem;
		friend class AssetsManager;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Asset)

		NEXUS_ENGINE_API Asset();
		NEXUS_ENGINE_API virtual ~Asset();

		NEXUS_ENGINE_API NxFr::GUID GetId() const { return Id; }
		NEXUS_ENGINE_API bool IsDirty() const { return Dirty; }
		NEXUS_ENGINE_API void SetDirty() { Dirty = true; }

	private:
		NEXUS_ENGINE_API void Load(YAML::Node& Node, NxFr::StringView Content);
		NEXUS_ENGINE_API void Unload();
		NEXUS_ENGINE_API void Save(YAML::Node& Node, NxFr::StringView Content);

		NEXUS_ENGINE_API virtual void OnLoad(YAML::Node& Node, NxFr::StringView Content) = 0;
		NEXUS_ENGINE_API virtual void OnUnload() = 0;
		NEXUS_ENGINE_API virtual void OnSave(YAML::Node& Node, NxFr::StringView Content) = 0;

	private:
		NxFr::GUID Id;
		bool Dirty;
	};
}
