#pragma once
#ifndef MUTEX_H
#define MUTEX_H

#include <atomic>

#define UNREF_PAR(par) (void)par

/// Operations on this type are atomic so is usable to implement thread safe reference counters
class AtomicRefCount
{
public:
	AtomicRefCount() : mCount(1) {}
	AtomicRefCount(const AtomicRefCount &aS) : mCount(1) { UNREF_PAR(aS); }
	AtomicRefCount(int aInit) : mCount(aInit) {}

	/// get current value
	int get() const { return mCount; }
	/// set current value
	void set(int aValue) { mCount = aValue; }

	/// decrement and return true if zero
	bool dereference()
	{
		return !dec();
	}

	void reference()
	{
		++mCount;
	}
	/// -- and return final value
	int dec()
	{
		return --mCount;
	}
	/// add and return final value
	int add(int aValue)
	{
		return (mCount += aValue);
	}
private:
	std::atomic_int_fast32_t mCount;

	void operator = (int aValue);
};

/// Prefered RefCount is AtomicRefCount
typedef AtomicRefCount RefCount;


#endif // !MUTEX_H