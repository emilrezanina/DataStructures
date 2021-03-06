#include <gtest\gtest.h>
#include <RedBlackTree\RedBlackTree.h>
#include <list>

#define DEFAULT_START_IDX 0
#define DEFAULT_END_IDX 10

typedef RedBlackTree<int, std::string> IntStringRBTree;

inline void fillIntStringRBTreeWithAscendingRange(IntStringRBTree& aTree, size_t aStartIdx, size_t aEndIdx)
{
	for (size_t i = aStartIdx; i < aEndIdx; i++)
	{
		std::string value("Item_");
		value.append(std::to_string(i));
		aTree.insert(i, value);
	}
}

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

TEST(RED_BLACK_TREE, CreateEmptyTreeTest)
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
	EXPECT_EQ(key, rbTree.size());
	EXPECT_TRUE(!rbTree.isEmpty());
}

TEST(RED_BLACK_TREE, InsertSomeItemsTest)
{
	IntStringRBTree rbTree;
	srand(time(NULL));
	size_t replicationsCount = rand() % 100;
	std::cout << "Replications count: " << replicationsCount << std::endl;
	fillIntStringRBTreeWithAscendingRange(rbTree, DEFAULT_START_IDX, replicationsCount);
	EXPECT_EQ(replicationsCount, rbTree.size());
	EXPECT_TRUE(!rbTree.isEmpty());
}

TEST(RED_BLACK_TREE, InsertItemsWithSameKeyDoesntRewriteItemTest)
{
	IntStringRBTree rbTree;
	int key = 0;
	std::string firstInsert("First Insert");
	std::string secondInsert("Second Insert");
	rbTree.insert(key, firstInsert);
	rbTree.insert(key, secondInsert);
	ASSERT_EQ(1, rbTree.size());
	IntStringRBTree::iterator beginIt = rbTree.begin();
	ASSERT_TRUE(beginIt->second.compare(firstInsert) == 0);
	ASSERT_FALSE(beginIt->second.compare(secondInsert) == 0);
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

TEST(RED_BLACK_TREE, BeginAndEndIsSameForEmptyTreeTest)
{
	IntStringRBTree rbTree;
	IntStringRBTree::iterator beginIt = rbTree.begin();
	IntStringRBTree::iterator endIt = rbTree.end();
	ASSERT_TRUE(beginIt == endIt);
}

TEST(RED_BLACK_TREE, RangeIteratorTest)
{
	IntStringRBTree rbTree;
	fillIntStringRBTreeWithAscendingRange(rbTree, DEFAULT_START_IDX, DEFAULT_END_IDX);
	IntStringRBTree::iterator itBegin, itEnd, itStep;
	itBegin = rbTree.begin();
	itEnd = rbTree.end();
	size_t counter = 0;
	std::for_each(itBegin, itEnd, [&counter](const IntStringRBTree::value_type& value) { ++counter;});
	ASSERT_EQ(rbTree.size(), counter);
}

TEST(RED_BLACK_TREE, ReverseRangeIteratorTest)
{
	IntStringRBTree rbTree;
	fillIntStringRBTreeWithAscendingRange(rbTree, DEFAULT_START_IDX, DEFAULT_END_IDX);
	IntStringRBTree::iterator itBegin, itEnd, itStep;
	itBegin = rbTree.begin();
	itEnd = rbTree.end();
	size_t counter = 0;
	std::for_each(itBegin, itEnd, [&counter](const IntStringRBTree::value_type& value) { ++counter;});
	ASSERT_EQ(rbTree.size(), counter);
}

TEST(RED_BLACK_TREE, RemoveSingletonTest)
{
	IntStringRBTree rbTree;
	int key = 0;
	std::string value("Singleton");
	rbTree.insert(key, value);
	size_t removedItemsCount = rbTree.remove(key);
	EXPECT_EQ(1, removedItemsCount);
	EXPECT_TRUE(rbTree.isEmpty());
	EXPECT_EQ(0, rbTree.size());
}

TEST(RED_BLACK_TREE, RemoveFirstFromTwinsTest)
{
	IntStringRBTree rbTree;
	int firstKey = 0;
	std::string firstValue("First");
	rbTree.insert(firstKey, firstValue);
	int secondKey = 1;
	std::string secondValue("Second");
	rbTree.insert(secondKey, secondValue);
	size_t removedItemsCount = rbTree.remove(firstKey);
	EXPECT_EQ(1, removedItemsCount);
	EXPECT_FALSE(rbTree.isEmpty());
	EXPECT_EQ(1, rbTree.size());
	IntStringRBTree::iterator begin = rbTree.begin();
	ASSERT_EQ(secondKey, begin->first);
	ASSERT_TRUE(secondValue.compare(begin->second) == 0);
	ASSERT_EQ(rbTree.end(), ++begin);
}

TEST(RED_BLACK_TREE, RemoveSecondFromTwinsTest)
{
	IntStringRBTree rbTree;
	int firstKey = 0;
	std::string firstValue("First");
	rbTree.insert(firstKey, firstValue);
	int secondKey = 1;
	std::string secondValue("Second");
	rbTree.insert(secondKey, secondValue);
	size_t removedItemsCount = rbTree.remove(secondKey);
	EXPECT_EQ(1, removedItemsCount);
	EXPECT_FALSE(rbTree.isEmpty());
	EXPECT_EQ(1, rbTree.size());
	IntStringRBTree::iterator begin = rbTree.begin();
	ASSERT_EQ(firstKey, begin->first);
	ASSERT_TRUE(firstValue.compare(begin->second) == 0);
	ASSERT_EQ(rbTree.end(), ++begin);
}

TEST(RED_BLACK_TREE, RemoveUnknownKeyReturnZeroTest)
{
	IntStringRBTree rbTree;
	int unknownKey = -1;
	size_t removedItemsCount = rbTree.remove(unknownKey);
	EXPECT_EQ(0, removedItemsCount);
	int doubleRemovedKey = 0;
	std::string value("value");
	rbTree.insert(doubleRemovedKey, value);
	removedItemsCount = rbTree.remove(unknownKey);
	EXPECT_EQ(0, removedItemsCount);
	rbTree.remove(doubleRemovedKey);
	removedItemsCount = rbTree.remove(doubleRemovedKey);
	EXPECT_EQ(0, removedItemsCount);
}

TEST(RED_BLACK_TREE, MultipleRemoveItemsTest)
{
	IntStringRBTree rbTree;
	size_t replicationsCount = 5;
	std::vector<IntStringRBTree::key_type> insertedKeys;
	for (size_t i = 0; i < replicationsCount; i++)
	{
		std::string value("Item_");
		value.append(std::to_string(i));
		insertedKeys.push_back(i);
		rbTree.insert(i, value);
	}
	srand(1);
	size_t expectedCount = rbTree.size();
	while (!insertedKeys.empty())
	{
		size_t index = rand() % insertedKeys.size();
		int key = insertedKeys[index];
		insertedKeys.erase(insertedKeys.begin() + index);
		expectedCount--;
		ASSERT_EQ(1, rbTree.remove(key)) << "Item with key " << key << " was not removed." << std::endl << "Expected count: " << expectedCount << ".";
		ASSERT_EQ(expectedCount, rbTree.size());
	}
	EXPECT_TRUE(rbTree.isEmpty());
	EXPECT_EQ(0, rbTree.size());
}

TEST(RED_BLACK_TREE, NotFoundItemTest)
{
	IntStringRBTree rbTree;
	int noFoundKey = -1;
	IntStringRBTree::iterator noFoundItemIt = rbTree.find(noFoundKey);
	ASSERT_EQ(rbTree.end(), noFoundItemIt);
	fillIntStringRBTreeWithAscendingRange(rbTree, DEFAULT_START_IDX, DEFAULT_END_IDX);
	noFoundItemIt = rbTree.find(noFoundKey);
	ASSERT_EQ(rbTree.end(), noFoundItemIt);
}

TEST(RED_BLACK_TREE, FindItemTest)
{
	IntStringRBTree rbTree;
	int foundKey = rand() % 10;
	std::string foundValue("Found item");
	rbTree.insert(foundKey, foundValue);
	IntStringRBTree::iterator foundItemIt = rbTree.find(foundKey);
	ASSERT_FALSE(foundItemIt == rbTree.end());
	ASSERT_TRUE(foundItemIt->second.compare(foundValue) == 0);
	fillIntStringRBTreeWithAscendingRange(rbTree, DEFAULT_START_IDX, DEFAULT_END_IDX);
	ASSERT_FALSE(foundItemIt == rbTree.end());
	ASSERT_TRUE(foundItemIt->second.compare(foundValue) == 0);
}

TEST(RED_BLACK_TREE, UseStdFunctionOnTreeTest)
{
	IntStringRBTree rbTree;
	int searchedKey = -1;
	std::string searchedValue = "Found!";
	rbTree.insert(searchedKey, searchedValue);
	fillIntStringRBTreeWithAscendingRange(rbTree, DEFAULT_START_IDX, DEFAULT_END_IDX);
	std::pair<int, std::string> pair(searchedKey, searchedValue);
	IntStringRBTree::iterator foundIt = rbTree.find(searchedKey);
	IntStringRBTree::iterator stdFoundIt = std::find(rbTree.begin(), rbTree.end(), IntStringRBTree::value_type(searchedKey, searchedValue));
	IntStringRBTree::iterator endIt = rbTree.end();
	ASSERT_FALSE(foundIt == endIt);
	ASSERT_FALSE(stdFoundIt == endIt);
	ASSERT_EQ(foundIt, stdFoundIt);
}