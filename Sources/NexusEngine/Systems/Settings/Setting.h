#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Styles.h"

namespace NxEn
{
	enum class SettingMode
	{
		Var, Seq, Map
	};

	class Setting
	{
	public:

		static NxFr::String Key(NxFr::StringView Page, NxFr::StringView Name)
		{
			return Page + "." + Name;
		}

		NX_ENGINE_API virtual void OnGui(const GUI::Style* Visual = nullptr) = 0;
		NX_ENGINE_API virtual void OnDeserialize(const YAML::Node & Node) = 0;
		NX_ENGINE_API virtual void OnSerialize(YAML::Node& Node) const = 0;

		NX_ENGINE_API virtual void Set(NxFr::StringView Value);
		NX_ENGINE_API virtual void Set(uint64 Index, NxFr::StringView Value);
		NX_ENGINE_API virtual void Set(NxFr::StringView Key, NxFr::StringView Value);
		NX_ENGINE_API virtual NxFr::String Get();
		NX_ENGINE_API virtual NxFr::String Get(uint64 Index);
		NX_ENGINE_API virtual NxFr::String Get(NxFr::StringView Key);

		NX_ENGINE_API NxFr::StringView GetId() const { return Id; }
		NX_ENGINE_API NxFr::StringView GetPage() const { return NxFr::StringUtility::Split(Id, ".", 0); }
		NX_ENGINE_API NxFr::StringView GetName() const { return NxFr::StringUtility::Split(Id, ".", 1); }
		NX_ENGINE_API SettingMode GetMode() const { return Mode; }

	protected:
		NX_ENGINE_API Setting(NxFr::StringView Page, NxFr::StringView Name, SettingMode Mode);
		NX_ENGINE_API virtual ~Setting();

	private:
		const NxFr::String Id;
		const SettingMode Mode;
	};
}
