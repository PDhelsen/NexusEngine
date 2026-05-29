#pragma once

#include "NexusEngine/Application/Object/Object.h"

#define NX_COMPONENT_DECLARATION(Type) \
NX_OBJECT(Type)

#define NX_COMPONENT_IMPLEMENTATION(Type) \
NxEn::WorldObjectStorage* Storage##Type = NxEn::WorldObjectStorage::Register<Type>();

namespace NxEn
{
	class NX_ENGINE_API Component : public Object
	{
		friend class WorldObjectFactory;
		friend class GameObject;

		const static inline uint8 ObjectFlag_EnabledInHierarchy = (uint8)ObjectFlags::Flag_1;

	public:
		NX_OBJECT(Component)

		Component();
		virtual ~Component();

		void SetEnabled(bool Enabled) override;
		bool IsEnabledInHierarchy() const;
		bool IsTicking() const override;

		void PatchReferences();
		void UpdateHierarchy();

		NxFr::StringView GetName() const override;
		NxFr::GUID GetId() const override;
		NxFr::Handle<GameObject> GetGameObject() const;

	protected:
		void OnGui(float TimeStep) override;
		void OnClone(const Object& Other) override;
		void OnSave(YAML::Node& Node) override;
		void OnLoad(const YAML::Node& Node) override;

		virtual void OnPatchReferences();
		virtual void OnUpdateHierarchy();

	private:
		NxFr::GUID ComponentId;
		NxFr::Handle<GameObject> Target;
	};
}
