#include <iostream>
#include <string>
#include <shlwapi.h>                // This is Windows shell Lightweight Utility API, which provides string manipulation, file path functions, and other utility functions
#include <psapi.h>                  // Provides APIs for querying system and process information, such as enumerating process handles, memory usage
#include <stdio.h>
#include <deque>                    // Provides the double-ended queue container, std::deque is a sequence container that allows fast insertion and deletion at both the beginning and the end
#include "ResolveObject.h"
#include <cstdint>

/* 
    The security solutions like Antivirus or EDR are always searching for new process creation,
    file manipulation, creation of remote threads, registry values or key modification. And
    with this, we don't need to do this considered malicious activities never
*/

// This macro is used to check if a function result(x) is a success.
#define NT_SUCCESS(x) ((x) >= 0)
// This is a constant that represent an NTSTATUS code which indicates the provided buffer
// was too small to hold all the data returned by a function
#define STATUS_INFO_LENGTH_MISMATCH 0xc0000004
// This defines a constant with value 16 which is used as an identifier to query information 
// about system handles using the NtQuerySystemInformation function
#define SystemHandlerInformation 16
// These constants define object information classes.
// These are typically used with NtQueryObject to get specific information about objects in Windows
#define ObjectBasicInformation 0
#define ObjectNameInformation 1
#define ObjectTypeInformation 2

// Define class MapMemoryObjects() for handling and mapping system memory objects
class MapMemoryObjects {
public:
    struct MemoryHandlerStruct {
        char HandleType[256];       // an Array of characters that can store a string of up to 255 characters
        uintptr_t HandleAddress;    // unsigned integer type capable of storing a pointer ---> Hole the address of the memory object represented by the handle 
        uintptr_t HandleValue;      // unsigned integer type ---> identify the handle within the operating system
        char HandleAccess[256];     // an Array describes the access rights associated with the handle, (e.g. "read", "write", "execute")
        char ProcessName[256];      // an Array of characters that hold the name of the process associated with the handle
        int ProcessID;              // an integer stores the unique identifier (ID)
    };

    MapMemoryObjects() {};

    // Function for retrieving and mapping system handle information
    PSYSTEM_HANDLE_INFORMATION MapMemoryHandlers() {

        // NtQuerySystemInformation_t is a function pointer type
        // Windows API: NtQuerySystemInformation_t 
        NtQuerySystemInformation_t pNtQuerySystemInformaiton = NULL;
        // Data type used by Windows to indicate the result of NTAPI function calls
        NTSTATUS status;
        // handleInfoSize is an unsigned long integer initialized to 0x100000 (65536 in decimal)
        // which represent the initial size of the buffer allocated to hold handle information
        ULONG handleInfoSize = 0x10000;
        // handleInfo: is a pointer to a structure that holds information about system handles.
        PSYSTEM_HANDLE_INFORMATION handleInfo;
    
        // Resolve NtQuerySystemInformation, NtDuplicateObject, NtQueryObject
        // The function is crucial for retrieving system handle information
        pNtQuerySystemInformaiton = (NtQuerySystemInformation_t)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtQuerySystemInformation");
        // Allocate memory for handle information
        handleInfo = (PSYSTEM_HANDLE_INFORMATION)malloc(handleInfoSize);

        while (status = pNtQuerySystemInformaiton(SystemHandlerInformation, handleInfo, handleInfoSize, NULL) == STATUS_INFO_LENGTH_MISMATCH) {
            handleInfoSize *= 2;        // double the size of handleInfoSize to accommodate more data if the previous allocation was too small
            handleInfo = (PSYSTEM_HANDLE_INFORMATION)realloc(handleInfo, handleInfoSize);
            // realloc(handleInfo, handleInfoSize): resizes handleInfo to the new size
        }
        return handleInfo;
    }

    // Function to filter file handles from system handle information
    std::deque<HANDLE> FilterFile(PSYSTEM_HANDLE_INFORMATION memoryHandlers) {
        // Create a deque (double-ended queue) to store file handles
        std::deque<HANDLE> processesHandlers;

        // Declare pointers to NtDuplicateObject and NtQueryObject functions
        NtDuplicateObject_t pNtDuplicateObject = NULL;
        NtQueryObject_t pNtQueryObject = NULL;

        // Load the NtDuplicateObject function from ntdll.dll
        pNtDuplicateObject = (NtDuplicateObject_t)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtDuplicateObject");

        // Load the NtQueryObject function from ntdll.dll
        pNtQueryObject = (NtQueryObject_t)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtQueryObject");

        // Variable to interate through handles
        ULONG i;

        // Declare pointers to hold object type information
        POBJECT_TYPE_INFORMATION objectTypeInfo;

        // Declare variable to hold NSTATUS return values
        NTSTATUS status;

        // Handle to the process owning the current handle
        HANDLE hProcess;

        // Pointer to hold information about the object name
        PVOID objectNameInfo;

        // Variable to store the size of the returned data
        ULONG returnLength;

        // Unicode string to store the object's name
        UNICODE_STRING objectName;

        // Variable to hold the duplicated handle
        HANDLE duplicatedHandle = NULL;

        // The table headers to the console output
        std::cout << "Process Name" << "\t" << "Process ID" << "\t" << "Handle Value" << "\t" << "Granted Access" << "\t" << "Handle Type" << "\t" << std::endl;
        std::cout << "------------" << "\t" << "----------" << "\t" << "------------" << "\t" << "-------------" << "\t" << "-----------" << "\t" << std::endl;

        // Loop through all the handles in the memoryHandlers structure
        for(i = 0; i < memoryHandlers->NumberOfHandles; i++) {
            // Access the current handle in the list
            SYSTEM_HANDLE_TABLE_ENTRY_INFO handle = memoryHandlers->Handles[i];

            // Attempt to open the process that owns the handle for duplication
            if(!(hProcess = OpenProcess(PROCESS_DUP_HANDLE | PROCESS_QUERY_INFORMATION, FALSE, handle.UniqueProcessId))) {
                // Access the current handle in the list
                continue;
            }

            // Duplicate the current handle into the current process
            HANDLE duplicatedHandle = NULL;
            NTSTATUS status = pNtDuplicateObject(hProcess, (HANDLE)(uintptr_t)handle.HandleValue, GetCurrentProcess(), &duplicatedHandle, 0, 0, DUPLICATE_SAME_ACCESS);

            if(!NT_SUCCESS(status)){
                CloseHandle(hProcess);
                continue;
            }

            // Allocate memory for the object type information
            objectTypeInfo = (POBJECT_TYPE_INFORMATION)malloc(0x2000);

            // Query the duplicated handle to get its type information
            pNtQueryObject(duplicatedHandle, ObjectTypeInformation, objectTypeInfo, 0x1000, &returnLength);

            // Compare the object type with "File" to filter file handles
            if (wcscmp(objectTypeInfo->Name.Buffer, L"File") == 0) {
                // If the handle is a file handle, add it to the deque of process handles
                processesHandlers.push_back(duplicatedHandle);

                // Print the handle information
                printf("[HP:%#25s : %#5d] [%#7x]  (0x%p) %#10x %.*S\n",
                    "process name",             // Placeholder for process name
                    GetProcessId(hProcess),     // Process ID owning the handle
                    handle.HandleValue,         // Value of the handle
                    handle.Object,              // Pointer to the handle's object
                    handle.GrantedAccess,       // Access rights granted to the handle
                    objectTypeInfo->Name.Length / 2,    // Length of the object type name
                    objectTypeInfo->Name.Buffer // Object type name buffer    
                    );
            }
        }

        return processesHandlers;   
    }

    // Function to return a deque of handles to registry keys
    std::deque<HANDLE> FilterRegisterKeys(PSYSTEM_HANDLE_INFORMATION memoryHandlers) {
        std::deque<HANDLE> processesHandlers;           // Filtered registry key handles
        NtDuplicateObject_t pNtDuplicateObject = NULL;  // Function that duplicates an object handle
        NtQueryObject_t pNtQueryObject = NULL;          // Function that queries information about an object including its type
        // Function pointers are loaded from the ntdll library using GetProcAddress
        pNtDuplicateObject = (NtDuplicateObject_t)GetProcAddress(GetModuleHandleW(L"ntdll.ll"), "NtDuplicateObject");
        pNtQueryObject = (NtQueryObject_t)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtQueryObject");
        ULONG i;                                        // Variables for iterating through loop handles
        POBJECT_TYPE_INFORMATION objectTypeInfo;        // Hold information about the object type
        NTSTATUS status;                                // Hold the status of NT function calls
        HANDLE hProcess;                                // Handle to the process that owns the current handle
        PVOID objectNameInfo;
        ULONG returnLength;
        UNICODE_STRING objectName;
        HANDLE duplicatedHandle = NULL;                 // The duplicated handle to be examined
        std::cout << "Process Name" << "\t" << "Process ID" << "\t" << "Handle Value" << "\t" << "Granted Access" << "\t" << "Handle Type" << "\t" << std::endl;
        std::cout << "------------" << "\t" << "----------" << "\t" << "------------" << "\t" << "-------------" << "\t" << "-----------" << "\t" << std::endl;

        // Loop iterates through all the system handles available
        for(i = 0; i < memoryHandlers->NumberOfHandles; i++) {
            // Each "handle" is represented by SYSTEM_HANDLE_TABLE_ENTRY_INFO 
            SYSTEM_HANDLE_TABLE_ENTRY_INFO handle = memoryHandlers->Handles[i];
            // Open Process with the desired access rights of duplicating the handle and querying information
            if (!(hProcess = OpenProcess(PROCESS_DUP_HANDLE | PROCESS_QUERY_INFORMATION, FALSE, handle.UniqueProcessId))) {
                continue;        
            }

            // Duplicates the current handle into the calling process to examine it
            HANDLE duplicatedHandle = NULL;
            NTSTATUS status = pNtDuplicateObject(hProcess, (HANDLE)(uintptr_t)handle.HandleValue, GetCurrentProcess(), &duplicatedHandle, 0, 0, DUPLICATE_SAME_ACCESS);
            if (!NT_SUCCESS(status)) {
                CloseHandle(hProcess);
                continue;
            }

            // Allocate memory for the object type information
            objectTypeInfo = (POBJECT_TYPE_INFORMATION)malloc(0x2000);
            // Query the duplicated handle to get its type information
            pNtQueryObject(duplicatedHandle, ObjectTypeInformation, objectTypeInfo, 0x1000, &returnLength);

            // Compare the object type with "Key" to filter file handles
            if (wcscmp(objectTypeInfo->Name.Buffer, L"Key") == 0) {
                processesHandlers.push_back(duplicatedHandle);      // If the handle is a file handle, add it to the deque of process handles

                // Print the handle information
                printf("[HP:%#25s : %#5d] [%#7x]  (0x%p) %#10x %.*S\n",
                    "process name",                         // Placeholder for process name
                    GetProcessId(hProcess),                 // Process ID owning the handle
                    handle.HandleValue,                     // Value of the handle
                    handle.Object,                          // Pointer to the handle's object
                    handle.GrantedAccess,                   // Access rights granted to the handle
                    objectTypeInfo->Name.Length / 2,        // Length of the object type name
                    objectTypeInfo->Name.Buffer             // Object type name buffer
                    );
            }
        }
        return processesHandlers;
    }

    // Function to return a deque of handles to filter process
    std::deque<HANDLE> FilterProcess(PSYSTEM_HANDLE_INFORMATION memoryHandlers) {
        std::deque<HANDLE> processHandlers;                 // Deque to store handles related to process objects
        NtDuplicateObject_t pNtDuplicateObject = NULL;      // Define function pointers for NtDuplicateObject and NtQueryObject
        NtQueryObject_t pNtQueryObject = NULL;

        // Load NtDuplicateObject and NtQueryObject from ntdll.dll
        pNtDuplicateObject = (NtDuplicateObject_t)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtDuplicateObject");
        pNtQueryObject = (NtQueryObject_t)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtQueryObject");

        // Variable declarations
        ULONG i;                                    // Loop counter
        POBJECT_TYPE_INFORMATION objectTypeInfo;    // Holds object type info
        NTSTATUS status;                            // Stores the status of NT calls
        HANDLE hProcess;                            // Process handle
        PVOID objectNameInfo;                       // Pointer to store object name info
        ULONG returnLength;                         // Stores the length of the returned data
        UNICODE_STRING objectName;                  // Used to store the object's name
        HANDLE duplicatedHandle = NULL;             // Handle to the duplicated object

        // Print table headers for ouput information about processes
        std::cout << "Process Name" << "\t" << "Process ID" << "\t" << "Handle Value" << "\t" << "Granted Access" << "\t" << "Handle Type" << "\t" << std::endl;
        std::cout << "------------" << "\t" << "----------" << "\t" << "------------" << "\t" << "-------------" << "\t" << "-----------" << "\t" << std::endl;

        // Iterate over all handles present in the system
        for (i = 0; i < memoryHandlers->NumberOfHandles; i++) {
            // Extract each handle from the system handle table
            SYSTEM_HANDLE_TABLE_ENTRY_INFO handle = memoryHandlers->Handles[i];

            // Try to open the process that owns the current handle
            if (!(hProcess = OpenProcess(PROCESS_DUP_HANDLE| PROCESS_QUERY_INFORMATION, FALSE, handle.UniqueProcessId))){
                continue;
            }

            // Duplicate the handle into the current process using NtDuplicateObject
            HANDLE duplicatedHandle = NULL;
            NTSTATUS status = pNtDuplicateObject(hProcess, (HANDLE)(uintptr_t)handle.HandleValue, GetCurrentProcess(), &duplicatedHandle, 0, 0, DUPLICATE_SAME_ACCESS);
            if (!NT_SUCCESS(status)) {
                // If the duplication failed, close the process handle
                CloseHandle(hProcess);
                continue;
            }

            // Allocate memory to store object type information
            objectTypeInfo = (POBJECT_TYPE_INFORMATION)malloc(0x2000);

            // Query the duplicated handle's object type using NtQueryObject
            pNtQueryObject(duplicatedHandle, ObjectTypeInformation, objectTypeInfo, 0x1000, &returnLength);

            // Check if the object type is "Process"
            if (wcscmp(objectTypeInfo->Name.Buffer, L"Process") == 0) {
                // If the object is of type "Process"
                processHandlers.push_back(duplicatedHandle);

                // Print details about the handle
                printf("[HP:%#25s : %#5d] [%#7x]  (0x%p) %#10x %.*S\n",
                    "process name",                   // Placeholder for process name
                    GetProcessId(hProcess),           // Get the process ID
                    handle.HandleValue,               // Print the handle value
                    handle.Object,                    // The handle's object pointer
                    handle.GrantedAccess,             // The access rights granted to this handle
                    objectTypeInfo->Name.Length / 2,   // Length of the object name
                    objectTypeInfo->Name.Buffer         // Object name
                    );
            }
        }
        return processHandlers;
    }
    // Function to return a deque of handles to filter tokens
    std::deque<HANDLE> FilterTokens(PSYSTEM_HANDLE_INFORMATION memoryHandlers) {
        // Deque to store handles related to token objects
        std::deque<HANDLE> processesHandlers;
    
        // Define function pointers for NtDuplicateObject and NtQueryObject
        NtDuplicateObject_t pNtDuplicateObject = NULL;
        NtQueryObject_t pNtQueryObject = NULL;
    
        // Load NtDuplicateObject and NtQueryObject from ntdll.dll
        pNtDuplicateObject = (NtDuplicateObject_t)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtDuplicateObject");
        pNtQueryObject = (NtQueryObject_t)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtQueryObject");

        // Variable declarations
        ULONG i;                                    // Loop counter
        POBJECT_TYPE_INFORMATION objectTypeInfo;    // Holds object type info
        NTSTATUS status;                            // Stores the status of NT calls
        HANDLE hProcess;                            // Process handle
        PVOID objectNameInfo;                       // Pointer to store object name info
        ULONG returnLength;                         // Stores the length of the returned data
        UNICODE_STRING objectName;                  // Used to store the object's name
        HANDLE duplicatedHandle = NULL;             // Handle to the duplicated object
    
        // Print table headers for output information about tokens
        std::cout << "Process Name" << "\t" << "Process ID" << "\t" << "Handle Value" << "\t" << "Granted Access" << "\t" << "Handle Type" << "\t" << std::endl;
        std::cout << "------------" << "\t" << "----------" << "\t" << "------------" << "\t" << "-------------" << "\t" << "-----------" << "\t" << std::endl;

        // Iterate over all handles present in the system
        for (i = 0; i < memoryHandlers->NumberOfHandles; i++) {
            // Extract each handle from the system handle table
            SYSTEM_HANDLE_TABLE_ENTRY_INFO handle = memoryHandlers->Handles[i];

            // Try to open the process that owns the current handle
            if (!(hProcess = OpenProcess(PROCESS_DUP_HANDLE | PROCESS_QUERY_INFORMATION, FALSE, handle.UniqueProcessId))) {
                continue;  // If the process couldn't be opened, skip to the next handle
            }

            // Duplicate the handle into the current process using NtDuplicateObject
            HANDLE duplicatedHandle = NULL;
            NTSTATUS status = pNtDuplicateObject(hProcess, (HANDLE)(uintptr_t)handle.HandleValue, GetCurrentProcess(), &duplicatedHandle, 0, 0, DUPLICATE_SAME_ACCESS);
            if (!NT_SUCCESS(status)) {
                // If the duplication failed, close the process handle and skip to the next iteration
                CloseHandle(hProcess);
                continue;
            }

            // Allocate memory to store object type information
            objectTypeInfo = (POBJECT_TYPE_INFORMATION)malloc(0x2000);
        
            // Query the duplicated handle's object type using NtQueryObject
            pNtQueryObject(duplicatedHandle, ObjectTypeInformation, objectTypeInfo, 0x1000, &returnLength);

            // Check if the object type is "Token"
            if (wcscmp(objectTypeInfo->Name.Buffer, L"Token") == 0) {
                // If the object is of type "Token", store the handle in the deque
                processesHandlers.push_back(duplicatedHandle);

                // Print details about the handle
                printf("[HP:%#25s : %#5d] [%#7x]  (0x%p) %#10x %.*S\n",
                    "process name",  // Placeholder for process name (not implemented)
                    GetProcessId(hProcess),  // Get the process ID
                    handle.HandleValue,  // Print the handle value
                    handle.Object,  // The handle's object pointer
                    handle.GrantedAccess,  // The access rights granted to this handle
                    objectTypeInfo->Name.Length / 2,  // Length of the object name
                    objectTypeInfo->Name.Buffer);  // Object name (in this case, "Token")
            }
        } 
        return processesHandlers;
    }
    // Function to return a deque of handles to filter threads
    std::deque<HANDLE> FilterThreads(PSYSTEM_HANDLE_INFORMATION memoryHandlers) {
        // Deque to store handles related to thread objects
        std::deque<HANDLE> processesHandlers;

        // Define function pointers for NtDuplicateObject and NtQueryObject
        NtDuplicateObject_t pNtDuplicateObject = NULL;
        NtQueryObject_t pNtQueryObject = NULL;

        // Load NtDuplicateObject and NtQueryObject from ntdll.dll
        pNtDuplicateObject = (NtDuplicateObject_t)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtDuplicateObject");
        pNtQueryObject = (NtQueryObject_t)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtQueryObject");

        // Variable declarations
        ULONG i;                                    // Loop counter
        POBJECT_TYPE_INFORMATION objectTypeInfo;    // Holds object type information
        NTSTATUS status;                            // Stores the status of NT calls
        HANDLE hProcess;                            // Process handle
        PVOID objectNameInfo;                       // Pointer to store object name info (unused here)
        ULONG returnLength;                         // Stores the length of the returned data
        UNICODE_STRING objectName;                  // Used to store the object's name
        HANDLE duplicatedHandle = NULL;             // Handle to the duplicated object

        // Print table headers for output information about thread handles
        std::cout << "Process Name" << "\t" << "Process ID" << "\t" << "Handle Value" << "\t" << "Granted Access" << "\t" << "Handle Type" << "\t" << std::endl;
        std::cout << "------------" << "\t" << "----------" << "\t" << "------------" << "\t" << "-------------" << "\t" << "-----------" << "\t" << std::endl;

        // Iterate over all handles present in the system
        for (i = 0; i < memoryHandlers->NumberOfHandles; i++) {
            // Extract each handle from the system handle table
            SYSTEM_HANDLE_TABLE_ENTRY_INFO handle = memoryHandlers->Handles[i];

            // Try to open the process that owns the current handle
            if (!(hProcess = OpenProcess(PROCESS_DUP_HANDLE | PROCESS_QUERY_INFORMATION, FALSE, handle.UniqueProcessId))) {
                continue;  // If the process couldn't be opened, skip to the next handle
            }

            // Duplicate the handle into the current process using NtDuplicateObject
            HANDLE duplicatedHandle = NULL;
            NTSTATUS status = pNtDuplicateObject(hProcess, (HANDLE)(uintptr_t)handle.HandleValue, GetCurrentProcess(), &duplicatedHandle, 0, 0, DUPLICATE_SAME_ACCESS);
            if (!NT_SUCCESS(status)) {
                // If the duplication failed, close the process handle and skip to the next iteration
                CloseHandle(hProcess);
                continue;
            }

            // Allocate memory to store object type information
            objectTypeInfo = (POBJECT_TYPE_INFORMATION)malloc(0x2000);

            // Query the duplicated handle's object type using NtQueryObject
            pNtQueryObject(duplicatedHandle, ObjectTypeInformation, objectTypeInfo, 0x1000, &returnLength);

            // Check if the object type is "Thread"
            if (wcscmp(objectTypeInfo->Name.Buffer, L"Thread") == 0) {
                // If the object is of type "Thread", store the handle in the deque
                processesHandlers.push_back(duplicatedHandle);

                // Print details about the handle
                printf("[HP:%#25s : %#5d] [%#7x]  (0x%p) %#10x %.*S\n",
                    "process name",  // Placeholder for process name (not implemented)
                    GetProcessId(hProcess),  // Get the process ID
                    handle.HandleValue,  // Print the handle value
                    handle.Object,  // The handle's object pointer
                    handle.GrantedAccess,  // The access rights granted to this handle
                    objectTypeInfo->Name.Length / 2,  // Length of the object name
                    objectTypeInfo->Name.Buffer);  // Object name (in this case, "Thread")
            }
        }
        return processesHandlers;
    }

    HANDLE FindRegistryKeyHandle(PSYSTEM_HANDLE_INFORMATION memoryHandlers, const std::wstring& registryName) {
        // Get function pointers to NtDuplicateObject and NtQueryObject from ntdll.dll
        NtDuplicateObject_t pNtDuplicateObject = (NtDuplicateObject_t)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtDuplicateObject");
        NtQueryObject_t pNtQueryObject = (NtQueryObject_t)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtQueryObject");

        // Declare variables
        POBJECT_TYPE_INFORMATION objectTypeInfo;            // Stores the object type information
        NTSTATUS status;                                    //NTSTATUS to check success or failure of NT API calls
        HANDLE hProcess;                                    // Handle to the process owning the handle being examined
        HANDLE duplicatedHandle = NULL;                     // Handle to store the duplicated handle

        // Iterate over all the system handles in memoryHandlers
        for (ULONG i = 0; i < memoryHandlers->NumberOfHandles; i++) {
            SYSTEM_HANDLE_TABLE_ENTRY_INFO handle = memoryHandlers->Handles[i];

            // Try to open the process that owns the current handle
            if (!(hProcess = OpenProcess(PROCESS_DUP_HANDLE | PROCESS_QUERY_INFORMATION, FALSE, handle.UniqueProcessId))) {
                continue;
            }

            // Duplicate the handle into the current process
            HANDLE duplicatedHandle = NULL;
            status = pNtDuplicateObject(hProcess, (HANDLE)(uintptr_t)handle.HandleValue, GetCurrentProcess(), &duplicatedHandle, 0, 0, DUPLICATE_SAME_ACCESS);
            if (!NT_SUCCESS(status)) {
                CloseHandle(hProcess);
                continue;
            }

            // Allocate memory to store object type information
            objectTypeInfo = (POBJECT_TYPE_INFORMATION)malloc(0x2000);
            ULONG returnLength;

            // Query the type of the duplicate handle
            status = pNtQueryObject(duplicatedHandle, ObjectTypeInformation, objectTypeInfo, 0x1000, &returnLength);
            if (!NT_SUCCESS(status)) {
                CloseHandle(hProcess);
                continue;
            }

            // Check if the object type is "Key"
            if (wcscmp(objectTypeInfo->Name.Buffer, L"Key") == 0) {
                // Query the name of the registry key handle
                status = pNtQueryObject(duplicatedHandle, ObjectNameInformation, objectTypeInfo, 0x1000, &returnLength);
                if (NT_SUCCESS(status)) {
                    // Get the name of the object
                    std::wstring objectName(objectTypeInfo->Name.Buffer, objectTypeInfo->Name.Length / sizeof(WCHAR));
                    // Compare the registry key name with the one we are looking for
                    if (objectName == registryName) {
                        CloseHandle(hProcess);
                        return duplicatedHandle;
                    }
                }
            }

            CloseHandle(hProcess);
        }

        return nullptr;
    }
};
