#pragma once

#include "NexusEngine/Application/Object.h"

namespace NxEn
{
	class Asset : public Object
	{
		friend struct AssetMetadata;
		friend class AssetsSystem;
		friend class AssetsRegistry;
		friend class AssetsManager;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Asset)

		NEXUS_ENGINE_API NxFr::GUID GetId() const { return Id; }
		NEXUS_ENGINE_API bool IsDirty() const { return Dirty; }
		NEXUS_ENGINE_API void SetDirty() { Dirty = true; }

	private:
		NEXUS_ENGINE_API Asset();
		NEXUS_ENGINE_API ~Asset();

		NEXUS_ENGINE_API void Load(YAML::Node& Node);
		NEXUS_ENGINE_API void Unload();
		NEXUS_ENGINE_API void Save(YAML::Node& Node);

		NEXUS_ENGINE_API void OnInitialize() override {};
		NEXUS_ENGINE_API void OnShutdown() override {};
		NEXUS_ENGINE_API void OnEnable() override {};
		NEXUS_ENGINE_API void OnDisable() override {};
		NEXUS_ENGINE_API void OnTick(float TimeStep) override {};
		NEXUS_ENGINE_API virtual void OnLoad(YAML::Node& Node) {};
		NEXUS_ENGINE_API virtual void OnUnload() {};
		NEXUS_ENGINE_API virtual void OnSave(YAML::Node& Node) {};

	private:
		NxFr::GUID Id;
		bool Dirty;
	};
}
