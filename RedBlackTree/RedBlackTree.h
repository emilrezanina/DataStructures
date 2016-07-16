#pragma once
#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <memory>

//STRUCTURES
template<typename KEY_TYPE, typename MAPPED_TYPE>
class RedBlackTree
{
public:
	typedef KEY_TYPE key_type;
	typedef MAPPED_TYPE mapped_type;
	typedef std::pair<const key_type, mapped_type> value_type;
	class iterator;
	RedBlackTree();
	bool isEmpty() const { return mRoot == mSentinel; }
	size_t size() const { return mCount; }
	iterator insert(const key_type& key, const mapped_type& data);
	size_t remove(const key_type& key);
	iterator begin();
	iterator end();
	iterator find(const key_type& key);
private:
	struct RedBlackNode;
	size_t mCount;
	std::shared_ptr<RedBlackNode> mSentinel;
	std::shared_ptr<RedBlackNode> mRoot;
	void rotateLeft(std::shared_ptr<RedBlackNode> x);
	void rotateRight(std::shared_ptr<RedBlackNode> x);
	void restoreAfterInsert(std::shared_ptr<RedBlackNode> x);
	void restoreAfterDelete(std::shared_ptr<RedBlackNode> x);
	void remove(std::shared_ptr<RedBlackNode> node);
};

template<typename KEY_TYPE, typename MAPPED_TYPE>
struct RedBlackTree<KEY_TYPE, MAPPED_TYPE>::RedBlackNode
{
	value_type* Value;
	std::shared_ptr<RedBlackNode> Left;
	std::shared_ptr<RedBlackNode> Right;
	std::shared_ptr<RedBlackNode> Parent;
	bool IsRed;
	RedBlackNode() : Value(NULL), Left(NULL), Right(NULL), Parent(NULL), IsRed(true) {}
	RedBlackNode(const key_type& key, const mapped_type& data) : Left(NULL), Right(NULL), Parent(NULL), IsRed(true)
	{
		Value = new value_type(key, data);
	}
	~RedBlackNode() { delete Value; }
};

template<typename KEY_TYPE, typename MAPPED_TYPE>
class RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator
{
public:
	iterator();
	iterator(const iterator& src);
	iterator& operator=(const iterator& src);
	value_type& operator*();
	value_type* operator->();
	iterator& operator++();
	iterator operator++(int);
	iterator& operator--();
	iterator operator--(int);
	bool operator==(const iterator& right) const;
	bool operator!=(const iterator& right) const;
private:
	std::shared_ptr<RedBlackNode> mNode;
	bool mIsAfterLast;
	bool mIsBeforeFirst;
	std::shared_ptr<RedBlackNode> mSentinel;
	friend RedBlackTree;
	iterator(std::shared_ptr<RedBlackNode> node, bool isAfterLast, bool isBeforeFirst, std::shared_ptr<RedBlackNode> sentinel);
};

//ITERATOR METHODS
template<typename KEY_TYPE, typename MAPPED_TYPE>
RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator::iterator()
	: mNode(NULL), mIsAfterLast(true), mIsBeforeFirst(true), mSentinel(NULL) {}

template<typename KEY_TYPE, typename MAPPED_TYPE>
RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator::iterator(const iterator& src)
	: mNode(src.mNode), mIsAfterLast(src.mIsAfterLast), mIsBeforeFirst(src.mIsBeforeFirst), mSentinel(src.mSentinel) {}

template<typename KEY_TYPE, typename MAPPED_TYPE>
RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator::iterator(std::shared_ptr<RedBlackNode> node, bool isAfterLast, bool isBeforeFirst, std::shared_ptr<RedBlackNode> sentinel)
	: mNode(node), mIsAfterLast(isAfterLast), mIsBeforeFirst(isBeforeFirst), mSentinel(sentinel) {}

template<typename KEY_TYPE, typename MAPPED_TYPE>
typename RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator& RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator::operator=(const iterator& src)
{
	if (*this != src)
	{
		mNode = src.mNode;
		mIsAfterLast = src.mIsAfterLast;
		mIsBeforeFirst = src.mIsBeforeFirst;
		mSentinel = src.mSentinel;
	}
	return *this;
}

template<typename KEY_TYPE, typename MAPPED_TYPE>
typename RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator& RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator::operator++()
{
	if (mIsAfterLast)
		throw std::exception();
	if (mIsBeforeFirst)
	{
		mIsBeforeFirst = false;
		return *this;
	}
	if (mNode->Right != mSentinel)
	{
		mNode = mNode->Right;
		while (mNode->Left != mSentinel)
			mNode = mNode->Left;
	}
	else
	{
		std::shared_ptr<RedBlackNode> temp = mNode;
		while (mNode->Parent != NULL && mNode == mNode->Parent->Right)
			mNode = mNode->Parent;
		if (mNode->Parent == NULL)
		{
			mNode = temp;
			mIsAfterLast = true;
		}
		else
			mNode = mNode->Parent;
	}
	return *this;
}

template<typename KEY_TYPE, typename MAPPED_TYPE>
typename RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator::operator++(int)
{
	RedBlackTree::iterator retIt = *this;
	++(*this);
	return retIt;
}

template<typename KEY_TYPE, typename MAPPED_TYPE>
bool RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator::operator==(const iterator& right) const
{
	if (this == &right)
		return true;
	if (mIsAfterLast != right.mIsAfterLast || mIsBeforeFirst != right.mIsBeforeFirst)
		return false;
	return this->mNode == right.mNode;
}

template<typename KEY_TYPE, typename MAPPED_TYPE>
bool RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator::operator!=(const iterator& right) const
{
	if (this == &right)
		return false;
	if (mIsAfterLast != right.mIsAfterLast || mIsBeforeFirst != right.mIsBeforeFirst)
		return true;
	return this->mNode != right.mNode;
}

template<typename KEY_TYPE, typename MAPPED_TYPE>
typename RedBlackTree<KEY_TYPE, MAPPED_TYPE>::value_type& RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator::operator*()
{
	if (mIsAfterLast || mIsBeforeFirst)
		throw std::exception();
	return *mNode->Value;
}

template<typename KEY_TYPE, typename MAPPED_TYPE>
typename RedBlackTree<KEY_TYPE, MAPPED_TYPE>::value_type* RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator::operator->()
{
	if (mIsAfterLast || mIsBeforeFirst)
		throw std::exception();
	return mNode->Value;
}

template<typename KEY_TYPE, typename MAPPED_TYPE>
typename RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator& RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator::operator--()
{
	if (mIsBeforeFirst || (!mIsAfterLast && mNode->Left == mSentinel && mNode->Parent == NULL))
		throw std::exception();
	if (mIsAfterLast)
	{
		mIsAfterLast = false;
		return *this;
	}
	if (mNode->Left != mSentinel)
	{
		mNode = mNode->Left;
		while (mNode->Right != mSentinel)
			mNode = mNode->Right;
	}
	else
	{
		std::shared_ptr<RedBlackNode> temp = mNode;
		while (mNode->Parent != NULL && mNode == mNode->Parent->Left)
			mNode = mNode->Parent;
		if (mNode->Parent == NULL)
		{
			mNode = temp;
			mIsBeforeFirst = true;
		}
		else
			mNode = mNode->Parent;
	}
	return *this;
}

template<typename KEY_TYPE, typename MAPPED_TYPE>
typename RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator::operator--(int)
{
	RedBlackTree::iterator retIt = *this;
	--(*this);
	return retIt;
}

//PRIVATE METHODS
template<typename KEY_TYPE, typename MAPPED_TYPE>
void RedBlackTree<KEY_TYPE, MAPPED_TYPE>::rotateLeft(std::shared_ptr<typename RedBlackTree<KEY_TYPE, MAPPED_TYPE>::RedBlackNode> x)
{
	std::shared_ptr<RedBlackNode> y = x->Right;
	x->Right = y->Left;
	if (y->Left != mSentinel)
		y->Left->Parent = x;
	if (y != mSentinel)
		y->Parent = x->Parent;
	if (x->Parent != NULL)
	{
		if (x == x->Parent->Left)
			x->Parent->Left = y;
		else
			x->Parent->Right = y;
	}
	else
		mRoot = y;
	y->Left = x;
	if (x != mSentinel)
		x->Parent = y;
}

template<typename KEY_TYPE, typename MAPPED_TYPE>
void RedBlackTree<KEY_TYPE, MAPPED_TYPE>::rotateRight(std::shared_ptr<typename RedBlackTree<KEY_TYPE, MAPPED_TYPE>::RedBlackNode> x)
{
	std::shared_ptr<RedBlackNode> y = x->Left;
	x->Left = y->Right;
	if (y->Right != mSentinel)
		y->Right->Parent = x;
	if (y != mSentinel)
		y->Parent = x->Parent;
	if (x->Parent != NULL)
	{
		if (x == x->Parent->Right)
			x->Parent->Right = y;
		else
			x->Parent->Left = y;
	}
	else
		mRoot = y;
	y->Right = x;
	if (x != mSentinel)
		x->Parent = y;
}

template<typename KEY_TYPE, typename MAPPED_TYPE>
void RedBlackTree<KEY_TYPE, MAPPED_TYPE>::restoreAfterInsert(std::shared_ptr<typename RedBlackTree<KEY_TYPE, MAPPED_TYPE>::RedBlackNode> x)
{
	std::shared_ptr<RedBlackNode> y;
	while (x != mRoot && x->Parent != NULL && x->Parent->IsRed)
	{
		if (x->Parent == x->Parent->Parent->Left)
		{
			y = x->Parent->Parent->Right;
			if (y != NULL && y->IsRed)
			{
				x->Parent->IsRed = false;
				y->IsRed = false;
				x->Parent->Parent->IsRed = true;
				x = x->Parent->Parent;
			}
			else
			{
				if (x == x->Parent->Right)
				{
					x = x->Parent;
					rotateLeft(x);
				}
				x->Parent->IsRed = false;
				x->Parent->Parent->IsRed = true;
				rotateRight(x->Parent->Parent);
			}
		}
		else
		{
			y = x->Parent->Parent->Left;
			if (y != NULL && y->IsRed)
			{
				x->Parent->IsRed = false;
				y->IsRed = false;
				x->Parent->Parent->IsRed = true;
				x = x->Parent->Parent;
			}
			else
			{
				if (x == x->Parent->Left)
				{
					x = x->Parent;
					rotateRight(x);
				}
				x->Parent->IsRed = false;
				x->Parent->Parent->IsRed = true;
				rotateLeft(x->Parent->Parent);
			}
		}
	}
	mRoot->IsRed = false;
}

template<typename KEY_TYPE, typename MAPPED_TYPE>
void RedBlackTree<KEY_TYPE, MAPPED_TYPE>::restoreAfterDelete(std::shared_ptr<typename RedBlackTree<KEY_TYPE, MAPPED_TYPE>::RedBlackNode> x)
{
	std::shared_ptr<RedBlackNode> y;
	while (x != mRoot && !x->IsRed)
	{
		if (x == x->Parent->Left)
		{
			y = x->Parent->Right;
			if (y->IsRed)
			{
				y->IsRed = false;
				x->Parent->IsRed = true;
				rotateLeft(x->Parent);
				y = x->Parent->Right;
			}
			if (!y->Left->IsRed && !y->Right->IsRed)
			{
				y->IsRed = true;
				x = x->Parent;
			}
			else
			{
				if (!y->Right->IsRed)
				{
					y->Left->IsRed = false;
					y->IsRed = true;
					rotateRight(y);
					y = x->Parent->Right;
				}
				y->IsRed = x->Parent->IsRed;
				x->Parent->IsRed = false;
				y->Right - IsRed = false;
				rotateLeft(x->Parent);
				x = mRoot;
			}
		}
		else
		{
			y = x->Parent->Left;
			if (y->IsRed)
			{
				y->IsRed = false;
				x->Parent->IsRed = true;
				rotateRight(x->Parent);
				y = x->Parent->Left;
			}
			if (!y->Right->IsRed && !y->Left->IsRed)
			{
				y->IsRed = true;
				x = x->Parent;
			}
			else
			{
				if (!y->Left->IsRed)
				{
					y->Right->IsRed = false;
					y->IsRed = true;
					rotateLeft(y);
					y = x->Parent->Left;
				}
				y->IsRed = x->Parent->IsRed;
				x->Parent->IsRed = false;
				y->Left->IsRed = false;
				rotateRight(x->Parent);
				x = mRoot;
			}
		}
	}
	x->isRed = false;
}

template<typename KEY_TYPE, typename MAPPED_TYPE>
void RedBlackTree<KEY_TYPE, MAPPED_TYPE>::remove(std::shared_ptr<typename RedBlackTree<KEY_TYPE, MAPPED_TYPE>::RedBlackNode> node)
{
	RedBlackNode* x;
	RedBlackNode* y;
	if (node->Left == mSentinel || node->Right == mSentinel)
		y = node;
	else
	{
		y = node->Right;
		while (y->Left != mSentinel)
			y = y->Left;
	}
	x = y->Left != mSentinel ? y->Left : y->Right;
	x->Parent = y->Parent;
	if (y->Parent != NULL)
		if (y == y->Parent->Left)
			y->Parent->Left = x;
		else
			y->Parent->Right = x;
	else
		mRoot = x;
	if (y != node)
	{
		node->Value = y->Value;
	}
	if (!y->IsRed)
		restoreAfterDelete(x);
}

//RED BLACK TREE METHODS
template<typename KEY_TYPE, typename MAPPED_TYPE>
RedBlackTree<KEY_TYPE, MAPPED_TYPE>::RedBlackTree() : mCount(0)
{
	RedBlackNode* sentinel = new RedBlackNode();
	mSentinel = std::shared_ptr<RedBlackNode>(sentinel);
	mSentinel->IsRed = false;
	mRoot = mSentinel;
}

template<typename KEY_TYPE, typename MAPPED_TYPE>
typename RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator RedBlackTree<KEY_TYPE, MAPPED_TYPE>::insert(const key_type& key, const mapped_type& data)
{
	RedBlackNode* newItem = new RedBlackNode(key, data);
	std::shared_ptr<RedBlackNode> node(newItem);
	std::shared_ptr<RedBlackNode> temp = mRoot;
	while (temp != mSentinel)
	{
		node->Parent = temp;
		if (key == temp->Value->first)
			return iterator(temp, false, false, mSentinel);
		else if (key > temp->Value->first)
			temp = temp->Right;
		else
			temp = temp->Left;
	}
	node->Left = mSentinel;
	node->Right = mSentinel;
	if (node->Parent != NULL)
	{
		if (node->Value->first > node->Parent->Value->first)
			node->Parent->Right = node;
		else
			node->Parent->Left = node;
	}
	else
		mRoot = node;
	restoreAfterInsert(node);
	iterator it = iterator(node, false, false, mSentinel);
	mCount++;
	return it;
}

template<typename KEY_TYPE, typename MAPPED_TYPE>
size_t RedBlackTree<KEY_TYPE, MAPPED_TYPE>::remove(const key_type& key)
{
	std::shared_ptr<RedBlackNode> node;
	int result = 0;
	node = mRoot;
	while (node != mSentinel)
	{
		result = key.compare(node->Value->first);
		if (result == 0)
			break;
		node = result < 0 ? node->Left : node->Right;
	}
	if (node == mSentinel)
		return 0;
	mCount--;
	remove(node);
	return 1;
}

template<typename KEY_TYPE, typename MAPPED_TYPE>
typename RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator RedBlackTree<KEY_TYPE, MAPPED_TYPE>::begin()
{
	std::shared_ptr<RedBlackNode> node = mRoot;
	if (node == mSentinel)
		return iterator(mSentinel, true, true, mSentinel);
	while (node->Left != mSentinel)
		node = node->Left;
	return iterator(node, false, false, mSentinel);
}

template<typename KEY_TYPE, typename MAPPED_TYPE>
typename RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator RedBlackTree<KEY_TYPE, MAPPED_TYPE>::end()
{
	std::shared_ptr<RedBlackNode> node = mRoot;
	if (node == mSentinel)
		return iterator(mSentinel, true, true, mSentinel);
	while (node->Right != mSentinel)
		node = node->Right;
	return iterator(node, true, false, mSentinel);
}

template<typename KEY_TYPE, typename MAPPED_TYPE>
typename RedBlackTree<KEY_TYPE, MAPPED_TYPE>::iterator RedBlackTree<KEY_TYPE, MAPPED_TYPE>::find(const key_type& key)
{
	if (mRoot == mSentinel)
		return iterator(mRoot, true, true, mSentinel);
	std::shared_ptr<RedBlackNode> node = mRoot;
	while (node != mSentinel)
	{
		int result = key.compare(node->Value->first);
		if (result == 0)
		{
			return iterator(node, false, false, mSentinel);
		}
		node = result < 0 ? node->Left : node->Right;
	}
	return end();
}
#endif // !RED_BLACK_TREE_H
