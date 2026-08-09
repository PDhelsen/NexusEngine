#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Draw.h"
#include "NexusEngine/Systems/GUI/Misc/Styles.h"

namespace NxEn
{
	namespace GUI
	{
		template<>
		struct Drawer<YAML::Node>
		{
			static void Property(const YAML::Node& Data, NxFr::StringView Label = "", const Transform& Visual = {})
			{
				NxFr::String ImGuiId = Utils::GenerateStringId("", Label);
				ImGui::PushID(ImGuiId.C());

				Transform NodeVisual = Visual;
				NodeVisual.Label = -1.0f;
				
				switch (Data.Type())
				{
					case YAML::NodeType::Undefined:
					{
						NxFr::StringView NodeValue = "<unknown>";
						Drawer<NxFr::StringView>::Property(NodeValue, Label, NodeVisual);
						break;
					}
					case YAML::NodeType::Null:
					{
						NxFr::StringView NodeValue = "";
						Drawer<NxFr::StringView>::Property(NodeValue, Label, NodeVisual);
						break;
					}
					case YAML::NodeType::Scalar:
					{
						NxFr::String NodeValue = Data.as<NxFr::String>();
						Drawer<NxFr::String>::Property(NodeValue, Label, NodeVisual);
						break;
					}
					case YAML::NodeType::Sequence:
					{
						if (ImGui::TreeNode(Label.C()))
						{
							for (uint64 Index = 0; Index < Data.size(); ++Index)
							{
								YAML::Node NodeValue = Data[Index];
								NxFr::String NodeLabel = "- [" + NxFr::StringUtility::ToString(Index) + "]";
								Drawer<YAML::Node>::Property(NodeValue, NodeLabel, NodeVisual);
							}
							ImGui::TreePop();
						}
						break;
					}
					case YAML::NodeType::Map:
					{
						if (ImGui::TreeNode(Label.C()))
						{
							for (auto It = Data.begin(); It != Data.end(); ++It)
							{
								YAML::Node NodeValue = It->second;
								NxFr::String NodeLabel = "- " + It->first.as<NxFr::String>();
								Drawer<YAML::Node>::Property(NodeValue, NodeLabel, NodeVisual);
							}
							ImGui::TreePop();
						}
						break;
					}
				}

				ImGui::PopID();
			}

			static bool Field(YAML::Node& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Transform& Visual = {})
			{
				bool Result = false;

				NxFr::String ImGuiId = Utils::GenerateStringId("", Label);
				ImGui::PushID(ImGuiId.C());

				Transform NodeVisual = Visual;
				NodeVisual.Label = -1.0f;

				switch (Data.Type())
				{
					case YAML::NodeType::Undefined:
					{
						NxFr::String NodeValue = "<unknown>";
						Result |= Drawer<NxFr::String>::Field(NodeValue, Label, "", NodeVisual);
						if (Result)
						{
							Data = NodeValue;
						}
						break;
					}
					case YAML::NodeType::Null:
					{
						NxFr::String NodeValue = "";
						Result |= Drawer<NxFr::String>::Field(NodeValue, Label, "", NodeVisual);
						if (Result)
						{
							Data = NodeValue;
						}
						break;
					}
					case YAML::NodeType::Scalar:
					{
						NxFr::String NodeValue = Data.as<NxFr::String>();
						Result |= Drawer<NxFr::String>::Field(NodeValue, Label, "", NodeVisual);
						if (Result)
						{
							Data = NodeValue;
						}
						break;
					}
					case YAML::NodeType::Sequence:
					{
						if (ImGui::TreeNode(Label.C()))
						{
							for (uint64 Index = 0; Index < Data.size(); ++Index)
							{
								YAML::Node NodeValue = Data[Index];
								NxFr::String NodeLabel = "- [" + NxFr::StringUtility::ToString(Index) + "]";
								Result |= Drawer<YAML::Node>::Field(NodeValue, NodeLabel, "", NodeVisual);
							}
							ImGui::TreePop();
						}
						break;
					}
					case YAML::NodeType::Map:
					{
						if (ImGui::TreeNode(Label.C()))
						{
							for (auto It = Data.begin(); It != Data.end(); ++It)
							{
								YAML::Node NodeValue = It->second;
								NxFr::String NodeLabel = "- " + It->first.as<NxFr::String>();
								Result |= Drawer<YAML::Node>::Field(NodeValue, NodeLabel, "", NodeVisual);
							}
							ImGui::TreePop();
						}
						break;
					}
				}

				ImGui::PopID();

				return Result;
			}
		};
	}
}
