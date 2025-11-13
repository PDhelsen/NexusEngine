#pragma once

#include "NexusEngine/Application/Object.h"

namespace NxEn
{
	class Asset : public Object
	{
		friend class AssetsSystem;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Asset)

		NEXUS_ENGINE_API NxFr::GUID GetId() const { return Id; }

	private:
		NEXUS_ENGINE_API Asset(NxFr::GUID Id);
		NEXUS_ENGINE_API ~Asset();

		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnEnable() override;
		NEXUS_ENGINE_API void OnDisable() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep) override;

		NEXUS_ENGINE_API void OnSerialize(YAML::Node& Node);
		NEXUS_ENGINE_API void OnDeserialize(const YAML::Node& Node);

	private:
		NxFr::GUID Id;
	};
}
