#include <windows.h>
#include <string>
#include <fstream>
#include <iostream>

// Definition of the WinRegistry class to manage Windows Registry Operations
class WinRegistry {
private:
    std::string name;                       // Registry name (key)
    std::string type;                       // Registry type (e.g., REG_SZ, REG_BINARY).
    std::string registryPath;               // The path of the registry key to be managed 
public:
    // Constructor to initialize the class with name, type, and registry path
    WinRegistry(std::string name, std::string type, std::string registryPath) {
        this->name = name;                  // Assigns the input 'name' to the class member 'name'
        this->type = type;                  // Assigns the input 'type' to the class member 'type'
        this->registryPath = registryPath;  // Assigns the input 'registryPath' to the class member 'registryPath'.
    }

    // Getters for each class member (name, type, registry path)
    std::string getName() {
        return this->name;                  // Return the registry name.
    }

    std::string getType() {
        return this->type;                  // Return the registry type.
    }

    std::string getRegistryPath() {
        return this->registryPath;          // Return the registry path.
    }

    // Setters for each class member (name, type, registry path)
    void setName(std::string name) {
        this->name = name;                  // Sets the registry name.
    }

    void setType(std::string type) {            
        this->type = type;                  // Sets the registry type.
    }

    void setRegistryPath(std::string registryPath) {
        this->registryPath = registryPath;  // Sets the registry path.
    }

    // Method to create a registry key at the specified registry path
    void createRegistry() {
        HKEY hKey;                          // Handle to the registry key
        // RegCreateKeyExA creates or opens a registry key. The result is stored in 'createStatus'
        LONG createStatus = RegCreateKeyExA(HKEY_CURRENT_USER, this->registryPath.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
        if (createStatus == ERROR_SUCCESS) {
            std::cout << "Registry created successfully" << std::endl;      // Success message if registry key creation succeeds.
        } else {
            std::cout << "Error creating registry" << std::endl;            // Error message if registry key creation fails
        }
    }

    // Method to delete a registry key
    void deleteRegistry() {
        LONG deleteStatus = RegDeleteKeyA(HKEY_CURRENT_USER, this->registryPath.c_str()); // Deleting a registry key
        if (deleteStatus == ERROR_SUCCESS) {
            std::cout << "Registry deleted successfully" << std::endl;      // Success message if key deletion succeeds.
        } else {
            std::cout << "Error deleting registry" << std::endl;            // Error message if key deletion fails
        }
    }

    // Method to set a string value in the registry
    bool setRegistryValue(const std::string& valueName, const std::string& valueData) {
        HKEY hKey;                                                          // Handle to the registry key.
        // Opens the registry key for setting values
        LONG openStatus = RegOpenKeyExA(HKEY_CURRENT_USER, registryPath.c_str(), 0, KEY_SET_VALUE, &hKey);
        if (openStatus == ERROR_SUCCESS) {
            // Sets the string value for the key
            LONG setValueStatus = RegSetValueExA(hKey, valueName.c_str(), 0, REG_SZ, 
                                                reinterpret_cast<const BYTE*>(valueData.c_str()),
                                                static_cast<DWORD>((valueData.length() + 1) * sizeof(char)));
            RegCloseKey(hKey);          // Closes the registry key after the operation
            if (setValueStatus == ERROR_SUCCESS) {
                std::cout << "Registry value set successfully" << std::endl; // Success message for setting the value
                return true;
            } else {
                std::cerr << "Error setting registry value" << std::endl;    // Error message if value setting fails
                return false;
            }
        } else {
            std::cerr << "Error opening registry key" << std::endl;          // Error message if key opening fails.
            return false;
        }
    }

    // Method to get a string value from the registry
    bool getRegistryValue(const std::string& valueName, std::string& buffer, DWORD bufferSize) {
        HKEY hKey;                       // Handle to registry key
        // Opens the registry key for querying values
        LONG openStatus = RegOpenKeyExA(HKEY_CURRENT_USER, registryPath.c_str(), 0, KEY_QUERY_VALUE, &hKey);
        if (openStatus == ERROR_SUCCESS) {
            DWORD valueType;               // To store the type of the registry value
            buffer.resize(bufferSize);     // Resizes the buffer to hold the value data
            // Queries the registry value
            LONG queryValueStatus = RegQueryValueExA(hKey, valueName.c_str(), NULL, &valueType, reinterpret_cast<BYTE*>(&buffer[0]), &bufferSize);
            RegCloseKey(hKey);              // Closes the registry key after querying
            if (queryValueStatus == ERROR_SUCCESS && valueType == REG_SZ) {
                std::cout << "Registry value retrieved successfully: " << buffer << std::endl; // Success message
                return true;
            } else {
                std::cerr << "Error retrieving registry value" << std::endl;                   // Error message
                return false;
            }
        } else {
            std::cerr << "Error opening registry key" << std::endl;                            // Error message if key opening fails
            return false;
        }
    }

    // Method to set a binary value in the registry
    bool setRegistryValueAsBinary(const std::string& valueName, std::string& valueData) {
        HKEY hKey;                          // Handle to the registry key
        // Opens the registry key for querying values
        LONG openStatus = RegOpenKeyExA(HKEY_CURRENT_USER, registryPath.c_str(), 0, KEY_SET_VALUE, &hKey);
        if (openStatus == ERROR_SUCCESS) {
            // Sets the binary value for the key
            LONG setValueStatus = RegSetValueExA(hKey, valueName.c_str(), 0, REG_BINARY, 
                                                reinterpret_cast<const BYTE*>(valueData.c_str()), 
                                                static_cast<DWORD>(valueData.length()));
            RegCloseKey(hKey);              // Closes the registry key after the operation
            if (setValueStatus == ERROR_SUCCESS) {
                std::cout << "Registry value set successfully" << std::endl;  // Success message
                return true;
            } else {
                std::cerr << "Error setting registry value" << std::endl;     // Error message if setting fails
                return false;
            }
        } else {
            std::cerr << "Error opening registry key" << std::endl;           // Error message if key opening fails
            return false;
        }
    }

    // Method to get a binary value from the registry
    bool getRegistryValueAsBinary(std::string& valueName, std::string& buffer, DWORD bufferSize) {
        HKEY hKey;                          // Handle to the registry key
        // Opens the registry key for querying values
        LONG openStatus = RegOpenKeyExA(HKEY_CURRENT_USER, registryPath.c_str(), 0, KEY_QUERY_VALUE, &hKey);
        if (openStatus == ERROR_SUCCESS) {
            DWORD valueType;
            buffer.resize(bufferSize);      // Resizes the buffer to hold the value data
            // Queries the binary value
            LONG queryValueStatus = RegQueryValueExA(hKey, valueName.c_str(), NULL, &valueType, reinterpret_cast<BYTE*>(&buffer[0]), &bufferSize);
            RegCloseKey(hKey);              // Closes the registry key after querying
            if (queryValueStatus == ERROR_SUCCESS && valueType == REG_BINARY) {
                std::cout << "Registry value retrieved successfully: " << buffer << std::endl; // Success message
                return true;
            } else {
                std::cerr << "Error retrieving registry value" << std::endl;                   // Error message
                return false;
            }
        } else {
            std::cerr << "Error opening registry key" << std::endl;                            // Error message if key opening fails
            return false;
        }
    }

    // Method to set a file as binary data in the registry
    bool setFileAsBinary(std::string srcFile) {
        HANDLE hFile = CreateFileA(srcFile.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            std::cerr << "Error opening file: " << GetLastError() << std::endl;
            return false;
        }
        HKEY hKey;
        // Opens the registry key for setting values
        LONG openStatus = RegOpenKeyExA(HKEY_CURRENT_USER, registryPath.c_str(), 0, KEY_SET_VALUE, &hKey);
        if (openStatus != ERROR_SUCCESS) {
            std::cerr << "Error opening registry key: " << openStatus << std::endl;
            CloseHandle(hFile);
            return false;
        }

        DWORD fileSize = GetFileSize(hFile, NULL);
        if (fileSize == INVALID_FILE_SIZE) {
            std::cerr << "Error getting file size: " << GetLastError() << std::endl;
            CloseHandle(hFile);
            RegCloseKey(hKey);
            return false;
        }

        std::string fileData(fileSize, '\0');
        DWORD bytesRead;
        if (!ReadFile(hFile, &fileData[0], fileSize, &bytesRead, NULL)) {
            std::cerr << "Error reading file: " << GetLastError() << std::endl;
            CloseHandle(hFile);
            RegCloseKey(hKey);
            return false;
        }
        CloseHandle(hFile);

        // Setting the binary data in the registry
        LONG setValueStatus = RegSetValueExA(hKey, srcFile.c_str(), 0, REG_BINARY, reinterpret_cast<const BYTE*>(fileData.c_str()), fileSize);
        RegCloseKey(hKey);
        if (setValueStatus == ERROR_SUCCESS) {
            std::cout << "File data set in registry successfully" << std::endl;
            return true;
        } else {
            std::cerr << "Error setting file data in registry: " << setValueStatus << std::endl;
            return false;
        }
    }

    // Method to retrieve and save binary data as a file
    bool getRegistryBinaryAsFile(std::string destFile) {
        HKEY hKey;
        // Opens the registry key for querying values
        LONG openStatus = RegOpenKeyExA(HKEY_CURRENT_USER, registryPath.c_str(), 0, KEY_QUERY_VALUE, &hKey);
        if (openStatus != ERROR_SUCCESS) {
            std::cerr << "Error opening registry key: " << openStatus << std::endl;
            return false;
        }

        DWORD bufferSize;
        LONG queryValueStatus = RegQueryValueExA(hKey, destFile.c_str(), NULL, NULL, NULL, &bufferSize);
        if (queryValueStatus != ERROR_SUCCESS) {
            std::cerr << "Error querying value size: " << queryValueStatus << std::endl;
            RegCloseKey(hKey);
            return false;
        }

        std::string buffer(bufferSize, '\0');
        DWORD valueType;
        queryValueStatus = RegQueryValueExA(hKey, destFile.c_str(), NULL, &valueType, reinterpret_cast<BYTE*>(&buffer[0]), &bufferSize);
        RegCloseKey(hKey);
        if (queryValueStatus != ERROR_SUCCESS || valueType != REG_BINARY) {
            std::cerr << "Error querying value or incorrect type: " << queryValueStatus << std::endl;
            return false;
        }

        HANDLE hFile = CreateFileA(destFile.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            std::cerr << "Error creating file: " << GetLastError() << std::endl;
            return false;
        }

        DWORD bytesWritten;
        if (!WriteFile(hFile, buffer.data(), bufferSize, &bytesWritten, NULL)) {
            std::cerr << "Error writing to file: " << GetLastError() << std::endl;
            CloseHandle(hFile);
            return false;
        }

        CloseHandle(hFile);
        std::cout << "Registry binary data saved to file successfully" << std::endl;
        return true;
    }
};

int main() {
    // Create a WinRegistry object with example values.
    WinRegistry myRegistry("MyApp", "REG_BINARY", "Software\\MyApp\\Settings");
    myRegistry.createRegistry(); // Create a registry key.

    // Set a string value in the registry.
    std::string valueName = "Username";
    std::string valueData = "User1";
    myRegistry.setRegistryValue(valueName, valueData);

    // Get a string value from the registry.
    std::string retrievedData;
    DWORD bufferSize = 256;
    myRegistry.getRegistryValue(valueName, retrievedData, bufferSize);

    // Set a binary value in the registry from file.
    myRegistry.setFileAsBinary("C:\\path_to_binary_file");

    // Retrieve binary data from the registry and save it to a file.
    myRegistry.getRegistryBinaryAsFile("C:\\path_to_save_binary");

    // Delete the registry key after use.
    myRegistry.deleteRegistry();

    return 0;
}