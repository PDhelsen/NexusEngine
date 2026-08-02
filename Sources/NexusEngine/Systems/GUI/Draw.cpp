#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Draw.h"

namespace NxEn
{
	namespace GUI
	{
		namespace Draw
		{
			void Label(NxFr::StringView Data, const Style* Visual)
			{
				if (Data.IsEmpty())
				{
					return;
				}

				Utils::PushStyle(Visual);
				Utils::SetPosition(Visual);

				Label(Data);

				Utils::PopStyle(Visual);
			}

			void Label(NxFr::StringView Data)
			{
				if (Data.IsEmpty())
				{
					return;
				}

				ImGui::AlignTextToFramePadding();
				ImGui::Text(Data.C());
				ImGui::SameLine();
			}

			void Text(NxFr::StringView Data, const Style* Visual)
			{
				Utils::PushStyle(Visual);
				Utils::SetPosition(Visual);

				Text(Data);

				Utils::PopStyle(Visual);
			}

			void Text(NxFr::StringView Data)
			{
				ImGui::Text(Data.C());
			}

			bool Input(NxFr::String& Data, NxFr::StringView Id, const Style* Visual)
			{
				Utils::PushStyle(Visual);
				Utils::SetPosition(Visual);
				Utils::SetWidth(Visual);
				uint64 Flag = Visual ? Visual->Flag : ImGuiInputTextFlags_EnterReturnsTrue;

				bool Result = Input(Data, Id, Flag);

				Utils::PopStyle(Visual);

				return Result;
			}

			bool Input(NxFr::String& Data, NxFr::StringView Id, uint64 Flag)
			{
				Data.Reserve(128);
				bool Result = ImGui::InputText(Id.C(), Data.Characters(), Data.GetCapacity(), Flag);
				if (Result)
				{
					Data.Validate();
				}

				return Result;
			}

			bool Button(NxFr::StringView Data, const Style* Visual)
			{
				Utils::PushStyle(Visual);
				Utils::SetPosition(Visual);
				Utils::SetWidth(Visual);
				NxFr::Vector2f Size = Visual ? Visual->Size : NxFr::Vector2f::Zero;

				bool Result = Button(Data, Size);

				Utils::PopStyle(Visual);

				return Result;
			}

			bool Button(NxFr::StringView Data, NxFr::Vector2f Size)
			{
				return ImGui::Button(Data.C(), Size);
			}
		}
	}
}
