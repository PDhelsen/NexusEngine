#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Elements/Popup.h"

namespace NxEn
{
	namespace GUI
	{
		Popup::Popup()
			: Message(), Items()
		{
		}

		Popup::~Popup()
		{
		}

		void Popup::Draw()
		{
			if (!IsEnabled())
			{
				return;
			}

			ImGui::SetNextWindowSize(NxFr::Vector2f::Zero, ImGuiCond_Always);

			ImGui::OpenPopup(GetNamedId().C());
			if (ImGui::BeginPopupModal(GetNamedId().C(), nullptr, GetImGuiFlags()))
			{
				if (!Message.IsEmpty())
				{
					ImGui::SetCursorPosX(GUI::Utils::Center(Message).x);
					ImGui::Text(Message.C());
				}

				OnDraw();

				if (Items.GetCount() > 0)
				{
					ImGui::SetCursorPosX(GUI::Utils::Center(NxFr::Vector2f(GUI::Styles::WidthButton() * Items.GetCount(), 0), Items.GetCount() - 1).x);
					for (uint64 Index = 0; Index < Items.GetCount(); ++Index)
					{
						Item& Button = Items[Index];
						if (ImGui::Button(Button.Label.C(), { GUI::Styles::WidthButton(), 0.0f }))
						{
							if (!Button.Callback.IsNull())
							{
								Button.Callback.Invoke();
							}

							Close();

							ImGui::CloseCurrentPopup();
						}

						ImGui::SameLine();
					}
				}
			}
			ImGui::EndPopup();
		}

		void Popup::SetMessage(NxFr::StringView Message)
		{
			this->Message = Message;
		}

		void Popup::AddButton(NxFr::StringView Label)
		{
			AddButton(Label, nullptr);
		}

		void Popup::AddButton(NxFr::StringView Label, const NxFr::Delegate<void()>& Callback)
		{
			Items.AppendConstruct(Callback, Label);
		}

		void Popup::Clear()
		{
			Items.Clear();
		}

		void Popup::OnInitialize()
		{
			Element::OnInitialize();

			SetGuiFlag(ElementFlags::RecycleInsteadOfClose, true);
			SetImGuiFlag(ImGuiWindowFlags_NoCollapse, true);
			SetImGuiFlag(ImGuiWindowFlags_NoDocking, true);
		}

		void Popup::OnShutdown()
		{
			Clear();

			Element::OnShutdown();
		}
	}
}
