#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Object/Object.h"
#include "NexusEngine/Systems/Assets/AssetsFactory.h"

#define NX_ASSET_DECLARATION(Type) \
NX_OBJECT(Type)

#define NX_ASSET_IMPLEMENTATION(Type) \
NxEn::AssetsFactory::Factory* Factory##Type = NxEn::AssetsFactory::Register<Type>();

namespace NxEn
{
	class NX_ENGINE_API Asset : public Object
	{
		friend class AssetsSystem;
		friend class AssetsManager;

	public:
		NX_OBJECT(Asset)

		Asset();
		virtual ~Asset();

		void Save(YAML::Node& Node, NxFr::StringView Content);
		void Load(const YAML::Node& Node, NxFr::StringView Content);
		void Unload() override;

		NxFr::GUID GetId() const override { return Id; }
		NxFr::StringView GetName() const override { return Name; }
		bool IsDirty() const { return Dirty; }
		void SetDirty() { Dirty = true; }

	protected:
		virtual void OnDraw() override;
		virtual void OnSave(YAML::Node& Node, NxFr::StringView ContentFsPath) = 0;
		virtual void OnLoad(const YAML::Node& Node, NxFr::StringView ContentFsPath) = 0;
		virtual void OnUnload() = 0;

	private:
		NxFr::GUID Id;
		NxFr::String Name;
		bool Dirty;
	};
}
