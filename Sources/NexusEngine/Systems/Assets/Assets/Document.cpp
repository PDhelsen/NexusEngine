#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/Assets/Assets/Document.h"

namespace NxEn
{
	NX_ASSET(Document)

	void Document::OnDraw()
	{
		Asset::OnDraw();

		GUI::Drawer<uint64>::Property(Data.GetCount(), "Length");
	}

	void Document::OnSave(YAML::Node& Node, NxFr::StringView ContentFsPath)
	{
		NxFr::File File = NxFr::File(ContentFsPath);
		File.Create(true);
		File.WriteText(Data);
		File.Close();
	}

	void Document::OnLoad(const YAML::Node& Node, NxFr::StringView ContentFsPath)
	{
		NxFr::File File = NxFr::File(ContentFsPath);
		File.Open(NxFr::File::Mode::Read);
		Data = File.ReadText();
		File.Close();
	}

	void Document::OnUnload()
	{
		Data = NxFr::String();
	}

	void Document::OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids) const
	{
	}
}
