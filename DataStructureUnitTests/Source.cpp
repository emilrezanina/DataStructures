#include <gtest\gtest.h>
#include <RedBlackTree\RedBlackTree.h>



TEST(RED_BLACK_TREE, InnerTypesTest)
{
	typedef int KEY_TYPE;
	typedef std::string MAPPED_TYPE;
	typedef std::pair<const KEY_TYPE, MAPPED_TYPE> VALUE_TYPE;
	typedef	RedBlackTree<KEY_TYPE, MAPPED_TYPE> IntStringRedBlackTree;
	EXPECT_TRUE(typeid(KEY_TYPE) == typeid(IntStringRedBlackTree::key_type));
	EXPECT_TRUE(typeid(MAPPED_TYPE) == typeid(IntStringRedBlackTree::mapped_type));
	EXPECT_TRUE(typeid(VALUE_TYPE) == typeid(IntStringRedBlackTree::value_type));
}

typedef RedBlackTree<int, std::string> IntStringRBTree;

TEST(RED_BLACK_TREE, CreateTest)
{
	IntStringRBTree rbTree;
	EXPECT_TRUE(rbTree.isEmpty());
	EXPECT_EQ(0, rbTree.size());
}

TEST(RED_BLACK_TREE, InsertItemTest)
{
	int key = 1;
	IntStringRBTree rbTree;
	rbTree.insert(key, "SingleItem");
	ASSERT_EQ(key, rbTree.size());
	ASSERT_TRUE(!rbTree.isEmpty());
}

TEST(RED_BLACK_TREE, InsertSomeItemsTest)
{
	IntStringRBTree rbTree;
	srand(time(NULL));
	size_t replicationsCount = rand() % 100;
	std::cout << "Replications count: " << replicationsCount << std::endl;
	for (size_t i = 0; i < replicationsCount; i++)
	{
		std::string value("Item_");
		value.append(std::to_string(i));
		rbTree.insert(i, value);
	}
	ASSERT_EQ(replicationsCount, rbTree.size());
	ASSERT_TRUE(!rbTree.isEmpty());
}

TEST(RED_BLACK_TREE, BasicIteratorTest)
{
	int key = 1;
	std::string value("FirstItem");
	IntStringRBTree rbTree;
	rbTree.insert(key, value);
	IntStringRBTree::iterator itBegin = rbTree.begin();
	IntStringRBTree::iterator itEnd = rbTree.end();
	ASSERT_EQ(key, itBegin->first);
	ASSERT_TRUE(value.compare(itBegin->second) == 0);
	ASSERT_FALSE(itBegin == itEnd);
	IntStringRBTree::iterator itBeforeEnd = --itEnd;
	ASSERT_EQ(itBegin, itBeforeEnd);
}

TEST(RED_BLACK_TREE, RangeIteratorTest)
{
	IntStringRBTree rbTree;
	size_t replicationsCount = 10;
	for (size_t i = 0; i < replicationsCount; i++)
	{
		std::string value("Item_");
		value.append(std::to_string(i));
		rbTree.insert(i, value);
	}
	IntStringRBTree::iterator itBegin, itEnd, itStep;
	itBegin = rbTree.begin();
	itEnd = rbTree.end();
	size_t counter = 0;
	for (itStep = itBegin; itStep != itEnd; ++itStep)
	{
		counter++;
	}
	ASSERT_EQ(rbTree.size(), counter);
}

TEST(RED_BLACK_TREE, ReverseRangeIteratorTest)
{
	IntStringRBTree rbTree;
	size_t replicationsCount = 10;
	for (size_t i = 0; i < replicationsCount; i++)
	{
		std::string value("Item_");
		value.append(std::to_string(i));
		rbTree.insert(i, value);
	}
	IntStringRBTree::iterator itBegin, itEnd, itStep;
	itBegin = rbTree.begin();
	itEnd = rbTree.end();
	size_t counter = 0;
	for (itStep = itEnd; itStep != itBegin; --itStep)
	{
		counter++;
	}
	ASSERT_EQ(rbTree.size(), counter);
}