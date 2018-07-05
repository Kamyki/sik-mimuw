#include "set.h"
#include "err.h"
#include <cstring>
#include <cstdlib>

void add_to_set(std::set<uint64_t>& collection, char* commaDelimited) {
    char* pt;
    pt = strtok(commaDelimited, ",");
    while (pt != nullptr) {
        int64_t num = strtol(pt, nullptr, 10);
        if(num >= 0) {
            collection.insert(static_cast<unsigned long long int&&>(num));
        }
        pt = strtok(nullptr, ",");
    }
}
