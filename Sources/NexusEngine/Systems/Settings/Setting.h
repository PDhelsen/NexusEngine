#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	struct Setting
	{
	private:
		union SettingData
		{
			SettingData() {}
			~SettingData() {}

			NxFr::String String;
			NxFr::Vector4f Vector;
			float Float;
			bool Bool;
		};

	public:
		enum class SettingMode
		{
			String, Vector, Float, Bool
		};

		NEXUS_ENGINE_API inline static NxFr::StringView Default = "Settings";

		NEXUS_ENGINE_API static NxFr::String Key(NxFr::StringView Page, NxFr::StringView Name);
		NEXUS_ENGINE_API static Setting Create(NxFr::StringView Page, NxFr::StringView Name, const char* Data);
		NEXUS_ENGINE_API static Setting Create(NxFr::StringView Page, NxFr::StringView Name, NxFr::Vector4f Data);
		NEXUS_ENGINE_API static Setting Create(NxFr::StringView Page, NxFr::StringView Name, float Data);
		NEXUS_ENGINE_API static Setting Create(NxFr::StringView Page, NxFr::StringView Name, bool Data);

		NEXUS_ENGINE_API Setting(NxFr::StringView Page, NxFr::StringView Name, SettingMode Mode);
		NEXUS_ENGINE_API Setting(const Setting& Other);
		NEXUS_ENGINE_API Setting(Setting&& Other) noexcept;
		NEXUS_ENGINE_API ~Setting();

		NEXUS_ENGINE_API void Set(NxFr::StringView Value);
		NEXUS_ENGINE_API NxFr::String Get();

		NEXUS_ENGINE_API void Serialize(YAML::Emitter& Node);
		NEXUS_ENGINE_API void Deserialize(const YAML::Node& Node);

		NEXUS_ENGINE_API NxFr::String& AsString() { NEXUS_ASSERT(Mode == SettingMode::String, Default, "Setting mode is not string"); return Data.String; }
		NEXUS_ENGINE_API NxFr::Vector4f& AsVector() { NEXUS_ASSERT(Mode == SettingMode::Vector, Default, "Setting mode is not vector"); return Data.Vector; }
		NEXUS_ENGINE_API float& AsFloat() { NEXUS_ASSERT(Mode == SettingMode::Float, Default, "Setting mode is not float"); return Data.Float; }
		NEXUS_ENGINE_API bool& AsBool() { NEXUS_ASSERT(Mode == SettingMode::Bool, Default, "Setting mode is not bool"); return Data.Bool; }

		NEXUS_ENGINE_API NxFr::StringView GetId() const { return Id; }
		NEXUS_ENGINE_API NxFr::StringView GetPage() const { return Id.Split(".", 0); }
		NEXUS_ENGINE_API NxFr::StringView GetName() const { return Id.Split(".", 1); }
		NEXUS_ENGINE_API SettingMode GetMode() const { return Mode; }

	private:
		const NxFr::String Id;
		const SettingMode Mode;
		SettingData Data;
	};
}
