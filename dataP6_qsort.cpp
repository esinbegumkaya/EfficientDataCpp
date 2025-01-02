#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <unordered_map>
#include <string>
#include <windows.h>
#include <chrono>
#include <ctime>
#include <algorithm>

struct Pair {
    std::string key;
    int value;
};

int comparePairs(const void* a, const void* b) {
    return static_cast<const Pair*>(b)->value - static_cast<const Pair*>(a)->value;
}

bool extractSubstring(const std::string& input, std::string& output) {
    
    size_t start = input.find("\"GET");
    size_t end = input.find('"', start + 1);

    if (start != std::string::npos && end != std::string::npos) {
        output = input.substr(start + 4, end - start - 4);
        return true;
    }

    return false;
}

int main() {
    
    // Record the start time
    auto start = std::chrono::system_clock::now();
    std::time_t startTime = std::chrono::system_clock::to_time_t(start);

    FILE* fp;
    if ((fp = fopen("C:\\Temp\\access_log\\access_log", "r + ")) == NULL) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;  // Return an error code
    }

    char line[1001];
    std::unordered_map<std::string, int> fileCount;

    // Record the end time
    auto end = std::chrono::system_clock::now();
    std::time_t endTime = std::chrono::system_clock::to_time_t(end);

    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    //std::cout << "1Elapsed Time: " << elapsedTime << " milliseconds" << std::endl;

    while (fgets(line, 1000, fp)) {
        std::string fileName;
        if (extractSubstring(line, fileName)) {
            // Increment the count for the file
            fileCount[fileName]++;
        }
    }
    fclose(fp);


    end = std::chrono::system_clock::now();
    endTime = std::chrono::system_clock::to_time_t(end);

    elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    //std::cout << "2Elapsed Time: " << elapsedTime << " milliseconds" << std::endl;


    char buffer[1000];

    Pair* sortedPairs = new Pair[fileCount.size()];
    int i = 0;

    for (const auto& pair : fileCount) {
        sortedPairs[i].key = pair.first;
        sortedPairs[i].value = pair.second;
        i++;
    }

    //SORT ile de denedik hemen hemen aynı süreler ölçüldü.
    // Sort the pairs alphabetically
    qsort(sortedPairs, fileCount.size(), sizeof(Pair), comparePairs);

    end = std::chrono::system_clock::now();
    endTime = std::chrono::system_clock::to_time_t(end);

    elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    //std::cout << "3Elapsed Time: " << elapsedTime << " milliseconds" << std::endl;

    if ((fp = fopen("C:\\Temp\\access_log\\access_log_ordered3", "w+t")) == NULL) {
        std::cerr << "Error opening file!" << std::endl;
        delete[] sortedPairs;  // Release memory before returning an error
        return 1;  // Return an error code
    }

    // Print the sorted nodes
    for (int j = 0; j < fileCount.size() && j < 10; j++) {
        
        sprintf(buffer, "%s%s%s%d%s", "(", sortedPairs[j].key.c_str(), "), (", sortedPairs[j].value, " visits) \n");
        fwrite(buffer, sizeof(char), strlen(buffer), fp);
        printf("%s", buffer);
    }

    fclose(fp);

    end = std::chrono::system_clock::now();
    endTime = std::chrono::system_clock::to_time_t(end);

    elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Top 10 List -  GetTickCoun " << elapsedTime << " ms" << std::endl;


    delete[] sortedPairs;  // Release memory


    return 0;
}
