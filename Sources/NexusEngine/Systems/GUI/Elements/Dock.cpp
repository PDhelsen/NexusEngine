#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Elements/Dock.h"

namespace NxEn
{
	namespace GUI
	{
		Dock::Dock()
			: Items(), Layout(Mode::FirstTime), Relayout(false)
		{
		}

		Dock::~Dock()
		{
		}

		void Dock::Draw()
		{
			if (!IsEnabled())
			{
				return;
			}

			NxFr::GUID DockId = ImGui::GetID(GetNamedId().C());
			bool FirstTime = ImGui::DockBuilderGetNode(DockId) == nullptr || Relayout;
			Relayout = false;

			ImGui::DockSpace(DockId, NxFr::Vector2f::Zero, ImGuiDockNodeFlags_PassthruCentralNode);

			if (Items.GetCount() > 0 && (
				Layout == Mode::Always || (Layout == Mode::FirstTime && FirstTime))
				)
			{
				auto DockElement = [=](uint32 Node, const Item& It)
				{
					if (It.Ratio > 0.0f)
					{
						uint32 Opposite = 0;
						ImGui::DockBuilderSplitNode(DockId, It.Direction, It.Ratio, &Node, &Opposite);
					}

					ImGui::DockBuilderDockWindow(It.Name.C(), Node);
					return Node;
				};

				uint32 Node = DockElement(DockId, Items[0]);
				for (uint64 Index = 1; Index < Items.GetCount(); ++Index)
				{
					Node = DockElement(Node, Items[Index]);
				}

				ImGui::DockBuilderFinish(DockId);
			}

			OnDraw();
		}

		void Dock::DockElement(Element* Instance, ImGuiDir Direction, float Ratio, bool Force)
		{
			auto It = NxFr::ContainerUtility::Where<Item>(Items, [=](const Item& Other) { return Other.Name == Instance->GetNamedId(); });
			if (It != Items.End())
			{
				auto& Item = Items[It.Id()];
				Item.Direction = Direction;
				Item.Ratio = Ratio;

			}
			else
			{
				Items.AppendConstruct(Instance->GetNamedId(), Direction, Ratio);
			}

			Relayout = Force;
		}

		void Dock::ClearElements()
		{
			Items.Clear();
		}

		void Dock::OnInitialize()
		{
			Element::OnInitialize();

			SetGuiFlag(NxEn::GUI::ElementFlags::AutoDraw, false);
		}
	}
}
