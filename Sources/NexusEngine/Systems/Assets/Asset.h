#pragma once

#include "NexusEngine/Application/Object.h"
#include "NexusEngine/Systems/Assets/AssetsFactory.h"

#define NX_ASSET_DECLARATION(Dll, Type) \
NX_OBJECT_DECLARATION(Dll, Type)

#define NX_ASSET_IMPLEMENTATION(Type) \
NX_OBJECT_IMPLEMENTATION(Type)\
NxEn::AssetsFactory::Factory* Factory##Type = NxEn::AssetsFactory::Register<Type>();

namespace NxEn
{
	class Asset : public Object
	{
		friend class AssetsSystem;
		friend class AssetsManager;

	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, Asset)

		NX_ENGINE_API Asset();
		NX_ENGINE_API virtual ~Asset();

		NX_ENGINE_API YAML::Node Save(NxFr::StringView Content);
		NX_ENGINE_API void Load(const YAML::Node& Node, NxFr::StringView Content);
		NX_ENGINE_API void Unload() override;

		NX_ENGINE_API NxFr::GUID GetId() const override { return Id; }
		NX_ENGINE_API bool IsDirty() const { return Dirty; }
		NX_ENGINE_API void SetDirty() { Dirty = true; }

	protected:
		NX_ENGINE_API virtual void OnGui(float TimeStep) override;
		NX_ENGINE_API virtual void OnSave(YAML::Node& Node, NxFr::StringView ContentFsPath) = 0;
		NX_ENGINE_API virtual void OnLoad(const YAML::Node& Node, NxFr::StringView ContentFsPath) = 0;
		NX_ENGINE_API virtual void OnUnload() = 0;

	private:
		NX_ENGINE_API YAML::Node Save() override { NX_ASSERT(false, System, "Use the version with the Content arg !"); return YAML::Node(); }
		NX_ENGINE_API void Load(const YAML::Node& Node) override { NX_ASSERT(false, System, "Use the version with the Content arg !"); }
		NX_ENGINE_API void OnSave(YAML::Node& Node) override { NX_ASSERT(false, System, "Use the version with the Content arg !"); }
		NX_ENGINE_API void OnLoad(const YAML::Node& Node) override { NX_ASSERT(false, System, "Use the version with the Content arg !"); }

	private:
		NxFr::GUID Id;
		bool Dirty;
	};
}
