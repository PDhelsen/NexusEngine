#pragma once

#include "NexusEngine/Systems/World/Component/Component.h"

namespace NxEn
{
	class Transform : public Component
	{
	public:
		NEXUS_COMPONENT_DECLARATION(NEXUS_ENGINE_API, Transform)

		NEXUS_ENGINE_API Transform();
		NEXUS_ENGINE_API ~Transform();

	protected:
		NEXUS_ENGINE_API void OnGui(float TimeStep) override;
		NEXUS_ENGINE_API void OnClone(const Object& Other) override;
		NEXUS_ENGINE_API void OnSave(YAML::Node& Node) override;
		NEXUS_ENGINE_API void OnLoad(const YAML::Node& Node) override;
		NEXUS_ENGINE_API void OnUnload() override;

	private:
		NxFr::Vector3f Position;
		NxFr::Quaternion Rotation;
		NxFr::Vector3f Scale;
	};
}

