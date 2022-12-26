#include "hashtable.h"
#include <iostream>
#include <iomanip>

using namespace std;

template <class HashedObj>
HashTable<HashedObj>::HashTable(const HashedObj& notFound, int size)
    : ITEM_NOT_FOUND(notFound), vecaray(size)
{
}

/**
         * Method that performs quadratic probing resolution.
         * Return the position where the search for x terminates.
         */
template <class HashedObj>
int HashTable<HashedObj>::findPos(const HashedObj& x) const
{
    int i = 0;   // number of collusions

    int p = hasher(x, vecaray.size()); // current position

    while (vecaray[p].info != EMPTY && vecaray[p].element != x) {

        p += 1;  // add the difference

        if (p >= vecaray.size()) {             // perform the mod
            p = p - vecaray.size();                // if necessary

        }
    }

    return p;
}

/**
         * Return true if currentPos exists and is active.
         */
template <class HashedObj>
bool HashTable<HashedObj>::isActive(int currentPos) const
{
    return vecaray[currentPos].info == ACTIVE;
}

/**
         * Remove item x from the hash table.
         *  x has to be in the table
         */
template <class HashedObj>
void HashTable<HashedObj>::remove(const HashedObj& x)
{
    int currentPos = findPos(x);

    if (isActive(currentPos))
        vecaray[currentPos].info = DELETED;
}

/**
         * Find item x in the hash table.
         * Return the matching item, or ITEM_NOT_FOUND, if not found.
         */
template <class HashedObj>
const HashedObj& HashTable<HashedObj>::find(const HashedObj& x) const
{
    int currentPos = findPos(x);

    if (isActive(currentPos))
        return vecaray[currentPos].element;

    return   ITEM_NOT_FOUND;
}

template <class HashedObj>
const bool& HashTable<HashedObj>::inTable(const HashedObj& x) const
{
    int currentPos = findPos(x);

    if (isActive(currentPos))
        return true;

    return   false;
}

/**
         * Insert item x into the hash table. If the item is
         * already present, then do nothing.
         */
template <class HashedObj>
void HashTable<HashedObj>::insert(const HashedObj& x)
{
    // Insert x as active
    int currentPos = findPos(x);

    if (isActive(currentPos))
        return;

    vecaray[currentPos] = HashEntry(x, ACTIVE);
    // enlarge the hash table if necessary 
    if (++currentSize >= vecaray.size() / 2)
        rehash();
}

/**
         * Expand the hash table.
         */
template <class HashedObj>
void HashTable<HashedObj>::rehash()
{
    vector<HashEntry> oldArray = vecaray;

    // Create new double-sized, empty table
    vecaray.resize(nextPrime(2 * oldArray.size()));

    for (int j = 0; j < vecaray.size(); j++)
        vecaray[j].info = EMPTY;

    // Copy table over
    currentSize = 0;
    for (int i = 0; i < oldArray.size(); i++)
        if (oldArray[i].info == ACTIVE)
            insert(oldArray[i].element);
}

template <class HashedObj>
int HashTable<HashedObj>::nextPrime(int n)
{
    if (n % 2 == 0)
        n++;

    for (; !isPrime(n); n += 2)
        ;

    return n;
}

template <class HashedObj>
int HashTable<HashedObj>::hasher(const KeyCodePair& element, int tableSize) const
{
    int sum = 0;

    for (int i = 0; i < element.Key.length(); i++) // add all bytes in a loop
        sum = sum + (unsigned char) element.Key[i];
    return (sum % tableSize);
}

template <class HashedObj>
int HashTable<HashedObj>::hasher(const string& key, int tableSize) const
{
    int sum = 0;

    for (int i = 0; i < key.length(); i++) // add all bytes in a loop
        sum = sum + key[i];
    return (sum % tableSize);
}

template <class HashedObj>
int HashTable<HashedObj>::hasher(const char& key, int tableSize) const
{
    int sum = 0;
    // add all bytes in a loop
    sum = sum + key;
    return (sum % tableSize);
}

template <class HashedObj>
int HashTable<HashedObj>::hasher(const int& key, int tableSize) const
{
    return (key % tableSize);
}


template <class HashedObj>
bool HashTable<HashedObj>::isPrime(int n)
{
    if (n == 2 || n == 3)
        return true;

    if (n == 1 || n % 2 == 0)
        return false;

    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0)
            return false;

    return true;
}


template <class HashedObj>
bool  operator == (const HashedObj& lhs, const HashedObj& rhs)
{
    return lhs =(rhs);
}

template <class HashedObj>
bool operator != (const HashedObj& lhs, const HashedObj& rhs)
{
    return !(lhs == rhs);
}
