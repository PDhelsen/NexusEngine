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

	protected:
		NEXUS_ENGINE_API virtual void OnSave(YAML::Node& Node, NxFr::StringView Content) = 0;
		NEXUS_ENGINE_API virtual void OnLoad(const YAML::Node& Node, NxFr::StringView Content) = 0;
		NEXUS_ENGINE_API virtual void OnUnload() = 0;

		NEXUS_ENGINE_API virtual NxFr::Array<NxFr::GUID> GetDependencies() = 0;

	private:
		NEXUS_ENGINE_API YAML::Node Save(NxFr::StringView Content);
		NEXUS_ENGINE_API void Load(const YAML::Node& Node, NxFr::StringView Content);
		NEXUS_ENGINE_API void Unload();

	private:
		NxFr::GUID Id;
		bool Dirty;
	};
}
