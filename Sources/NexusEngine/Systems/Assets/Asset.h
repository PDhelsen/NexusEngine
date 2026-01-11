#pragma once

#include "NexusEngine/Application/Object.h"
#include "NexusEngine/Systems/Assets/AssetsFactory.h"

#define NEXUS_ASSET_DECLARATION(Dll, Type) \
NEXUS_OBJECT_DECLARATION(Dll, Type)

#define NEXUS_ASSET_IMPLEMENTATION(Type) \
NEXUS_OBJECT_IMPLEMENTATION(Type)\
NxEn::AssetsFactory::Factory* Factory##Type = NxEn::AssetsFactory::Register<Type>();

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

		NEXUS_ENGINE_API YAML::Node Save(NxFr::StringView Content);
		NEXUS_ENGINE_API void Load(const YAML::Node& Node, NxFr::StringView Content);
		NEXUS_ENGINE_API void Unload() override;

		NEXUS_ENGINE_API NxFr::GUID GetId() const override { return Id; }
		NEXUS_ENGINE_API bool IsDirty() const { return Dirty; }
		NEXUS_ENGINE_API void SetDirty() { Dirty = true; }

	protected:
		NEXUS_ENGINE_API virtual void OnSave(YAML::Node& Node, NxFr::StringView ContentFilePath) = 0;
		NEXUS_ENGINE_API virtual void OnLoad(const YAML::Node& Node, NxFr::StringView ContentFilePath) = 0;
		NEXUS_ENGINE_API virtual void OnUnload() = 0;

	private:
		NEXUS_ENGINE_API YAML::Node Save() override { NEXUS_ASSERT(false, System, "Use the version with the Content arg !"); return YAML::Node(); }
		NEXUS_ENGINE_API void Load(const YAML::Node& Node) override { NEXUS_ASSERT(false, System, "Use the version with the Content arg !"); }
		NEXUS_ENGINE_API void OnSave(YAML::Node& Node) override { NEXUS_ASSERT(false, System, "Use the version with the Content arg !"); }
		NEXUS_ENGINE_API void OnLoad(const YAML::Node& Node) override { NEXUS_ASSERT(false, System, "Use the version with the Content arg !"); }

	private:
		NxFr::GUID Id;
		bool Dirty;
	};
}
