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

	namespace GUI
	{
		namespace Draw
		{
			template<typename T>
			void PropertyOjbectId(T* Target, NxFr::StringView Label, const Transform& Visual = {})
			{
				NxFr::GUID Id = *Target ? (*Target)->GetId() : Object::NullId;
				NxFr::StringView Name = *Target ? (*Target)->GetName() : (NxFr::StringView)NxFr::StringUtility::Empty;

				Transform ItemVisual = Visual;
				ItemVisual.Size.x = GUI::Styles::WidthLabel();

				Drawer<NxFr::GUID>::Property(Id, Label, ItemVisual);
				ImGui::SameLine();
				Text(Name);
			}

			template<typename T>
			bool FieldOjbectId(T* Target, NxFr::StringView Label, const Transform& Visual = {})
			{
				NxFr::GUID Id = *Target ? (*Target)->GetId() : Object::NullId;
				NxFr::StringView Name = *Target ? (*Target)->GetName() : (NxFr::StringView)NxFr::StringUtility::Empty;
				NxFr::String Buffer = NxFr::StringUtility::ToString(Id);
				NxFr::String ImGuiId = Utils::GenerateStringId(Buffer, Label);

				Transform ItemVisual = Visual;
				ItemVisual.Size.x = GUI::Styles::WidthLabel();

				bool Result = Drawer<NxFr::GUID>::Field(Id, Label, ImGuiId, ItemVisual);
				ImGui::SameLine();
				Text(Name);

				if (Result)
				{
					ObjectId::Resolve(Target, NxFr::StringUtility::FromString<NxFr::GUID>(Buffer));
				}

				return Result;
			}
		}
	}
}
