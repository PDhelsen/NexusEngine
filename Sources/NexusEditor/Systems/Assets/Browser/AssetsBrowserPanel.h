#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEd
{
	class AssetsBrowserPanel : public NxEn::GUI::Panel
	{
		enum class InfoType
		{
			Directory,
			File,
			Asset
		};

		struct Selection
		{
			bool State;
			uint64 From;
			uint64 To;
			uint64 Flag;
		};

		struct Info
		{
			NxFr::String Path;
			NxFr::StringView Label;
			InfoType Type;
			uint64 Depth;
			int64 Jump;
			bool Expand;
			bool Selected;

			Info(NxFr::StringView Path, uint64 Depth);
			void Update(NxFr::StringView FilePath);
		};

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, AssetsBrowserPanel)

		NEXUS_EDITOR_API AssetsBrowserPanel();
		NEXUS_EDITOR_API ~AssetsBrowserPanel();

		NEXUS_EDITOR_API void Refresh();

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnEnable() override;
		NEXUS_EDITOR_API void OnGui(float TimeStep) override;

	private:
		void DrawHeader();

		void FetchFolder(uint64 Depth = 0, NxFr::StringView Path = "");
		void DrawFolder(uint64& Index);
		NxFr::String& GenerateImGuiLabel(const Info& Instance);

		void ApplySelection();
		void ApplySearch();

	private:
		NxEn::InputSystem* Inputs;

		NxEn::GUI::Style Style;
		NxFr::String Buffer;

		NxFr::List<Info> Infos;
		Selection Select;
		NxFr::String Selected;
		NxFr::Set<NxFr::String> Search;
		NxFr::String Filter;
	};
}
