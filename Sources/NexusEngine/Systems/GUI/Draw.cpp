#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Draw.h"

namespace NxEn
{
	namespace GUI
	{
		namespace Draw
		{
			void Label(NxFr::StringView Data, const Transform& Visual)
			{
				if (Data.IsEmpty())
				{
					return;
				}

				Utils::SetPosition(Visual.Position);

				Label(Data);

				Utils::OffsetLabel(Visual.Label);
			}

			void Label(NxFr::StringView Data)
			{
				if (Data.IsEmpty())
				{
					return;
				}

				ImGui::AlignTextToFramePadding();
				ImGui::Text(Data.C(true));
				ImGui::SameLine();
			}

			void Text(NxFr::StringView Data, const Transform& Visual)
			{
				Utils::SetPosition(Visual.Position);

				Text(Data);
			}

			void Text(NxFr::StringView Data)
			{
				ImGui::Text(Data.C(true));
			}

			void TextSelectable(NxFr::StringView Data, NxFr::StringView Id, const Transform& Visual)
			{
				Utils::SetPosition(Visual.Position);
				Utils::SetSize(Visual.Size);

				TextSelectable(Data, Id);
			}

			void TextSelectable(NxFr::StringView Data, NxFr::StringView Id)
			{
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));

				NxFr::String& Temp = NxFr::StringView::GetTempExactString(Data);
				ImGui::InputText(Utils::GenerateStringId(Id, Data).C(), Temp.Characters(), Temp.GetCapacity(), 0);

				ImGui::PopStyleColor(1);
			}

			bool Selectable(NxFr::StringView Data, bool State, const Transform& Visual)
			{
				Utils::SetPosition(Visual.Position);
				Utils::SetSize(Visual.Size);

				return Selectable(Data, State);
			}

			bool Selectable(NxFr::StringView Data, bool State)
			{
				return ImGui::Selectable(Data.C(true), State);
			}

			bool Input(NxFr::String& Data, NxFr::StringView Id, const Transform& Visual)
			{
				Utils::SetPosition(Visual.Position);
				Utils::SetSize(Visual.Size);
				
				return Input(Data, Id, Visual.Size);
			}

			bool Input(NxFr::String& Data, NxFr::StringView Id, NxFr::Vector2f Size, uint64 Flag)
			{
				if (Size.y > ImGui::GetTextLineHeight())
				{
					Flag = NxFr::Integer::SetFlag<uint64>(Flag, ImGuiInputTextFlags_Multiline, true);
					Flag = NxFr::Integer::SetFlag<uint64>(Flag, ImGuiInputTextFlags_EnterReturnsTrue, false);
				}

				if (!NxFr::Integer::CheckFlag<uint64>(Flag, ImGuiInputTextFlags_Multiline))
				{
					Size.y = 0.0f;
				}

				bool Result = ImGui::InputTextEx(Id.C(true), nullptr, Data.Characters(), Data.GetCapacity(), Size, Flag | ImGuiInputTextFlags_CallbackResize, [](ImGuiInputTextCallbackData* Callback)
				{
					if (Callback->EventFlag == ImGuiInputTextFlags_CallbackResize)
					{
						NxFr::String* Text = static_cast<NxFr::String*>(Callback->UserData);
						uint64 Requested = Callback->BufTextLen + 1;
						if (Text->GetCapacity() < Requested)
						{
							Text->Reserve(NxFr::Math::Max(Text->GetCapacity() * 2, Requested));
							Callback->Buf = Text->Characters();
						}
					}

					return 0;
				}, &Data);

				if (Result)
				{
					Data.Validate();
				}

				return Result;
			}

			bool Button(NxFr::StringView Data, const Transform& Visual)
			{
				Utils::SetPosition(Visual.Position);

				return Button(Data, Visual.Size);
			}

			bool Button(NxFr::StringView Data, NxFr::Vector2f Size)
			{
				return ImGui::Button(Data.C(true), Size);
			}

			bool CheckBox(NxFr::StringView Text, bool& Data, const Transform& Visual)
			{
				Utils::SetPosition(Visual.Position);
				Utils::SetSize(Visual.Size);

				return CheckBox(Text, Data);
			}

			bool CheckBox(NxFr::StringView Text, bool& Data)
			{
				return ImGui::Checkbox(Text.C(true), &Data);
			}

			void Space(const Transform& Visual)
			{
				Utils::SetPosition(Visual.Position);

				return Space(Visual.Size);
			}

			void Space(NxFr::Vector2f Size)
			{
				if (Size == NxFr::Vector2f::Zero)
				{
					Size = NxFr::Vector2f(0.0f, ImGui::GetTextLineHeight());
				}

				ImGui::Dummy(Size);
			}

			void Separator(const Transform& Visual)
			{
				Utils::SetPosition(Visual.Position);
				Utils::SetSize(Visual.Size);

				Separator();
			}

			void Separator()
			{
				ImGui::Separator();
			}
		}
	}
}
