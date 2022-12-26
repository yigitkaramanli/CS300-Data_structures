#ifndef HASHTABLE_H
#define HASHTABLE_H

#pragma once

#include <iostream>
#include <string>
#include <vector>
using namespace std;

/*Struct to hold key and code values*/
struct KeyCodePair
{
    string Key;
    int code;

    KeyCodePair() {};
    KeyCodePair(const string & key, const int & code)
        :Key(key), code(code)
    {

    };

    bool operator==(const KeyCodePair& rhs) const {
        return this->Key == rhs.Key;
    };

    bool operator!=(const KeyCodePair& rhs) const {
        return this->Key != rhs.Key;
    };

};

/*Class Taken from the lecture slides.
I didn't use most of the functions*/
template <class HashedObj>
class HashTable
{
public:
    explicit HashTable(const HashedObj& notFound, int size = 4096);
    HashTable(const HashTable& rhs)
        : ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND),
        vecaray(rhs.aray), currentSize(rhs.currentSize) { }

    const HashedObj& find(const HashedObj& x) const;
    const bool& inTable(const HashedObj& x) const;//ACIKLAMA YAZ

    void insert(const HashedObj& x);
    void remove(const HashedObj& x);

    //const HashTable& operator=(const HashTable& rhs);
    
    enum EntryType { ACTIVE, EMPTY, DELETED };
private:
    struct HashEntry
    {
        HashedObj element; //Key ??
        EntryType info;

        HashEntry(const HashedObj& e = HashedObj(), EntryType i = EMPTY)
            : element(e), info(i) { }
    };

    vector<HashEntry> vecaray;
    int currentSize;
    const HashedObj ITEM_NOT_FOUND;
    bool isActive(int currentPos) const;
    int findPos(const HashedObj& x) const;
    void rehash();

    bool isPrime(int n);
    int nextPrime(int n);
    int hasher(const string& key, int tablesize)const;
    int hasher(const KeyCodePair& element, int tablesize)const;
    int hasher(const char& key, int tablesize)const;
    int hasher(const int& key, int tablesize)const;

};
template <class HashedObj>
bool operator ==  (const HashedObj& lhs, const HashedObj& rhs);

template <class HashedObj>
bool operator !=  (const HashedObj& lhs, const HashedObj& rhs);

#include "HashTable.cpp"
#endif