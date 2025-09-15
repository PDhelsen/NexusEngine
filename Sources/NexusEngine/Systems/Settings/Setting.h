#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class SettingsSystem;

	namespace Settings
	{
		NEXUS_ENGINE_API extern NxFr::StringView DefaultPage;
		NEXUS_ENGINE_API NxFr::String Key(NxFr::StringView Page, NxFr::StringView Name);

		enum class Mode
		{
			Variable, Sequence, Map
		};

		enum class Type
		{
			Invalid, String, Vector, Float, Bool
		};

		union Data
		{
			Data() {}
			~Data() {}

			NxFr::String String;
			NxFr::Vector4f Vector;
			float Float;
			bool Bool;
		};

		struct Value
		{
			NEXUS_ENGINE_API Value(Type SettingType);
			NEXUS_ENGINE_API Value(const Value& Other);
			NEXUS_ENGINE_API Value(Value&& Other) noexcept;
			NEXUS_ENGINE_API ~Value();

			NEXUS_ENGINE_API Value& operator=(const Value& Other);
			NEXUS_ENGINE_API Value& operator=(Value&& Other) noexcept;

			NEXUS_ENGINE_API void Set(NxFr::StringView Value);
			NEXUS_ENGINE_API NxFr::String Get() const;

			template<typename T>
			T& As()
			{
				NEXUS_ASSERT(false, Default, "Unsupported type");
				return 0;
			}

			const Type SettingType;
			Data SettingData;
		};

		template<>
		inline NxFr::String& Value::As()
		{
			return SettingData.String;
		}
		template<>
		inline NxFr::Vector4f& Value::As()
		{
			return SettingData.Vector;
		}
		template<>
		inline float& Value::As()
		{
			return SettingData.Float;
		}
		template<>
		inline bool& Value::As()
		{
			return SettingData.Bool;
		}

		template<typename T>
		inline Type ToType(const T& Value)
		{
			return Type::Invalid;
		}
		template<>
		inline Type ToType(const NxFr::String& Value)
		{
			return Type::String;
		}
		template<>
		inline Type ToType(const NxFr::Vector4f& Value)
		{
			return Type::Vector;
		}
		template<>
		inline Type ToType(const float& Value)
		{
			return Type::Float;
		}
		template<>
		inline Type ToType(const bool& Value)
		{
			return Type::Bool;
		}
	}

	struct Setting
	{
	public:
		NEXUS_ENGINE_API virtual void Set(NxFr::StringView Value) { NEXUS_ASSERT(false, Default, "Not supported"); }
		NEXUS_ENGINE_API virtual NxFr::String Get() const { NEXUS_ASSERT(false, Default, "Not supported"); return NxFr::StringUtility::Empty; }
		template<typename T> T& As() { NEXUS_ASSERT(false, Default, "Not supported"); return 0; }

		NEXUS_ENGINE_API NxFr::StringView GetId() const { return Id; }
		NEXUS_ENGINE_API NxFr::StringView GetPage() const { return Id.Split(".", 0); }
		NEXUS_ENGINE_API NxFr::StringView GetName() const { return Id.Split(".", 1); }
		NEXUS_ENGINE_API Settings::Mode GetMode() const { return Mode; }
		NEXUS_ENGINE_API Settings::Type GetType() const { return Type; }

	protected:
		NEXUS_ENGINE_API Setting(NxFr::StringView Page, NxFr::StringView Name, Settings::Mode Mode, Settings::Type Type);
		NEXUS_ENGINE_API virtual ~Setting();

	protected:
		const NxFr::String Id;
		const Settings::Mode Mode;
		const Settings::Type Type;
	};

	struct SettingVar : public Setting
	{
	public:
		template<typename T>
		static SettingVar* Create(NxFr::StringView Page, NxFr::StringView Name, const T& Value)
		{
			NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));

			SettingVar* Instance = new SettingVar(Page, Name, Settings::ToType(Value));
			Instance->As<T>() = Value;
			SettingsSystem::RegisterSetting(Instance);
			return Instance;
		}

		NEXUS_ENGINE_API void Set(NxFr::StringView Data) override { Value.Set(Data); }
		NEXUS_ENGINE_API NxFr::String Get() const override { return Value.Get(); }
		template<typename T> T& As() { return Value.As<T>(); }

	protected:
		NEXUS_ENGINE_API SettingVar(NxFr::StringView Page, NxFr::StringView Name, Settings::Type Type);
		NEXUS_ENGINE_API virtual ~SettingVar();

	protected:
		Settings::Value Value;
	};
}
