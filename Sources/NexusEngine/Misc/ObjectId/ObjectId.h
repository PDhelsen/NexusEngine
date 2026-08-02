#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/GUISystem.h"

namespace NxEn
{
	namespace ObjectId
	{
		template<typename T, typename = void>
		struct ObjectIdResolver
		{
			static T Resolve(NxFr::GUID Id)
			{
				return T();
			}
		};

		template<typename T>
		void Resolve(T* Target, NxFr::GUID Id)
		{
			*Target = ObjectIdResolver<T>::Resolve(Id);
		}

		template<typename T>
		void DrawProperty(T* Target, NxFr::StringView Label, const GUI::Style* Visual = nullptr)
		{
			NxFr::GUID Id = *Target ? (*Target)->GetId() : Object::NullId;
			NxFr::StringView Name = *Target ? (*Target)->GetName() : NxFr::StringUtility::Empty;

			GUI::Style DrawerStyle = GUI::Style::Copy(Visual);
			GUI::Draw::Label(Label, &DrawerStyle);
			DrawerStyle.Position.x = -1.0f;
			GUI::Draw::Text(NxFr::StringUtility::ToString(Id), &DrawerStyle);
			ImGui::SameLine();
			GUI::Draw::Text(Name, &DrawerStyle);
		}

		template<typename T>
		bool DrawField(T* Target, NxFr::StringView Label, const GUI::Style* Visual = nullptr)
		{
			NxFr::GUID Id = *Target ? (*Target)->GetId() : Object::NullId;
			NxFr::StringView Name = *Target ? (*Target)->GetName() : (NxFr::StringView)NxFr::StringUtility::Empty;
			NxFr::String Value = NxFr::StringUtility::ToString(Id);
			NxFr::String ImGuiId = GUI::Utils::GenerateId(Value, Label);

			GUI::Style DrawerStyle = GUI::Style::Copy(Visual);
			GUI::Draw::Label(Label, &DrawerStyle);
			DrawerStyle.Position.x = -1.0f;
			DrawerStyle.Size.x = 150.0f;
			DrawerStyle.Flag = DrawerStyle.Flag != 0 ? DrawerStyle.Flag : ImGuiInputTextFlags_EnterReturnsTrue;
			bool Result = GUI::Draw::Input(Value, ImGuiId, &DrawerStyle);
			ImGui::SameLine();
			GUI::Draw::Text(Name, &DrawerStyle);

			if (Result)
			{
				Resolve(Target, NxFr::StringUtility::FromString<NxFr::GUID>(Value));
			}

			return Result;
		}

		template<typename T>
		void Clone(T* Target, T Instance)
		{
			Resolve(Target, Instance ? Instance->GetId() : Object::NullId);
		}

		template<typename T>
		void Serialize(T* Target, YAML::Node Node)
		{
			Node = *Target ? (*Target)->GetId() : Object::NullId;
		}

		template<typename T>
		void Deserialize(T* Target, YAML::Node Node)
		{
			Resolve(Target, Node.as<NxFr::GUID>());
		}
	};
}
