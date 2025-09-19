#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Styles.h"

namespace NxEn
{
	namespace GUI
	{
		Style::Style()
			: Properties()
		{
		}

		Style::~Style()
		{
		}

		void Style::Push() const
		{
			for (auto& [Id, Prop] : Properties)
			{
				Apply(Id, Prop.Flag, Prop.Data);
			}
		}

		void Style::Pop() const
		{
			ImGui::PopStyleColor(Counts.x);
			ImGui::PopStyleVar(Counts.y);
		}

		void Style::Append(int32 Id, Type Flag, NxFr::Vector4f Data)
		{
			Property* Prop = Properties.TryGet(Id);
			if (Prop != nullptr)
			{
				if (Prop->Flag == Type::VarX && Flag == Type::VarY)
				{
					Prop->Flag = Type::VarXY;
					Prop->Data.y = Data.y;
				}
				else if (Prop->Flag == Type::VarY && Flag == Type::VarX)
				{
					Prop->Flag = Type::VarXY;
					Prop->Data.x = Data.x;
				}
				else if (Prop->Flag == Type::VarXY && Flag == Type::VarY)
				{
					Prop->Data.y = Data.y;
				}
				else if (Prop->Flag == Type::VarXY && Flag == Type::VarX)
				{
					Prop->Data.x = Data.x;
				}
				else
				{
					Prop->Data = Data;
				}
				return;
			}

			if (Flag == Type::Color) Counts.x++; else Counts.y++;
			Properties.Append(Id, { Flag, Data });
		}

		void Style::Remove(int32 Id)
		{
			if (Properties[Id].Flag == Type::Color) Counts.x--; else Counts.y--;
			Properties.Remove(Id);
		}

		void Style::Apply(int32 Id, Type Flag, NxFr::Vector4f Data) const
		{
			switch (Flag)
			{
			case Type::Color: ImGui::PushStyleColor(Id, NxFr::Color(Data)); break;
			case Type::Var: ImGui::PushStyleVar(Id, Data.x); break;
			case Type::VarX: ImGui::PushStyleVarX(Id, Data.x); break;
			case Type::VarY: ImGui::PushStyleVarY(Id, Data.y); break;
			case Type::VarXY: ImGui::PushStyleVar(Id, { Data.x, Data.y }); break;
			default: NEXUS_ASSERT(false, Default, "ImGui unsupported style var index: %d", Flag); break;
			}
		}

		namespace Styles
		{
			NxFr::Dictionary<NxFr::StringId, GUI::Style> Styles;
			NxFr::Dictionary<NxFr::StringId, NxFr::Color> Colors;
			NxFr::Dictionary<NxFr::StringId, float> Vars;
		}

		namespace Scope
		{
			Style::Style(NxFr::StringId Id)
				: Value(GUI::Styles::Styles[Id])
			{
				Value.Push();
			}

			Style::Style(const::NxEn::GUI::Style& Value)
				: Value(Value)
			{
				Value.Push();
			}

			Style::~Style()
			{
				Value.Pop();
			}

			Color::Color(ImGuiCol Id, NxFr::Color Value)
			{
				ImGui::PushStyleColor(Id, Value);
			}

			Color::~Color()
			{
				ImGui::PopStyleColor(1);
			}

			Var::Var(ImGuiStyleVar Id, float Value)
			{
				ImGui::PushStyleVar(Id, Value);
			}

			Var::Var(ImGuiStyleVar Id, float Value, uint8 Index)
			{
				switch (Index)
				{
				case 0: ImGui::PushStyleVarX(Id, Value); break;
				case 1: ImGui::PushStyleVarY(Id, Value); break;
				default: NEXUS_ASSERT(false, Default, "ImGui unsupported style var index: %d", Index); break;
				}
			}

			Var::Var(ImGuiStyleVar Id, NxFr::Vector2f Value)
			{
				ImGui::PushStyleVar(Id, Value);
			}

			Var::~Var()
			{
				ImGui::PopStyleVar(1);
			}

			Width::Width(float Size)
			{
				ImGui::PushItemWidth(Size);
			}

			Width::~Width()
			{
				ImGui::PopItemWidth();
			}
		}

		namespace Utils
		{
			NxFr::Vector2f Fill(NxFr::Vector2f Offset, float Count, bool IncludePadding, bool Window)
			{
				NxFr::Vector2f ContentArea = ImGui::GetContentRegionAvail();
				NxFr::Vector2f WindowArea = NxFr::Vector2f(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
				NxFr::Vector2f Total = Window ? WindowArea : ContentArea;
				NxFr::Vector2f Spacing = ImGui::GetStyle().ItemSpacing;
				NxFr::Vector2f Padding = ImGui::GetStyle().FramePadding;
				return Total - Offset - Spacing * Count - (IncludePadding ? Padding : NxFr::Vector2f::Zero);
			}
		}
	}
}
