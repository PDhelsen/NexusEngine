#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

#define NX_OBJECT_DECLARATION(Dll, Type)\
Dll virtual NxFr::StringId GetObjectType() const;\
Dll static NxFr::StringId GetClassType();

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
	class Object
	{
	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, Object)

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

		NX_ENGINE_API Object();
		NX_ENGINE_API virtual ~Object();

		NX_ENGINE_API bool operator==(const Object& Other) const;
		NX_ENGINE_API bool operator!=(const Object& Other) const;

		NX_ENGINE_API virtual void Initialize();
		NX_ENGINE_API virtual void Shutdown();
		NX_ENGINE_API virtual void Tick(float TimeStep = 0.0f);
		NX_ENGINE_API virtual void DrawGui(float TimeStep = 0.0f);

		NX_ENGINE_API virtual bool IsInitialized() const;
		NX_ENGINE_API virtual bool IsEnabled() const;
		NX_ENGINE_API virtual void SetEnabled(bool Enabled);
		NX_ENGINE_API virtual bool IsTickable() const;
		NX_ENGINE_API virtual void SetTickable(bool Tickable);
		NX_ENGINE_API virtual bool IsTicking() const;

		NX_ENGINE_API virtual Object* Clone() const;
		NX_ENGINE_API virtual void Clone(Object* Other) const;
		NX_ENGINE_API virtual void Clone(const Object* Other);
		NX_ENGINE_API virtual YAML::Node Save();
		NX_ENGINE_API virtual void Load(const YAML::Node& Node);
		NX_ENGINE_API virtual void Unload();
		NX_ENGINE_API virtual NxFr::Array<NxFr::GUID> GetDependencies();

		NX_ENGINE_API virtual NxFr::StringView GetName() const;
		NX_ENGINE_API virtual NxFr::GUID GetId() const;

	protected:
		NX_ENGINE_API virtual void OnInitialize();
		NX_ENGINE_API virtual void OnShutdown();
		NX_ENGINE_API virtual void OnEnable();
		NX_ENGINE_API virtual void OnDisable();
		NX_ENGINE_API virtual void OnTick(float TimeStep = 0.0f);
		NX_ENGINE_API virtual void OnGui(float TimeStep = 0.0f);

		NX_ENGINE_API virtual void OnClone(const Object& Other);
		NX_ENGINE_API virtual void OnSave(YAML::Node& Node);
		NX_ENGINE_API virtual void OnLoad(const YAML::Node& Node);
		NX_ENGINE_API virtual void OnUnload();
		NX_ENGINE_API virtual void OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids);

	protected:
		NX_ENGINE_API NX_FORCE_INLINE bool GetFlag(ObjectFlags Flag) const;
		NX_ENGINE_API NX_FORCE_INLINE void SetFlag(ObjectFlags Flag, bool Value);

	private:
		ObjectFlags Flags;
	};
}
