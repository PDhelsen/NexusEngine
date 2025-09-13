#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Settings/Setting.h"

namespace NxEn
{
	NxFr::String Setting::Key(NxFr::StringView Page, NxFr::StringView Name)
	{
		return Page + "." + Name;
	}

	Setting Setting::Create(NxFr::StringView Page, NxFr::StringView Id, const char* Data)
	{
		Setting Instance = Setting(Page, Id, SettingMode::String);
		Instance.AsString() = Data;
		SettingsSystem::RegisterSetting(&Instance);
		return Instance;
	}

	Setting Setting::Create(NxFr::StringView Page, NxFr::StringView Id, NxFr::Vector4f Data)
	{
		Setting Instance = Setting(Page, Id, SettingMode::Vector);
		Instance.AsVector() = Data;
		SettingsSystem::RegisterSetting(&Instance);
		return Instance;
	}

	Setting Setting::Create(NxFr::StringView Page, NxFr::StringView Id, float Data)
	{
		Setting Instance = Setting(Page, Id, SettingMode::Float);
		Instance.AsFloat() = Data;
		SettingsSystem::RegisterSetting(&Instance);
		return Instance;
	}

	Setting Setting::Create(NxFr::StringView Page, NxFr::StringView Id, bool Data)
	{
		Setting Instance = Setting(Page, Id, SettingMode::Bool);
		Instance.AsBool() = Data;
		SettingsSystem::RegisterSetting(&Instance);
		return Instance;
	}

	Setting::Setting(NxFr::StringView Page, NxFr::StringView Name, SettingMode Mode)
		: Id(Key(Page, Name)), Mode(Mode)
	{
		NxFr::Memory::MemSet(&Data, 0, sizeof(SettingData));
	}

	Setting::Setting(const Setting& Other)
		: Id(Other.Id), Mode(Other.Mode)
	{
		switch (Mode)
		{
		case SettingMode::String: Data.String = Other.Data.String; break;
		case SettingMode::Vector: Data.Vector = Other.Data.Vector; break;
		case SettingMode::Float: Data.Float = Other.Data.Float; break;
		case SettingMode::Bool: Data.Bool = Other.Data.Bool; break;
		}
	}

	Setting::Setting(Setting&& Other) noexcept
		: Id(Move(Other.Id)), Mode(Other.Mode)
	{
		switch (Mode)
		{
		case SettingMode::String: Data.String = NxFr::Move(Other.Data.String); break;
		case SettingMode::Vector: Data.Vector = NxFr::Move(Other.Data.Vector); break;
		case SettingMode::Float: Data.Float = NxFr::Move(Other.Data.Float); break;
		case SettingMode::Bool: Data.Bool = NxFr::Move(Other.Data.Bool); break;
		}
	}

	Setting::~Setting()
	{
		if (Mode == SettingMode::String)
		{
			Data.String.~String();
		}
	}

	void Setting::Set(NxFr::StringView Value)
	{
		switch (Mode)
		{
		case SettingMode::String: Data.String = Value.ToString(); break;
		case SettingMode::Vector: NxFr::StringUtility::Scan(Value, "[%f, %f, %f, %f]", &AsVector().x, &AsVector().y, &AsVector().z, &AsVector().w); break;
		case SettingMode::Float: Data.Float = (float)NxFr::StringUtility::ToDouble(Value); break;
		case SettingMode::Bool: Data.Bool = Value == "True" ? true : false; break;
		}
	}

	NxFr::String Setting::Get()
	{
		switch (Mode)
		{
		case SettingMode::String: return Data.String;
		case SettingMode::Vector: return Data.Vector.ToString();
		case SettingMode::Float: return NxFr::StringUtility::ToStringF(Data.Float);
		case SettingMode::Bool: return NxFr::StringUtility::ToStringB(Data.Bool);
		}
	}

	void Setting::Serialize(YAML::Emitter& Node)
	{
		Node << YAML::Key << GetName();
		switch (Mode)
		{
		case Setting::SettingMode::String: Node << YAML::Value << Data.String; break;
		case Setting::SettingMode::Vector: Node << YAML::Value << Data.Vector; break;
		case Setting::SettingMode::Float: Node << YAML::Value << Data.Float; break;
		case Setting::SettingMode::Bool: Node << YAML::Value << Data.Bool; break;
		}
	}

	void Setting::Deserialize(const YAML::Node& Node)
	{
		switch (Mode)
		{
		case Setting::SettingMode::String: Data.String = Node.as<NxFr::String>(); break;
		case Setting::SettingMode::Vector: Data.Vector = Node.as<NxFr::Vector4f>(); break;
		case Setting::SettingMode::Float: Data.Float = Node.as<float>(); break;
		case Setting::SettingMode::Bool: Data.Bool = Node.as<bool>(); break;
		}
	}
}
