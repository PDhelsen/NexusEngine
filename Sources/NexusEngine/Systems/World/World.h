#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Object/Object.h"
#include "NexusEngine/Systems/World/GameObject.h"

namespace NxEn
{
	class NX_ENGINE_API World : public Object
	{
		friend class WorldManager;

	public:
		NX_OBJECT(World)

		World();
		~World();

		NxFr::Handle<GameObject> GetRoot() const;

		NxFr::GUID GetId() const override;
		NxFr::GUID GetWorldId() const;
		NxFr::StringView GetName() const override;

	protected:
		void OnTick(float TimeStep) override;

	private:
		NxFr::StringId WorldId;
		NxFr::Handle<GameObject> Root;
	};
}
