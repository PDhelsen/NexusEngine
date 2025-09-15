#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Settings/Setting.h"

namespace NxEn
{
	namespace Settings
	{
		NxFr::StringView DefaultPage = "Settings";

		NxFr::String Key(NxFr::StringView Page, NxFr::StringView Name)
		{
			return Page + "." + Name;
		}

		Value::Value(Type Type)
			: SettingType(Type)
		{
			if (SettingType == Type::String)
			{
				new (&SettingData.String) NxFr::String("");
			}
		}

		Value::Value(const Value& Other)
			: SettingType(Other.SettingType)
		{
			switch (SettingType)
			{
			case Type::String: new (&SettingData.String) NxFr::String(Other.SettingData.String); break;
			case Type::Vector: SettingData.Vector = Other.SettingData.Vector; break;
			case Type::Float: SettingData.Float = Other.SettingData.Float; break;
			case Type::Bool: SettingData.Bool = Other.SettingData.Bool; break;
			}
		}

		Value::Value(Value&& Other) noexcept
			: SettingType(Other.SettingType)
		{
			switch (SettingType)
			{
			case Type::String: new (&SettingData.String) NxFr::String(NxFr::Move(Other.SettingData.String)); break;
			case Type::Vector: SettingData.Vector = Other.SettingData.Vector; break;
			case Type::Float: SettingData.Float = Other.SettingData.Float; break;
			case Type::Bool: SettingData.Bool = Other.SettingData.Bool; break;
			}
		}

		Value::~Value()
		{
			if (SettingType == Type::String)
			{
				SettingData.String.~String();
			}
		}

		Value& Value::operator=(const Value& Other)
		{
			NEXUS_ASSERT(SettingType == Other.SettingType, Default, "Suppport only assign from same type");
			switch (SettingType)
			{
			case Type::String: SettingData.String = Other.SettingData.String; break;
			case Type::Vector: SettingData.Vector = Other.SettingData.Vector; break;
			case Type::Float: SettingData.Float = Other.SettingData.Float; break;
			case Type::Bool: SettingData.Bool = Other.SettingData.Bool; break;
			}
			return *this;
		}

		Value& Value::operator=(Value&& Other) noexcept
		{
			NEXUS_ASSERT(SettingType == Other.SettingType, Default, "Suppport only assign from same type");
			switch (SettingType)
			{
			case Type::String: SettingData.String = NxFr::Move(Other.SettingData.String); break;
			case Type::Vector: SettingData.Vector = Other.SettingData.Vector; break;
			case Type::Float: SettingData.Float = Other.SettingData.Float; break;
			case Type::Bool: SettingData.Bool = Other.SettingData.Bool; break;
			}
			return *this;
		}

		void Value::Set(NxFr::StringView Value)
		{
			switch (SettingType)
			{
			case Type::String: SettingData.String = Value.ToString(); break;
			case Type::Vector: NxFr::StringUtility::Scan(Value, "(%f, %f, %f, %f)", &SettingData.Vector.x, &SettingData.Vector.y, &SettingData.Vector.z, &SettingData.Vector.w); break;
			case Type::Float: SettingData.Float = (float)NxFr::StringUtility::ToDouble(Value); break;
			case Type::Bool: SettingData.Bool = Value == "True" ? true : false; break;
			}
		}

		NxFr::String Value::Get() const
		{
			switch (SettingType)
			{
			case Type::String: return SettingData.String;
			case Type::Vector: return SettingData.Vector.ToString();
			case Type::Float: return NxFr::StringUtility::ToStringF(SettingData.Float);
			case Type::Bool: return NxFr::StringUtility::ToStringB(SettingData.Bool);
			}

			return NxFr::StringUtility::Empty;
		}
	}

	Setting::Setting(NxFr::StringView Page, NxFr::StringView Name, Settings::Mode Mode, Settings::Type Type)
		: Id(Settings::Key(Page, Name)), Mode(Mode), Type(Type)
	{
	}

	Setting::~Setting()
	{
	}

	SettingVar::SettingVar(NxFr::StringView Page, NxFr::StringView Name, Settings::Type Type)
		: Setting(Page, Name, Settings::Mode::Variable, Type), Value(Type)
	{
	}

	SettingVar::~SettingVar()
	{
	}
}
