#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/Settings/Setting.h"
#include "NexusEngine/Systems/Settings/SettingsSystem.h"
#include "NexusEngine/Systems/Memory/MemorySystem.h"

namespace NxEn
{
	template<typename T>
	class SettingBase : public Setting
	{
	public:
		void OnDraw() override
		{
			GUI::Drawer<T>::Field(Data, GetName(), GetKey());
		}

		void OnSerialize(YAML::Node& Node) override
		{
			Node[GetName()] = Data;
		}

		void OnDeserialize(const YAML::Node& Node) override
		{
			Data = Node.as<T>();
		}

		T& GetValue()
		{
			return Data;
		}

		const T& GetValue() const
		{
			return Data;
		}

	protected:
		SettingBase(NxFr::StringView Page, NxFr::StringView Name, const T& Data)
			: Setting(Page, Name), Data(Data)
		{
		}

		~SettingBase()
		{
		}

		T Data;
	};

	template<typename T>
	class SettingVar : public SettingBase<T>
	{
	public:
		static SettingVar<T>* Create(NxFr::StringView Page, NxFr::StringView Name, const T& Data)
		{
			SettingVar<T>* Instance = new SettingVar<T>(Page, Name, Data);
			SettingsSystem::Settings.Register(Instance->GetId(), Instance);
			return Instance;
		}

		void Set(NxFr::StringView Value) override
		{
			this->Data = NxFr::StringUtility::FromString<T>(Value);
		}

		NxFr::String Get() override
		{
			return NxFr::StringUtility::ToString<T>(this->Data);
		}

	private:
		SettingVar(NxFr::StringView Page, NxFr::StringView Name, const T& Data)
			: SettingBase<T>(Page, Name, Data)
		{
		}
	};

	template<typename T>
	class SettingSeq : public SettingBase<NxFr::List<T>>
	{
	public:
		static SettingSeq<T>* Create(NxFr::StringView Page, NxFr::StringView Name, const NxFr::List<T>& Data)
		{
			SettingSeq<T>* Instance = new SettingSeq<T>(Page, Name, Data);
			SettingsSystem::Settings.Register(Instance->GetId(), Instance);
			return Instance;
		}

		void Set(uint64 Index, NxFr::StringView Value) override
		{
			this->Data[Index] = NxFr::StringUtility::FromString<T>(Value);
		}

		NxFr::String Get(uint64 Index) override
		{
			return NxFr::StringUtility::ToString<T>(this->Data[Index]);
		}

	private:
		SettingSeq(NxFr::StringView Page, NxFr::StringView Name, const NxFr::List<T>& Data)
			: SettingBase<NxFr::List<T>>(Page, Name, Data)
		{
		}
	};

	template<typename T>
	class SettingMap : public SettingBase<NxFr::Dictionary<NxFr::String, T>>
	{
	public:
		static SettingMap<T>* Create(NxFr::StringView Page, NxFr::StringView Name, const NxFr::Dictionary<NxFr::String, T>& Data)
		{
			SettingMap<T>* Instance = new SettingMap<T>(Page, Name, Data);
			SettingsSystem::Settings.Register(Instance->GetId(), Instance);
			return Instance;
		}

		void Set(NxFr::StringView Key, NxFr::StringView Value) override
		{
			this->Data[Key] = NxFr::StringUtility::FromString<T>(Value);
		}

		NxFr::String Get(NxFr::StringView Key) override
		{
			return NxFr::StringUtility::ToString<T>(this->Data[Key]);
		}

	private:
		SettingMap(NxFr::StringView Page, NxFr::StringView Name, const NxFr::Dictionary<NxFr::String, T>& Data)
			: SettingBase<NxFr::Dictionary<NxFr::String, T>>(Page, Name, Data)
		{
		}
	};
}
