#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Object/Object.h"

namespace NxEn
{
	class NX_ENGINE_API Setting : public Object
	{
	public:
		Setting(NxFr::StringView Page, NxFr::StringView Name);
		virtual ~Setting();

		virtual void Set(NxFr::StringView Value);
		virtual void Set(uint64 Index, NxFr::StringView Value);
		virtual void Set(NxFr::StringView Key, NxFr::StringView Value);
		virtual NxFr::String Get();
		virtual NxFr::String Get(uint64 Index);
		virtual NxFr::String Get(NxFr::StringView Key);

		NxFr::GUID GetId() const override { return Id.GetId(); }
		NxFr::StringView GetKey() const { return Id.GetString(); }
		NxFr::StringView GetPage() const { return Page; }
		NxFr::StringView GetName() const override { return Name; }

	protected:
		virtual void OnDraw() override = 0;
		virtual void OnSerialize(NxFr::Yaml::Node& Node) const override = 0;
		virtual void OnDeserialize(const NxFr::Yaml::Node& Node) override = 0;

	private:
		NxFr::StringId Id;
		NxFr::StringView Page;
		NxFr::StringView Name;
	};
}
