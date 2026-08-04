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

				Style::Scope _ = Visual;
				Utils::SetPosition(Visual);

				Label(Data);

				Utils::OffsetLabel(Visual);
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
				Style::Scope _ = Visual;
				Utils::SetPosition(Visual);

				Text(Data);
			}

			void Text(NxFr::StringView Data)
			{
				ImGui::Text(Data.C());
			}

			bool Input(NxFr::String& Data, NxFr::StringView Id, const Style* Visual)
			{
				Style::Scope _ = Visual;
				Utils::SetPosition(Visual);
				Utils::SetSize(Visual);
				uint64 Flag = Visual ? Visual->Flag : ImGuiInputTextFlags_EnterReturnsTrue;

				return Input(Data, Id, Flag);;
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
				Style::Scope _ = Visual;
				Utils::SetPosition(Visual);
				NxFr::Vector2f Size = Visual ? Visual->Size : NxFr::Vector2f::Zero;

				return Button(Data, Size);
			}

			bool Button(NxFr::StringView Data, NxFr::Vector2f Size)
			{
				return ImGui::Button(Data.C(), Size);
			}

			bool CheckBox(NxFr::StringView Text, bool& Data, const Style* Visual)
			{
				Style::Scope _ = Visual;
				Utils::SetPosition(Visual);
				Utils::SetSize(Visual);

				return CheckBox(Text, Data);
			}

			bool CheckBox(NxFr::StringView Text, bool& Data)
			{
				return ImGui::Checkbox(Text.C(), &Data);
			}

			void Space(const Style* Visual)
			{
				Style::Scope _ = Visual;
				Utils::SetPosition(Visual);
				NxFr::Vector2f Size = Visual ? Visual->Size : NxFr::Vector2f::Zero;

				return Space(Size);
			}

			void Space(NxFr::Vector2f Size)
			{
				ImGui::Dummy(Size);
			}

			void Separator(const Style* Visual)
			{
				Style::Scope _ = Visual;
				Utils::SetPosition(Visual);
				Utils::SetSize(Visual);

				Separator();
			}

			void Separator()
			{
				ImGui::Separator();
			}
		}
	}
}
