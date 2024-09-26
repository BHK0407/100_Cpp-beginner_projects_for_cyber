#include <windows.h>
/*
    This header file serves the purpose of defining NT (Windows NT API)
    function prototypes, structures, and type difinitions that will be
    used in the implementation of a Windows system-level program
*/
typedef NTSTATUS(NTAPI* NtQuerySystemInformation_t)(
    /* NtQuerySystemInformation_t: declares a function pointer to NtQuerySystemInformation,
        which queries information about various aspects of the system. */
    ULONG SystemInformationClass,       // The Class of system information being queried
    PVOID SystemInformation,            // Buffer to receive the system information
    ULONG SystemInformationLenght,      // Size of the buffer
    PULONG ReturnLength                 // Size of the data returned
);

typedef NTSTATUS(NTAPI* NtDuplicateObject_t)(
    /* NtDuplicateObject_t: declares a function to pointer to NtDuplicateObject,
        which is used to duplicate handles between processes. */
    HANDLE SourceProcessHandle,         // Handle to the source process
    HANDLE SourceHandle,                // Handle to the source object
    HANDLE TargetProcessHandle,         // Handle to the target process
    PHANDLE TargetHandle,               // Handle to the duplicated object
    ACCESS_MASK DesiredAccess,          // Access rights for the new handle
    ULONG Attributes,                   // Attributes of the new handle
    ULONG Options                       // Options for duplication
);

typedef NTSTATUS(NTAPI* NtQueryObject_t)(
    /* NtQueryObject_t: defines a function pointer to NtQueryObject,
        which retrieves information about a specific object. */
    HANDLE ObjectHandle,                // Handle to the object being queried
    ULONG ObjectInformationClass,       // Type of information to query about the object
    PVOID ObjectInformation,            // Buffer to receive the object information
    ULONG ObjectInformationLength,      // Size of the buffer
    PULONG ReturnLength                 // Size of the data returned
);

// Write Unicode String structure
typedef struct _UNICODE_STRING
{
    /* UNICODE_STRING: Structure that holds a Unicode string
        It's used by the Windows API for handling wide-character strings. */
    USHORT Length;                      // Length of the string buffer
    USHORT MaximumLength;               // Maximum length of the string buffer
    PWSTR Buffer;                       // Pointer to the string buffer (wide characters)
} UNICODE_STRING, * PUNICODE_STRING;

// System Handle Information structure
// 1. System Handle Table Entry
typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO
{
    /* SYSTEM_HANDLE_TABLE_ENTRY_INFO: Structure that represents information about a 
        single handle in the system, including the owning process and object */
    USHORT UniqueProcessId;             // Process ID that owns the handle 
    USHORT CreatorBackTraceIndex;       // Index for debugging purposes.
    UCHAR ObjectTypeIndex;              // Index of the object type
    UCHAR HandleAttributes;             // Attributes of the handle
    USHORT HandleValue;                 // Value of the handle
    PVOID Object;                       // Pointer to the object in memory
    ULONG GrantedAccess;                // Access rights associated with the handle 
} SYSTEM_HANDLE_TABLE_ENTRY_INFO, * PSYSTEM_HANDLE_TABLE_ENTRY_INFO;

// 2. System Handle Information
typedef struct _SYSTEM_HANDLE_INFORMATION
{
    /* SYSTEM_HANDLE_INFORMATION: Structure stores information about all system handles,
        where Handles[1] is a flexible array member to hold multiple 
        SYSTEM_HANDLE_TABLE_ENTRY_INFO structures */
    ULONG NumberOfHandles;              // Number of handles in the system
    SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[1];  // Arrau of handle entries
} SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

// Pool Type Enumeration
typedef enum _POOL_TYPE {
    /* POOL_TYPE: An enumeration that defines different types of memory pools that objects
        may use in the system*/
    NonPagedPool,                       // Memory that is never paged out
    PagedPool,                          // Memory that can be paged out to disk
    NonPagedPoolMustSucceed,            // Allocation from this pool must always succeed
    DontUseThisType,                    // Reserved
    NonPagedPoolCacheAligned,           // Non-paged and cache-aligned memory
    PagedPoolCacheAligned,              // Paged and cache-aligned memory
    NonPagePoolCacheAlignedMustS        // Non-paged, cache-aligned, and must succeed
} POOL_TYPE, * PPOOL_TYPE;

// Object Type Information structure
typedef struct _OBJECT_TYPE_INFORMATION
{
    /* OBJECT_TYPE_INFORMATION: Structure holds detailed information about a specific object type
        such as memory usage, access rights, and object count. */
    UNICODE_STRING Name;                // Name of the object type
    ULONG TotalNumberOfObjects;         // Total number of objects of this type
    ULONG TotalNumberOfHandles;         // Total number of handles of this type
    ULONG TotalPagedPoolUsage;          // Amount of paged pool memory used by these objects
    ULONG TotalNonPagedPoolUsage;       // Amount of non-paged pool memory used by these objects
    ULONG TotalNamePoolUsage;           // Memory used for storing object names
    ULONG TotalHandleTableUsage;        // Memory used for storing handle tables
    ULONG HighWaterNumberOfObjects;     // Maximum number of objects of this type
    ULONG HighWaterNumberOfHandles;     // Maximum number of handles of this type
    ULONG HighWaterPagedPoolUsage;      // Maximum paged pool usage
    ULONG HighWaterNonPagedPoolUsage;   // Maximum non-paged pool usage
    ULONG HighWaterNamePoolUsage;       // Maximum name pool usage
    ULONG HighWaterHandleTableUsage;    // Maximum handle table usage
    ULONG InvalidAttributes;            // Invalid attributes for object of this type
    GENERIC_MAPPING GenericMapping;     // Mapping of generic access rights to object-specific access rights
    ULONG ValidAccess;                  // Valid access rights for this object type
    BOOLEAN SecurityRequired;           // Whether security is required for this object type
    BOOLEAN MaintainHandleCount;        // Whether to track the number of handles
    USHORT MaintainTypeList;            // Type List maintenance index
    POOL_TYPE PoolType;                 // Type of memory pool used for objects of this type
    ULONG PagedPoolUsage;               // Current paged pool usage
    ULONG NonPagedPoolUsage;            // Current non-paged pool usage 
} OBJECT_TYPE_INFORMATION, * POBJECT_TYPE_INFORMATION;




