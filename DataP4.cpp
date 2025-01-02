#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <vector>

struct Node {
    char* key;
    int value;
    Node* next;

    // Constructor to initialize a Node with a key, value, and next pointer
    Node(const char* k, int v) : key(_strdup(k)), value(v), next(nullptr) {}

    // Destructor to free memory when a Node is deleted
    ~Node() {
        free(key);
    }
};

// Hash table class using separate chaining for collision resolution
template <size_t TableSize>
class MyHashTable {
private:
    Node* buckets[TableSize];
    Node* lastBucket;

     //Custom hash function for char*
    size_t hashFunction(const char* key) const {
        size_t hash = 5381;
        int c;

        // Iterate through each character in the key
        while ((c = *key++)) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }

        // Take the modulo to ensure the hash fits within the hash table size
        return hash % TableSize;
    }

    //size_t hashFunction(const char* key) const {
    //    size_t hash = 0; // Initial hash value

    //    // Iterate through each character in the key
    //    while (*key) {
    //        // Update the hash using a simple formula: hash * 31 + current character
    //        hash = (hash * 31) + *key++;
    //    }

    //    // Take the modulo to ensure the hash fits within the hash table size
    //    return hash % TableSize;
    //}

    //size_t hashFunction(const char* key) const {
    //    size_t hash = 0; // Initial hash value

    //    // Iterate through each character in the key
    //    while (*key) {
    //        // Update the hash using a simple formula: hash * 31 + current character
    //        hash = (hash * 31) ;
    //        key++;
    //    }

    //    // Take the modulo to ensure the hash fits within the hash table size
    //    return hash % TableSize;
    //  }

public:
    // Constructor
    MyHashTable() {
        // Initialize buckets with nullptr
        for (size_t i = 0; i < TableSize; ++i) {
            buckets[i] = nullptr;
        }
    }

    // Destructor to free memory
    ~MyHashTable() {
        for (size_t i = 0; i < TableSize; ++i) {
            Node* current = buckets[i];
            while (current) {
                Node* next = current->next;
                delete current;
                current = next;
            }
        }
    }

    // Insert a key-value pair into the hash table
    void insert(const char* key, int value) {

        if (!get(key, value)) {
            // If the key is not already in the hash table, insert a new node
            size_t bucketIndex = hashFunction(key);

            // Create a new node
            Node* newNode = new Node(key, value);

            // Insert at the beginning of the linked list
            newNode->next = lastBucket;
            buckets[bucketIndex] = newNode;
            lastBucket = newNode;
        }
    }

    // Retrieve the value associated with a key
    bool get(const char* key, int value) const {
        size_t bucketIndex = hashFunction(key);
        Node* current = buckets[bucketIndex];

        // Search for the key in the linked list
        while (current) {
            if (strcmp(current->key, key) == 0) {
                // Key found, return the associated value
                value = current->value;
                current->value += 1;
                return true;
            }
            else {
                // TODO: What should happen if the key is not found?
                return false;
            }
            current = current->next;
        }

        // Key not found
        return false;
    }

    // Print the linked list
    void printLinkedList() const {
        Node* current;
        current = lastBucket;

        long cnt = 0;
        long cntd = 0;

        while (current) {
            std::cout << "(" << current->key << "), (" << current->value << ") \n";
            cnt += current->value;
            current = current->next;
            cntd++;
        }

        std::cout << std::endl;
    }

    // Print the ordered list
    void printOrderedList() const {
        // Create a vector to store pointers to the nodes
        std::vector<Node*> nodes;

        Node* current;
        current = lastBucket;

        // Populate the vector with nodes
        while (current) {
            nodes.push_back(current);
            current = current->next;
        }

        // Sort the nodes based on their values in descending order
        std::sort(nodes.begin(), nodes.end(), [](const Node* a, const Node* b) {
            return a->value > b->value;
            });

        // Open a file for writing the ordered list
        FILE* fp = NULL;
        char buffer[1000];
        fp = fopen("C:\\Temp\\access_log\\access_log_ordered2", "w+t");

        int i = 0;
        // Print the sorted nodes to the file
        for (const Node* node : nodes) {
            if (i < 10) {
                sprintf(buffer, "%s%s%s%d%s", "(", node->key, "), (", node->value, ") \n");
                fwrite(buffer, sizeof(char), strlen(buffer), fp);
                i++;
            }
            else
                break;
        }

        // Close the file
        fclose(fp);
        std::cout << std::endl;
    }
};

// Function to extract a substring from the input based on specific patterns
bool extractSubstring(const char* input, char* output, size_t outputSize) {
    const char* startPtr = strstr(input, "\"GET");

    if (startPtr != nullptr) {
        const char* endPtr = strstr(startPtr + 1, "\"");

        if (endPtr != nullptr) {
            size_t substringLength = endPtr - (startPtr + 1);

            // Check if output buffer is large enough
            if (substringLength < outputSize - 1) {
                strncpy(output, startPtr + 1, substringLength);
                output[substringLength] = '\0';  // Null-terminate the substring
                return true;
            }
            else {
                return false; // Output buffer is too small
            }
        }
        else {
            return false; // Closing double quote not found
        }
    }
    else {
        return false; // Opening double quote not found
    }

    return false;
}

// Main function
int main() {
    FILE* stream = nullptr;
    fopen_s(&stream, "C:\\Temp\\access_log\\access_log", "r+");
    if (stream == nullptr) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }

    char buffer[1000];
    const size_t tableSize = 100000000;  // Set the table size directly as a constant
    MyHashTable<tableSize>* myHashTable = new MyHashTable<tableSize>;
    char fileName[300] = {};  // Initialize the buffer with zeros

    DWORD startTickCount = GetTickCount();

    // Read data from the file, extract substrings, and insert into the hash table
    while (fgets(buffer, sizeof(buffer), stream)) {
        if (extractSubstring(buffer, fileName, sizeof(fileName))) {
            myHashTable->insert(fileName, 1);
        }
    }

    // Close the input file
    fclose(stream);

    // Measure the elapsed time
    DWORD elapsedTime = GetTickCount() - startTickCount;

    // Print the first GetTickCount value
    printf("Parse file and populate hastable -  GetTickCount %d\n", elapsedTime);

    // Sort and print the ordered list to a file
    myHashTable->printOrderedList();

    // Measure the elapsed time again
    elapsedTime = GetTickCount() - startTickCount;

    // Print the second GetTickCount value
    printf("Order and Save the top ten list -  GetTickCount %d\n", elapsedTime);

    // Cleanup
    //delete myHashTable;

    return 0;
}
