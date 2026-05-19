#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

#define NX_OBJECT_DECLARATION(Type)\
virtual NxFr::StringId GetObjectType() const;\
static NxFr::StringId GetClassType();

#define NX_OBJECT_IMPLEMENTATION(Type) \
NxFr::StringId Type::GetObjectType() const \
{\
	return Type::GetClassType();\
}\
NxFr::StringId Type::GetClassType()\
{\
	static NxFr::StringId T = #Type##_Sid; \
	return T; \
}

namespace NxEn
{
	enum class ObjectFlags : uint8
	{
		None = 0,

		Initialized = 1 << 0,
		Enabled = 1 << 1,
		Tickable = 1 << 2,

		Flag_1 = 1 << 3,
		Flag_2 = 1 << 4,
		Flag_3 = 1 << 5,
		Flag_4 = 1 << 6,
		Flag_5 = 1 << 7,
	};
}

NX_FLAG(NxEn::ObjectFlags, uint8)

namespace NxEn
{
	class NX_ENGINE_API Object
	{
	public:
		NX_OBJECT_DECLARATION(Object)

		template<typename T>
		static T* Create(bool Enable = true)
		{
			T* Instance = new T();
			Instance->Initialize();
			Instance->SetEnabled(Enable);
			return Instance;
		}

		template<typename T>
		static T* Destroy(T* Instance)
		{
			if (Instance == nullptr)
			{
				return nullptr;
			}

			Instance->SetEnabled(false);
			Instance->Shutdown();
			delete Instance;
			return nullptr;
		}

		Object();
		virtual ~Object();

		bool operator==(const Object& Other) const;
		bool operator!=(const Object& Other) const;

		virtual void Initialize();
		virtual void Shutdown();
		virtual void Tick(float TimeStep = 0.0f);
		virtual void DrawGui(float TimeStep = 0.0f);

		virtual bool IsInitialized() const;
		virtual bool IsEnabled() const;
		virtual void SetEnabled(bool Enabled);
		virtual bool IsTickable() const;
		virtual void SetTickable(bool Tickable);
		virtual bool IsTicking() const;

		virtual Object* Clone() const;
		virtual void Clone(Object* Other) const;
		virtual void Clone(const Object* Other);
		virtual YAML::Node Save();
		virtual void Load(const YAML::Node& Node);
		virtual void Unload();
		virtual NxFr::Array<NxFr::GUID> GetDependencies();

		virtual NxFr::StringView GetName() const;
		virtual NxFr::GUID GetId() const;

	protected:
		virtual void OnInitialize();
		virtual void OnShutdown();
		virtual void OnEnable();
		virtual void OnDisable();
		virtual void OnTick(float TimeStep = 0.0f);
		virtual void OnGui(float TimeStep = 0.0f);

		virtual void OnClone(const Object& Other);
		virtual void OnSave(YAML::Node& Node);
		virtual void OnLoad(const YAML::Node& Node);
		virtual void OnUnload();
		virtual void OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids);

	protected:
		NX_FORCE_INLINE bool GetFlag(ObjectFlags Flag) const;
		NX_FORCE_INLINE void SetFlag(ObjectFlags Flag, bool Value);

	private:
		ObjectFlags Flags;
	};
}
