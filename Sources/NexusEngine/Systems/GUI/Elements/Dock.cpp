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
			bool IsOpen = true;
			bool IsMain = GetGuiFlag(NxEn::GUI::ElementFlags::Main);
			bool FirstTime = ImGui::DockBuilderGetNode(DockId) == nullptr || Relayout;
			const ImGuiViewport* Viewport = ImGui::GetMainViewport();
			Relayout = false;

			NxFr::Vector2f Position = IsMain ? (NxFr::Vector2f)Viewport->WorkPos : NxFr::Vector2f(50.0f);
			NxFr::Vector2f Size = IsMain ? (NxFr::Vector2f)Viewport->WorkSize : (NxFr::Vector2f)Viewport->WorkSize * 0.8f;

			ImGui::SetNextWindowPos(Position, IsMain ? ImGuiCond_Always : ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(Size, IsMain ? ImGuiCond_Always : ImGuiCond_FirstUseEver);

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin(GetNamedId().C(), &IsOpen, GetImGuiFlags());
			ImGui::PopStyleVar(3);

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

			ImGui::End();

			if (!IsOpen)
			{
				Close();
			}
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
