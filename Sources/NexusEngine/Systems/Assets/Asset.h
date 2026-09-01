#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Object/Object.h"

#define NX_ASSET(Type) static NxFr::Factory<NxEn::Asset>::Creator* Factory##Type = NxEn::AssetsSystem::GetFactory().Register<Type>(Type::GetClassType());

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

		Asset* Clone() const override;

		NxFr::GUID GetId() const override { return Id; }
		NxFr::StringView GetName() const override { return Name; }
		bool IsDirty() const { return Dirty; }
		void SetDirty() { Dirty = true; }

	protected:
		void OnDraw() override;

	private:
		NxFr::GUID Id;
		NxFr::String Name;
		bool Dirty;
	};
}
