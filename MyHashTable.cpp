#pragma once


#include "MyHashTable.h"  // Include the header file


MyHashTable<TableSize>::MyHashTable() {
    for (size_t i = 0; i < TableSize; ++i) {
        buckets[i] = nullptr;
    }
}


MyHashTable<TableSize>::~MyHashTable() {
    for (size_t i = 0; i < TableSize; ++i) {
        Node* current = buckets[i];
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
}


void MyHashTable<TableSize>::insert(const char* key, int value) {
    if (!get(key, value)) {
        size_t bucketIndex = hashFunction(key);
        Node* newNode = new Node(key, value);
        newNode->next = lastBucket;
        buckets[bucketIndex] = newNode;
        lastBucket = newNode;
    }
}


bool MyHashTable<TableSize>::get(const char* key, int value) const {
    size_t bucketIndex = hashFunction(key);
    Node* current = buckets[bucketIndex];

    while (current) {
        if (strcmp(current->key, key) == 0) {
            value = current->value;
            current->value += 1;
            return true;
        }
        else {
            return false;
        }
        current = current->next;
    }

    return false;
}


void MyHashTable<TableSize>::printLinkedList() const {
    Node* current;
    current = lastBucket;

    while (current) {
        std::cout << "(" << current->key << "), (" << current->value << ") \n";
        current = current->next;
    }

    std::cout << std::endl;
}


void MyHashTable<TableSize>::printOrderedList() const {
    std::vector<Node*> nodes;
    Node* current;
    current = lastBucket;

    while (current) {
        nodes.push_back(current);
        current = current->next;
    }

    std::sort(nodes.begin(), nodes.end(), [](const Node* a, const Node* b) {
        return a->value > b->value;
        });

    FILE* fp = nullptr;
    char buffer[1000];
    fp = fopen("C:\\Users\\sami.ozen\\Downloads\\wetransfer_proje_2023-12-16_2104\\access_log\\access_log_ordered2", "w+t");

    for (const Node* node : nodes) {
        sprintf(buffer, "%s%s%s%d%s", "(", node->key, "), (", node->value, ") \n");
        fwrite(buffer, sizeof(char), strlen(buffer), fp);
    }

    fclose(fp);
    std::cout << std::endl;
}

template <size_t TableSize>
size_t MyHashTable<TableSize>::hashFunction(const char* key) const {
    size_t hash = 5381;
    int c;

    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash % TableSize;
}

