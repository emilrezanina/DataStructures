#include <gtest\gtest.h>
#include <RedBlackTree\RedBlackTree.h>
#include <list>



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
	EXPECT_EQ(key, rbTree.size());
	EXPECT_TRUE(!rbTree.isEmpty());
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
	EXPECT_EQ(replicationsCount, rbTree.size());
	EXPECT_TRUE(!rbTree.isEmpty());
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