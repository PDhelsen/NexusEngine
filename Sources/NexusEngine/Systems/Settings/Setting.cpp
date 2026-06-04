#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Settings/Setting.h"

namespace NxEn
{
	Setting::Setting(NxFr::StringView Page, NxFr::StringView Name, SettingMode Mode)
		: Id(SettingsSystem::Key(Page, Name)), Mode(Mode)
	{
	}

	Setting::~Setting()
	{
	}

	void Setting::Set(NxFr::StringView Value)
	{
		NX_ASSERT(false, Default, "Not implemented");
	}

	void Setting::Set(uint64 Index, NxFr::StringView Value)
	{
		NX_ASSERT(false, Default, "Not implemented");
	}

	void Setting::Set(NxFr::StringView Key, NxFr::StringView Value)
	{
		NX_ASSERT(false, Default, "Not implemented");
	}

	NxFr::String Setting::Get()
	{
		NX_ASSERT(false, Default, "Not implemented");
		return NxFr::StringUtility::Empty;
	}

	NxFr::String Setting::Get(uint64 Index)
	{
		NX_ASSERT(false, Default, "Not implemented");
		return NxFr::StringUtility::Empty;
	}

	NxFr::String Setting::Get(NxFr::StringView Key)
	{
		NX_ASSERT(false, Default, "Not implemented");
		return NxFr::StringUtility::Empty;
	}
}
