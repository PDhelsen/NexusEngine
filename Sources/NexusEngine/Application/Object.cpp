#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Object.h"

namespace NxEn
{
	NEXUS_OBJECT_IMPLEMENTATION(Object)

	Object::Object()
		: Flags(ObjectFlags::None)
	{
	}

	Object::~Object()
	{
	}

	bool Object::operator==(const Object& Other) const
	{
		return GetId() == Other.GetId();
	}

	bool Object::operator!=(const Object& Other) const
	{
		return !(*this == Other);
	}

	void Object::Initialize()
	{
		if (IsInitialized())
		{
			return;
		}

		OnInitialize();

		SetFlag(ObjectFlags::Initialized, true);
	}

	void Object::Shutdown()
	{
		if (!IsInitialized())
		{
			return;
		}

		OnShutdown();

		SetFlag(ObjectFlags::Initialized, false);
	}

	void Object::Tick(float TimeStep)
	{
		if (!IsTicking())
		{
			return;
		}

		OnTick(TimeStep);
	}

	void Object::DrawGui(float TimeStep)
	{
		OnGui(TimeStep);
	}

	bool Object::IsInitialized() const
	{
		return GetFlag(ObjectFlags::Initialized);
	}

	bool Object::IsEnabled() const
	{
		return GetFlag(ObjectFlags::Enabled);
	}

	void Object::SetEnabled(bool Enabled)
	{
		if (IsEnabled() == Enabled)
		{
			return;
		}

		SetFlag(ObjectFlags::Enabled, Enabled);

		if (IsEnabled())
		{
			OnEnable();
		}
		else
		{
			OnDisable();
		}
	}

	bool Object::IsTickable() const
	{
		return GetFlag(ObjectFlags::Tickable);
	}

	void Object::SetTickable(bool Tickable)
	{
		SetFlag(ObjectFlags::Tickable, Tickable);
	}

	bool Object::IsTicking() const
	{
		return IsEnabled() && IsTickable();
	}

	Object* Object::Clone() const
	{
		NEXUS_ASSERT(false, Default, "Not Implemented");
		return nullptr;
	}

	void Object::Clone(Object* Other) const
	{
		Other->Clone(this);
	}

	void Object::Clone(const Object* Other)
	{
		OnClone(*Other);
	}

	YAML::Node Object::Save()
	{
		YAML::Node Node;
		OnSave(Node);
		return Node;
	}

	void Object::Load(const YAML::Node& Node)
	{
		OnLoad(Node);
	}

	void Object::Unload()
	{
		OnUnload();
	}

	NxFr::Array<NxFr::GUID> Object::GetDependencies()
	{
		NxFr::Set<NxFr::GUID> Ids;
		OnGetDependencies(Ids);
		return NxFr::ContainerUtility::ToArray<NxFr::GUID>(Ids);
	}

	NxFr::StringView Object::GetName() const
	{
		return GetObjectType().GetString();
	}

	NxFr::GUID Object::GetId() const
	{
		return reinterpret_cast<NxFr::GUID>(this);
	}

	void Object::OnInitialize()
	{
	}

	void Object::OnShutdown()
	{
		
	}

	void Object::OnEnable()
	{
	}

	void Object::OnDisable()
	{
	}

	void Object::OnTick(float TimeStep)
	{
	}

	void Object::OnGui(float TimeStep)
	{
		bool Enabled = IsEnabled();
		GUI::Drawer<bool>::Field(Enabled);
		if (Enabled != IsEnabled())
		{
			SetEnabled(Enabled);
		}

		ImGui::SameLine();

		GUI::Drawer<NxFr::String>::Property(GetName(), "Name");

		ImGui::SameLine();

		GUI::Drawer<NxFr::GUID>::Property(GetId());
	}

	void Object::OnClone(const Object& Other)
	{
		Flags = Other.Flags;
	}

	void Object::OnSave(YAML::Node& Node)
	{
	}

	void Object::OnLoad(const YAML::Node& Node)
	{
	}

	void Object::OnUnload()
	{
	}

	void Object::OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids)
	{
	}

	bool Object::GetFlag(ObjectFlags Flag) const
	{
		return NxFr::Enum::CheckFlag(Flags, Flag);
	}

	void Object::SetFlag(ObjectFlags Flag, bool Value)
	{
		Flags = NxFr::Enum::SetFlag(Flags, Flag, Value);
	}
}
