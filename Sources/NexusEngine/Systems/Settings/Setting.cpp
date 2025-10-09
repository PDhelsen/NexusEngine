#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Settings/Setting.h"

#include "NexusEngine/Systems/Settings/SettingsPanel.h"

namespace NxEn
{
	static const uint64 TextBuffer = 128;
	static NxFr::StringId WidthLabel = "WidthLabel"_Sid;

	namespace Settings
	{
		NxFr::StringView PageSettings = "Settings";
		NxFr::StringView PagePreferences = "Preferences";

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

		void Value::Deserialize(const YAML::Node& Node)
		{
			switch (SettingType)
			{
			case Type::String: SettingData.String = Node.as<NxFr::String>(); break;
			case Type::Vector: SettingData.Vector = Node.as<NxFr::Vector4f>(); break;
			case Type::Float: SettingData.Float = Node.as<float>(); break;
			case Type::Bool: SettingData.Bool = Node.as<bool>(); break;
			}
		}

		void Value::Serialize(YAML::Emitter& Node) const
		{
			switch (SettingType)
			{
			case Type::String: Node << SettingData.String; break;
			case Type::Vector: Node << SettingData.Vector; break;
			case Type::Float: Node << SettingData.Float; break;
			case Type::Bool: Node << SettingData.Bool; break;
			}
		}

		void Value::Set(NxFr::StringView Value)
		{
			switch (SettingType)
			{
			case Type::String: SettingData.String = NxFr::StringUtility::FromString<NxFr::String>(Value); break;
			case Type::Vector: SettingData.Vector = NxFr::StringUtility::FromString<NxFr::Vector4f>(Value); break;
			case Type::Float: SettingData.Float = NxFr::StringUtility::FromString<float>(Value); break;
			case Type::Bool: SettingData.Bool = NxFr::StringUtility::FromString<bool>(Value); break;
			}
		}

		NxFr::String Value::Get() const
		{
			switch (SettingType)
			{
			case Type::String: return NxFr::StringUtility::ToString(SettingData.String);
			case Type::Vector: return NxFr::StringUtility::ToString(SettingData.Vector);
			case Type::Float: return NxFr::StringUtility::ToString(SettingData.Float);
			case Type::Bool: return NxFr::StringUtility::ToString(SettingData.Bool);
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

	SettingVar* SettingVar::Create(NxFr::StringView Page, NxFr::StringView Name, Settings::Type Type)
	{
		NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		SettingVar* Instance = new SettingVar(Page, Name, Type);
		SettingsSystem::RegisterSetting(Instance);
		return Instance;
	}

	void SettingVar::OnGui()
	{
		NxFr::String ImGuiId = "##" + Id;
		NxFr::String Text = Get();
		Text.Grow(TextBuffer);

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		if (ImGui::InputText(ImGuiId.C(), Text.Characters(), Text.GetCapacity(), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			Text.Validate();
			Set(Text);
		}
	}

	void SettingVar::Deserialize(const YAML::Node& Node)
	{
		Value.Deserialize(Node);
	}

	void SettingVar::Serialize(YAML::Emitter& Node) const
	{
		Value.Serialize(Node);
	}

	SettingVar::SettingVar(NxFr::StringView Page, NxFr::StringView Name, Settings::Type Type)
		: Setting(Page, Name, Settings::Mode::Variable, Type), Value(Type)
	{
	}

	SettingVar::~SettingVar()
	{
	}

	SettingSeq* SettingSeq::Create(NxFr::StringView Page, NxFr::StringView Name, Settings::Type Type)
	{
		NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		SettingSeq* Instance = new SettingSeq(Page, Name, Type);
		SettingsSystem::RegisterSetting(Instance);
		return Instance;
	}

	void SettingSeq::OnGui()
	{
		float Position = ImGui::GetCursorPosX();
		ImGui::Text("Count: %d", Values.GetCount());

		for (uint64 Index = 0; Index < Values.GetCount(); ++Index)
		{
			NxFr::String ImGuiId = "##" + Id + NxFr::StringUtility::ToString(Index);
			NxFr::String Text = Get(Index);
			Text.Grow(TextBuffer);

			ImGui::SetCursorPosX(Position);
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImGui::InputText(ImGuiId.C(), Text.Characters(), Text.GetCapacity(), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				Text.Validate();
				Set(Index, Text);
			}
		}
	}

	void SettingSeq::Deserialize(const YAML::Node& Node)
	{
		Values.Clear();
		Values.Grow(Node.size());

		for (uint64 Index = 0; Index < Node.size(); ++Index)
		{
			Values.AppendConstruct(Type);
			Values[Index].Deserialize(Node[Index]);
		}
	}

	void SettingSeq::Serialize(YAML::Emitter& Node) const
	{
		Node << YAML::BeginSeq;
		for (uint64 Index = 0; Index < Values.GetCount(); ++Index)
		{
			Values[Index].Serialize(Node);
		}
		Node << YAML::EndSeq;
	}

	SettingSeq::SettingSeq(NxFr::StringView Page, NxFr::StringView Name, Settings::Type Type)
		: Setting(Page, Name, Settings::Mode::Sequence, Type), Values()
	{
	}

	SettingSeq::~SettingSeq()
	{
	}

	SettingMap* SettingMap::Create(NxFr::StringView Page, NxFr::StringView Name, Settings::Type Type)
	{
		NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));

		SettingMap* Instance = new SettingMap(Page, Name, Type);
		SettingsSystem::RegisterSetting(Instance);
		return Instance;
	}

	void SettingMap::OnGui()
	{
		float Position = ImGui::GetCursorPosX();
		ImGui::Text("Count: %d", Values.GetCount());

		for (auto& [Key, Value] : Values)
		{
			NxFr::String ImGuiId = "##" + Id + Key;
			NxFr::String Text = Get(Key);
			Text.Grow(TextBuffer);
			
			ImGui::SetCursorPosX(Position);
			ImGui::AlignTextToFramePadding();
			ImGui::Text(Key.C());
			ImGui::SameLine();
			ImGui::SetCursorPosX(Position + GUI::Style::GetVar(WidthLabel));
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImGui::InputText(ImGuiId.C(), Text.Characters(), Text.GetCapacity(), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				Text.Validate();
				Set(Key, Text);
			}
		}
	}

	void SettingMap::Deserialize(const YAML::Node& Node)
	{
		Values.Clear();
		Values.Grow(Node.size());

		for (auto It = Node.begin(); It != Node.end(); ++It)
		{
			NxFr::String Key = It->first.as<NxFr::String>();
			Values.AppendConstruct(NxFr::Move(Key), Type);
			Values[Key].Deserialize(It->second);
		}
	}

	void SettingMap::Serialize(YAML::Emitter& Node) const
	{
		Node << YAML::BeginMap;
		for (auto& [Key, Value] : Values)
		{
			Node << YAML::Key << Key << YAML::Value;
			Value.Serialize(Node);
		}
		Node << YAML::EndMap;
	}

	SettingMap::SettingMap(NxFr::StringView Page, NxFr::StringView Name, Settings::Type Type)
		: Setting(Page, Name, Settings::Mode::Sequence, Type), Values()
	{
	}

	SettingMap::~SettingMap()
	{
	}
}
