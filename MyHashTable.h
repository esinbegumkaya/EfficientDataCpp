#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

struct Node {
    char* key;
    int value;
    Node* next;

    Node(const char* k, int v) : key(_strdup(k)), value(v), next(nullptr) {}

    ~Node() {
        free(key);
    }
};

template <size_t TableSize>
class MyHashTable {
private:
    Node* buckets[TableSize];
    Node* lastBucket;

    size_t hashFunction(const char* key) const;

public:
    MyHashTable();

    ~MyHashTable();

    void insert(const char* key, int value);

    bool get(const char* key, int value) const;

    void printLinkedList() const;

    void printOrderedList() const;
};


