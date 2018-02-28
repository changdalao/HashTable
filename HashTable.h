#pragma once

#include<stdio.h>
#include<malloc.h>
#include<assert.h>
#include<stdlib.h>

typedef int KeyType;
typedef int ValueType;

enum Status {
	EMPTY,DELEATE,EXIT,
};
typedef struct HashNode
{
	KeyType	_key;
	ValueType _value;
	enum Status _status;
}HashNode;
typedef struct HashTable {
	HashNode* _table;
	size_t size;//元素个数
	size_t N;//大小
}HashTable;



void HashTableInit(HashTable* ht);
int HashTableInsert(HashTable* ht, KeyType key, ValueType value);
HashNode* HashTableFind(HashTable* ht, KeyType key);
int HashTableRemove(HashTable* ht, KeyType key);
int HashTableDestery(HashTable* ht);
size_t GetNextPrimeNum(size_t num);


size_t GetNextPrimeNum(size_t num)
{
	const int _PrimeSize = 28;
	static const unsigned long _PrimeList[] =
	{
		53ul,97ul,193ul,389ul,769ul,1543ul,3079ul, 
		6151ul,12289ul,24593ul,49157ul,98317ul,196613ul,393241ul,
		786433ul,1572869ul,3145739ul,6291469ul,12582917ul, 25165843ul,
		50331653ul,100663319ul,201326611ul, 402653189ul, 805306457ul,
		1610612741ul, 3221225473ul, 4294967291ul
	};
	for (size_t i = 0; i < _PrimeSize; i++)
	{
		if (_PrimeList[i] > num)
			return _PrimeList[i];
	}
}
void HashTableInit(HashTable* ht)
{
	assert(ht);
	ht->size = 0;
	ht->N = 5;
	//开辟空间
	ht->_table= (HashNode*)malloc(sizeof(HashNode)*ht->N);
	for (size_t i = 0; i < ht->N; i++)
	{
		ht->_table[i]._status = EMPTY;
	}
}
int HashFunc(KeyType key,size_t N)
{
	return key % N;
}
void HashPrint(HashTable* ht)
{
	for (size_t i = 0; i < ht->N; i++)
	{
		if (ht->_table[i]._status == EXIT)
		{
			printf("【%d】 ", ht->_table[i]._key);//有数值
		}
		else if (ht->_table[i]._status == EMPTY)//无数值
			printf("[E] ");
		else //删除过
			printf("<D> ");
	}
	printf("\n");
}
HashNode* HashTableFind(HashTable* ht, KeyType key)
{
	assert(ht);
	size_t index = HashFunc(key, ht->N);
	while (ht->_table[index]._status != EMPTY)
	{
		if (ht->_table[index]._status == EXIT)
		{
			if (ht->_table[index]._key == key)
				return ht->_table[index]._key;
		}
		index++;
	}
	return NULL;
}
int HashTableInsert(HashTable* ht, KeyType key, ValueType value)
{
	assert(ht);
	size_t index = 0;
	if (ht->size * 10 / ht->N > 7)//扩容（哈希负载因子0.7）
	{
		size_t NewN = GetNextPrimeNum(ht->N);
		HashNode* NewTable= (HashNode*)malloc(sizeof(HashNode)*NewN);
		assert(NewTable);
		//初始化新表
		for (size_t i = 0; i < NewN; i++)
			NewTable[i]._status = EMPTY;
		//入新表
		for (size_t i = 0; i < ht->N; i++)
		{
			if (ht->_table[i]._status == EXIT)//旧表有元素入新表
			{
				size_t NewIndex = HashFunc(ht->_table[i]._key, NewN);
				while (NewTable[NewIndex]._status == EMPTY || NewTable[index]._key != ht->_table[i]._key)//解决哈希冲突。不同数值对应一个index
				{
					if (NewTable[NewIndex]._status == EMPTY)
					{
						NewTable[NewIndex]._key = ht->_table[i]._key;
						NewTable[NewIndex]._status = EXIT;
						break;
					}
					else if (NewTable[index]._key != ht->_table[i]._key)
						++NewIndex;
					if (NewIndex == NewN)
						NewIndex = 0;
				}
			}
		}
		ht->N = NewN;
		ht->_table = NewTable;
	}
	//判断所插位置
	index = HashFunc(key, ht->N);
	while (ht->_table[index]._status == EMPTY || ht->_table[index]._key != key)
	{
		if (ht->_table[index]._status == EMPTY)
		{
			ht->_table[index]._key = key;
			ht->_table[index]._status = EXIT;
			ht->size++;
			return 0;
		}
		if (ht->_table[index]._key != key)
			++index;
		if (index == ht->N)
			index = 0;
	}
}
int HashTableRemove(HashTable* ht, KeyType key)
{
	assert(ht);
	size_t index = HashFunc(key, ht->N);
	while (ht->_table[index]._status != EMPTY)
	{
		if (ht->_table[index]._key == key)
		{
			ht->_table[index]._status = DELEATE;
			ht->size--;
			return 0;
		}
		index++;
	}
	return -1;
}
int HashTableDestery(HashTable* ht)
{
	assert(ht);
	ht->size = 0;
	ht->N = 0;
	free(ht->_table);
	ht->_table = NULL;
}

void TestHast()
{
	HashTable ht;
	HashTableInit(&ht);
	HashTableInsert(&ht, 53, 0);
	HashTableInsert(&ht, 106, 0);
	HashTableInsert(&ht, 54, 0);
	HashTableInsert(&ht, 3, 0);
	HashTableInsert(&ht, 212, 0);
	HashTableInsert(&ht, 68, 0);
	HashTableInsert(&ht, 8, 0);
	HashTableInsert(&ht, 10, 0);
	HashTableInsert(&ht, 9, 0);
	HashPrint(&ht);

	printf("查找53->%d\n", HashTableFind(&ht, 53));
	printf("查找106->%d\n", HashTableFind(&ht, 106));
	printf("查找54->%d\n", HashTableFind(&ht, 54));
	printf("查找3->%d\n", HashTableFind(&ht, 3));
	printf("查找212->%d\n", HashTableFind(&ht, 212));
	printf("查找68->%d\n", HashTableFind(&ht, 68));
	printf("查找10->%d\n", HashTableFind(&ht, 10));
	printf("查找9->%d\n", HashTableFind(&ht, 9));

	HashTableRemove(&ht, 53);
	HashTableRemove(&ht, 106);
	HashTableRemove(&ht, 10);
	HashTableRemove(&ht, 212);
	HashTableRemove(&ht, 4);
	HashTableRemove(&ht, 9);
	HashPrint(&ht);

	printf("查找10->%d\n", HashTableFind(&ht, 10));
	printf("查找9->%d\n", HashTableFind(&ht, 9));

	HashTableDestery(&ht);
	HashPrint(&ht);
}