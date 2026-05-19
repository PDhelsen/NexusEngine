#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Styles.h"

namespace NxEn
{
	enum class SettingMode
	{
		Var, Seq, Map
	};

	class NX_ENGINE_API Setting
	{
	public:

		static NxFr::String Key(NxFr::StringView Page, NxFr::StringView Name)
		{
			return Page + "." + Name;
		}

		virtual void OnGui(const GUI::Style* Visual = nullptr) = 0;
		virtual void OnDeserialize(const YAML::Node & Node) = 0;
		virtual void OnSerialize(YAML::Node& Node) const = 0;

		virtual void Set(NxFr::StringView Value);
		virtual void Set(uint64 Index, NxFr::StringView Value);
		virtual void Set(NxFr::StringView Key, NxFr::StringView Value);
		virtual NxFr::String Get();
		virtual NxFr::String Get(uint64 Index);
		virtual NxFr::String Get(NxFr::StringView Key);

		NxFr::StringView GetId() const { return Id; }
		NxFr::StringView GetPage() const { return NxFr::StringUtility::Split(Id, ".", 0); }
		NxFr::StringView GetName() const { return NxFr::StringUtility::Split(Id, ".", 1); }
		SettingMode GetMode() const { return Mode; }

	protected:
		Setting(NxFr::StringView Page, NxFr::StringView Name, SettingMode Mode);
		virtual ~Setting();

	private:
		const NxFr::String Id;
		const SettingMode Mode;
	};
}
