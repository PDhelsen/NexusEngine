#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Debug/StatsPanel.h"

namespace NxEn
{
	static char FilterBuffer[256] = "";
	static NxFr::StringView FilterExclude[] = { NxFr::StatsHeader::TickId.C(), NxFr::StatsHeader::CommentId.C() };

	NEXUS_OBJECT_IMPLEMENTATION(StatsPanel)

	StatsPanel::StatsPanel()
		: Statistiques(nullptr), Filters(), Ids(), Values()
	{
	}

	StatsPanel::~StatsPanel()
	{
	}

	void StatsPanel::SetStats(const NxFr::Stats* Stats)
	{
		Statistiques = Stats;
		Values = Stats->GetAllCurrentStats();

		Ids = NxFr::List<const NxFr::String*>(Values.GetCount());
		for (auto& [Id, Value] : Values)
		{
			Ids.Append(&Id.ToString());
		}
		Ids.Sort();
	}

	const NxFr::Stats* StatsPanel::GetStats()
	{
		return Statistiques;
	}

	void StatsPanel::OnInitialize()
	{
		Panel::OnInitialize();

		Title = "Statistiques";
		GuiFlags = NxFr::Integer::SetFlag(GuiFlags, ImGuiWindowFlags_MenuBar, false);
	}

	void StatsPanel::OnGui(float TimeStep)
	{
		DrawFilter();

		for (auto Label : Ids)
		{
			if (!FilterStats(*Label))
			{
				continue;
			}

			DrawStats(*Label);
		}
	}

	void StatsPanel::DrawFilter()
	{
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Filter:");
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::InputText("##Filter", FilterBuffer, IM_ARRAYSIZE(FilterBuffer));
		ImGui::PopItemWidth();
		ImGui::Separator();

		Filters = NxFr::StringUtility::SplitAll(FilterBuffer, ",");
	}

	void StatsPanel::DrawStats(const NxFr::String& Label) const
	{
		NxFr::StringId Id = NxFr::StringId(Label);
		const NxFr::Stats::Stat* Value = Values[Id];

		switch (Value->GetType())
		{
		case NxFr::Stats::StatType::Label: ImGui::Text("%s: %s", Label.C(), Value->GetValue<const NxFr::String&>().C()); break;
		case NxFr::Stats::StatType::Check: ImGui::Text("%s: %s", Label.C(), Value->GetValue<bool>() ? "true" : "false"); break;
		case NxFr::Stats::StatType::Integer: ImGui::Text("%s: %d", Label.C(), Value->GetValue<uint64>()); break;
		case NxFr::Stats::StatType::UnsignedInteger: ImGui::Text("%s: %d", Label.C(), Value->GetValue<uint64>()); break;
		case NxFr::Stats::StatType::Decimal: ImGui::Text("%s: %f", Label.C(), Value->GetValue<float>()); break;
		case NxFr::Stats::StatType::DecimalPrecision: ImGui::Text("%s: %f", Label.C(), Value->GetValue <double>()); break;
		}
	}

	bool StatsPanel::FilterStats(const NxFr::String& Label)
	{
		for (auto F : FilterExclude)
		{
			if (Label == F)
			{
				return false;
			}
		}

		if (Filters.IsEmpty())
		{
			return true;
		}
		
		for (auto F : Filters)
		{
			if (Label.Contains(F))
			{
				return true;
			}
		}

		return false;
	}
}
