#include <iostream>
#include <deque>
#include <windows.h>
#include "MapMemoryObjects.h"

int main() {
    // Create an instance of MapMemoryObjects
    MapMemoryObjects mapper = MapMemoryObjects();
    // Map memory to get system handles
    PSYSTEM_HANDLE_INFORMATION mappedObjects = mapper.MapMemoryHandlers();

    if (!mappedObjects) {
        std::cout << "Failed to map memory handlers." << std::endl;
        return -1;
    }

    // Filter registry key handles
    std::deque<HANDLE> handles = mapper.FilterRegisterKeys(mappedObjects);
    // Look for a specific registry key handle by name
    HANDLE hkey = mapper.FindRegistryKeyHandle(mappedObjects, L"DisableBehaviorMonitoring");
    
    if(hkey != NULL) {
        std::cout << "Registry key 'DisableBehaviorMonitoring' found." << std::endl;
    } else {
        std::cout << "Registry key 'DisableBehaviorMonitoring' not found." << std::endl;
    }

    return 0;
}