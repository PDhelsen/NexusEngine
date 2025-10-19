#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace Settings
	{
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
			NEXUS_ENGINE_API Value();
			NEXUS_ENGINE_API Value(Type SettingType);
			NEXUS_ENGINE_API Value(const Value& Other);
			NEXUS_ENGINE_API Value(Value&& Other) noexcept;
			NEXUS_ENGINE_API ~Value();

			NEXUS_ENGINE_API Value& operator=(const Value& Other);
			NEXUS_ENGINE_API Value& operator=(Value&& Other) noexcept;

			NEXUS_ENGINE_API void Deserialize(const YAML::Node& Node);
			NEXUS_ENGINE_API void Serialize(YAML::Emitter& Node) const;

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
	}

	struct Setting
	{
	public:
		NEXUS_ENGINE_API static void ResetStyle();

		NEXUS_ENGINE_API virtual void OnGui() = 0;

		NEXUS_ENGINE_API virtual void Deserialize(const YAML::Node& Node) = 0;
		NEXUS_ENGINE_API virtual void Serialize(YAML::Emitter& Node) const = 0;

		NEXUS_ENGINE_API virtual void Set(NxFr::StringView Data) { NEXUS_ASSERT(false, Default, "Not supported"); }
		NEXUS_ENGINE_API virtual void Set(uint64 Index, NxFr::StringView Data) { NEXUS_ASSERT(false, Default, "Not supported"); }
		NEXUS_ENGINE_API virtual void Set(const NxFr::String& Key, NxFr::StringView Data) { NEXUS_ASSERT(false, Default, "Not supported"); }
		NEXUS_ENGINE_API virtual NxFr::String Get() const { NEXUS_ASSERT(false, Default, "Not supported"); return NxFr::StringUtility::Empty; }
		NEXUS_ENGINE_API virtual NxFr::String Get(uint64 Index) const { NEXUS_ASSERT(false, Default, "Not supported"); return NxFr::StringUtility::Empty; }
		NEXUS_ENGINE_API virtual NxFr::String Get(const NxFr::String& Key) const { NEXUS_ASSERT(false, Default, "Not supported"); return NxFr::StringUtility::Empty; }
		template<typename T> T& As() { NEXUS_ASSERT(false, Default, "Not supported"); return 0; }
		template<typename T> T& As(uint64 Index) { NEXUS_ASSERT(false, Default, "Not supported"); return 0; }
		template<typename T> T& As(const NxFr::String& Key) { NEXUS_ASSERT(false, Default, "Not supported"); return 0; }

		NEXUS_ENGINE_API NxFr::StringView GetId() const { return Id; }
		NEXUS_ENGINE_API NxFr::StringView GetPage() const { return NxFr::StringUtility::Split(Id, ".", 0); }
		NEXUS_ENGINE_API NxFr::StringView GetName() const { return NxFr::StringUtility::Split(Id, ".", 1); }
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
		NEXUS_ENGINE_API static SettingVar* Create(NxFr::StringView Page, NxFr::StringView Name, Settings::Type Type);

		NEXUS_ENGINE_API void OnGui() override;

		NEXUS_ENGINE_API void Deserialize(const YAML::Node& Node) override;
		NEXUS_ENGINE_API void Serialize(YAML::Emitter& Node) const override;

		NEXUS_ENGINE_API void Set(NxFr::StringView Data) override { Value.Set(Data); }
		NEXUS_ENGINE_API NxFr::String Get() const override { return Value.Get(); }
		template<typename T> T& As() { return Value.As<T>(); }

	protected:
		NEXUS_ENGINE_API SettingVar(NxFr::StringView Page, NxFr::StringView Name, Settings::Type Type);
		NEXUS_ENGINE_API virtual ~SettingVar();

	protected:
		Settings::Value Value;
	};

	struct SettingSeq : public Setting
	{
	public:
		NEXUS_ENGINE_API static SettingSeq* Create(NxFr::StringView Page, NxFr::StringView Name, Settings::Type Type);

		NEXUS_ENGINE_API void OnGui() override;

		NEXUS_ENGINE_API void Deserialize(const YAML::Node& Node);
		NEXUS_ENGINE_API void Serialize(YAML::Emitter& Node) const;

		NEXUS_ENGINE_API virtual void Set(uint64 Index, NxFr::StringView Data) { Values[Index].Set(Data); }
		NEXUS_ENGINE_API virtual NxFr::String Get(uint64 Index) const { return Values[Index].Get(); }
		template<typename T> T& As(uint64 Index) { return Values[Index].As<T>(); }

		NEXUS_ENGINE_API NxFr::Collection<Settings::Value> GetCollection() const { return Values; }
		NEXUS_ENGINE_API uint64 GetCount() const { return Values.GetCount(); }

	protected:
		NEXUS_ENGINE_API SettingSeq(NxFr::StringView Page, NxFr::StringView Name, Settings::Type Type);
		NEXUS_ENGINE_API virtual ~SettingSeq();

	protected:
		NxFr::List<Settings::Value> Values;
	};

	struct SettingMap : public Setting
	{
	public:
		NEXUS_ENGINE_API static SettingMap* Create(NxFr::StringView Page, NxFr::StringView Name, Settings::Type Type);

		NEXUS_ENGINE_API void OnGui() override;

		NEXUS_ENGINE_API void Deserialize(const YAML::Node& Node);
		NEXUS_ENGINE_API void Serialize(YAML::Emitter& Node) const;

		NEXUS_ENGINE_API virtual void Set(const NxFr::String& Key, NxFr::StringView Data) { Values[Key].Set(Data); }
		NEXUS_ENGINE_API virtual NxFr::String Get(const NxFr::String& Key) const { return Values[Key].Get(); }
		template<typename T> T& As(const NxFr::String& Key) { return Values[Key].As<T>(); }

		NEXUS_ENGINE_API NxFr::Collection<NxFr::KeyValuePair<const NxFr::String, Settings::Value>> GetCollection() const { return Values; }
		NEXUS_ENGINE_API uint64 GetCount() const { return Values.GetCount(); }

	protected:
		NEXUS_ENGINE_API SettingMap(NxFr::StringView Page, NxFr::StringView Name, Settings::Type Type);
		NEXUS_ENGINE_API virtual ~SettingMap();

	protected:
		NxFr::Dictionary<NxFr::String, Settings::Value> Values;
	};
}
