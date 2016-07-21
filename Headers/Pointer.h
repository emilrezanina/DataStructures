#pragma once
#ifndef POINTER_H
#define POINTER_H

template< class T >
class AutoRefPtr
{
public:
	AutoRefPtr() { mP = NULL; }
	AutoRefPtr(T* aP) { mP = aP; if (mP) mP->reference(); }
	AutoRefPtr(T* aP, bool aDontReference) { UNREF_PAR(aDontReference) mP = aP; }
	AutoRefPtr(const AutoRefPtr<T>& aP) { mP = aP.mP; if (mP) mP->reference(); }
	~AutoRefPtr() { if (mP) mP->dereference(); }

	void operator= (const AutoRefPtr<T>& aP)
	{
		if (mP != aP.mP)
		{
			if (mP) mP->dereference();
			mP = aP.mP; if (mP) mP->reference();
		}
	}
	void operator= (T* aP)
	{
		if (mP == aP) return;
		if (mP) mP->dereference();
		mP = aP;
		if (mP) mP->reference();
	}
	bool operator== (const AutoRefPtr<T>& aP) const
	{
		return bool(mP == aP.mP);
	}
	bool operator!= (const AutoRefPtr<T>& aP) const
	{
		return bool(mP != aP.mP);
	}
	bool operator== (const T* aP) const { return bool(mP == aP); }
	bool operator!= (const T* aP) const { return bool(mP != aP); }
	bool operator< (const T* aP) const { return bool(mP<aP); }
	bool operator!() const { return mP == NULL; }
	operator bool() const { return mP != NULL; }
	operator T*() { return mP; }
	operator const T*() const { return mP; }

	const T& operator*() const { return *mP; }
	T& operator*() { return *mP; }
	const T* operator->() const { return mP; }
	T* operator->() { return mP; }

	void makeNULL() { if (mP) mP->dereference(); mP = NULL; }
	void swap(AutoRefPtr<T>& aOther) { std::swap(mP, aOther.mP); } // nothrow
private:
	T* mP;
};

#endif // !POINTER_H