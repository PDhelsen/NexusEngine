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
			case Type::Color: ImGui::PushStyleColor(Id, Data); break;
			case Type::Var: ImGui::PushStyleVar(Id, Data.x); break;
			case Type::VarX: ImGui::PushStyleVarX(Id, Data.x); break;
			case Type::VarY: ImGui::PushStyleVarY(Id, Data.y); break;
			case Type::VarXY: ImGui::PushStyleVar(Id, { Data.x, Data.y }); break;
			default: NEXUS_ASSERT(false, Default, "ImGui unsupported style var index: %d", Flag); break;
			}
		}

		namespace Scope
		{
			Color::Color(ImGuiCol Id, NxFr::Vector4f Value)
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

			Style::Style(NxFr::StringId Id)
				: Value(Application::GetSystem<GUISystem>()->GetStyle(Id))
			{
				Value.Push();
			}

			Style::Style(const::NxEn::GUI::Style Value)
				: Value(Value)
			{
				Value.Push();
			}

			Style::~Style()
			{
				Value.Pop();
			}
		}
	}
}

