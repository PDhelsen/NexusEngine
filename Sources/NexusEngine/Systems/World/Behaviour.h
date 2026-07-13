#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Object/Object.h"

namespace NxEn
{
	class GameObject;

	class NX_ENGINE_API Behaviour : public Object
	{
		friend class WorldManager;

	public:
		NX_OBJECT(Behaviour)

		Behaviour();
		virtual ~Behaviour();

		bool IsTicking() const override;

		NxFr::StringView GetName() const override;
		NxFr::GUID GetId() const override;
		NxFr::Handle<GameObject> GetGameObject() const;

	protected:
		void OnUpdateHierarchy() override;
		void OnDraw() override;
		void OnClone(const Object& Other) override;

	private:
		NxFr::GUID BehaviourId;
		NxFr::Handle<GameObject> Target;
	};
}
