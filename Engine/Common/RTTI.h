#pragma once

#include "Common/Common.h"

// ��� ���迡 �ִ� Ŭ���� ����
// ����(�����߿�) ����ȯ�� �ϱ� ���� ����ϴ� Ŭ����.
// RunTime-Type-Information.
namespace Wanted
{
	// Ŀ���� RTTI�� �����ϴ� Ŭ������ �ֻ��� Ŭ����.
	// C#�� Object, Java�� object, 
	// �𸮾��� UObject�� �����غ� �� ����.
	class WANTED_API RTTI
	{
	public:
		virtual const size_t& GetType() const = 0;

		virtual bool Is(RTTI* const rtti) const
		{
			return false;
		}

		virtual bool Is(const size_t id) const
		{
			return false;
		}

		template<typename T>
		bool IsTypeOf()
		{
			return Is(T::TypeIdClass());
		}

		template<typename T>
		T* As()
		{
			if (Is(T::TypeIdClass()))
			{
				return (T*)this;
			}

			return nullptr;
		}

		template<typename T>
		const T* As() const
		{
			if (Is(T::TypeIdClass()))
			{
				return (T*)this;
			}

			return nullptr;
		}
	};
}

// RTTI�� ������ Ŭ������ �߰��� ��ũ��.
// �Ʒ� �ڵ忡�� Type, ParentType�� ���� Ÿ������ ��ȯ�Ǿ� ����/�ٿ��ֱ� ��.
#define RTTI_DECLARATIONS(Type, ParentType)												\
friend class RTTI;																		\
protected:																				\
	static const size_t TypeIdClass()													\
	{																					\
		static int runTimeTypeId = 0;													\
		return reinterpret_cast<size_t>(&runTimeTypeId);								\
	}																					\
public:																					\
	virtual const size_t& GetType() const override { return Type::TypeIdClass(); }		\
	using super = ParentType;															\
	virtual bool Is(const size_t id) const override										\
	{																					\
		if (id == TypeIdClass())														\
		{																				\
			return true;																\
		}																				\
		else																			\
		{																				\
			return ParentType::Is(id);													\
		}																				\
	}																					\
	virtual bool Is(RTTI* const rtti) const override									\
	{																					\
		return Is(rtti->GetType());														\
	}