#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class NX_ENGINE_API Setting
	{
	public:
		Setting(NxFr::StringView Page, NxFr::StringView Name);
		virtual ~Setting();

		virtual void OnDraw() = 0;
		virtual void OnSerialize(YAML::Node& Node) = 0;
		virtual void OnDeserialize(const YAML::Node& Node) = 0;

		virtual void Set(NxFr::StringView Value);
		virtual void Set(uint64 Index, NxFr::StringView Value);
		virtual void Set(NxFr::StringView Key, NxFr::StringView Value);
		virtual NxFr::String Get();
		virtual NxFr::String Get(uint64 Index);
		virtual NxFr::String Get(NxFr::StringView Key);

		NxFr::GUID GetId() const { return Id.GetId(); }
		NxFr::StringView GetKey() const { return Id.GetString(); }
		NxFr::StringView GetPage() const { return Page; }
		NxFr::StringView GetName() const { return Name; }

	private:
		NxFr::StringId Id;
		NxFr::StringView Page;
		NxFr::StringView Name;
	};
}
