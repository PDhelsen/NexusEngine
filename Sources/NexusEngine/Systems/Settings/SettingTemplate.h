#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/Settings/Setting.h"
#include "NexusEngine/Systems/Settings/SettingsSystem.h"
#include "NexusEngine/Systems/Memory/MemorySystem.h"

namespace NxEn
{
	template<typename T>
	class SettingVar : public Setting
	{
		using D = T;

	public:
		static SettingVar<T>* Create(NxFr::StringView Page, NxFr::StringView Name, const D& Data)
		{
			SettingVar<T>* Instance = new SettingVar<T>(Page, Name, Data);
			SettingsSystem::Settings.Register(Instance->GetId(), Instance);
			return Instance;
		}

		void OnGui(const GUI::Style* Visual = nullptr) override
		{
			GUI::Drawer<D>::Field(Data, GetName(), GetId(), Visual);
		}

		void OnDeserialize(const YAML::Node& Node) override
		{
			Data = Node.as<D>();
		}

		void OnSerialize(YAML::Node& Node) const override
		{
			Node[GetName()] = Data;
		}

		void Set(NxFr::StringView Value) override
		{
			Data = NxFr::StringUtility::FromString<T>(Value);
		}

		NxFr::String Get() override
		{
			return NxFr::StringUtility::ToString<T>(Data);
		}

		D& GetValue()
		{
			return Data;
		}

		const D& GetValue() const
		{
			return Data;
		}

	protected:
		SettingVar(NxFr::StringView Page, NxFr::StringView Name, const D& Data)
			: Setting(Page, Name, SettingMode::Var), Data(Data)
		{
		}

		~SettingVar()
		{
		}

	private:
		D Data;
	};

	template<typename T>
	class SettingSeq : public Setting
	{
		using D = NxFr::List<T>;

	public:
		static SettingSeq<T>* Create(NxFr::StringView Page, NxFr::StringView Name, const D& Data)
		{
			SettingSeq<T>* Instance = new SettingSeq<T>(Page, Name, Data);
			SettingsSystem::Settings.Register(Instance->GetId(), Instance);
			return Instance;
		}

		void OnGui(const GUI::Style* Visual = nullptr) override
		{
			GUI::Drawer<D>::Field(Data, GetName(), GetId(), Visual);
		}

		void OnDeserialize(const YAML::Node& Node) override
		{
			Data = Node.as<D>();
		}

		void OnSerialize(YAML::Node& Node) const override
		{
			Node[GetName()] = Data;
		}

		void Set(uint64 Index, NxFr::StringView Value) override
		{
			Data[Index] = NxFr::StringUtility::FromString<T>(Value);
		}

		NxFr::String Get(uint64 Index) override
		{
			return NxFr::StringUtility::ToString<T>(Data[Index]);
		}

		D& GetValue()
		{
			return Data;
		}

		const D& GetValue() const
		{
			return Data;
		}

	protected:
		SettingSeq(NxFr::StringView Page, NxFr::StringView Name, const D& Data)
			: Setting(Page, Name, SettingMode::Seq), Data(Data)
		{
		}

		~SettingSeq()
		{
		}

	private:
		D Data;
	};

	template<typename T>
	class SettingMap : public Setting
	{
		using D = NxFr::Dictionary<NxFr::String, T>;

	public:
		static SettingMap<T>* Create(NxFr::StringView Page, NxFr::StringView Name, const D& Data)
		{
			SettingMap<T>* Instance = new SettingMap<T>(Page, Name, Data);
			SettingsSystem::Settings.Register(Instance->GetId(), Instance);
			return Instance;
		}

		void OnGui(const GUI::Style* Visual = nullptr) override
		{
			GUI::Drawer<D>::Field(Data, GetName(), GetId(), Visual);
		}

		void OnDeserialize(const YAML::Node& Node) override
		{
			Data = Node.as<D>();
		}

		void OnSerialize(YAML::Node& Node) const override
		{
			Node[GetName()] = Data;
		}

		void Set(NxFr::StringView Key, NxFr::StringView Value) override
		{
			Data[Key] = NxFr::StringUtility::FromString<T>(Value);
		}

		NxFr::String Get(NxFr::StringView Key) override
		{
			return NxFr::StringUtility::ToString<T>(Data[Key]);
		}

		D& GetValue()
		{
			return Data;
		}

		const D& GetValue() const
		{
			return Data;
		}

	protected:
		SettingMap(NxFr::StringView Page, NxFr::StringView Name, const D& Data)
			: Setting(Page, Name, SettingMode::Map), Data(Data)
		{
		}

		~SettingMap()
		{
		}

	private:
		D Data;
	};
}
