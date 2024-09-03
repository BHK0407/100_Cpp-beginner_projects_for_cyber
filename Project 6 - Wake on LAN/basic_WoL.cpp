#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>         // For socket programming functions and definitions
#include <netinet/in.h>         // For internet address family structures
#include <arpa/inet.h>          // For functions to manipulate IP addresses
#include <string.h>
#include <sys/types.h>

using namespace std;
// Main function
int main(){
    int i;
    unsigned char toSend[102], mac[6];
    struct sockaddr_in udpClient, udpServer;
    int broadcast = 1;

    // UDP Socket creation
    int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    // This creates a UDP socket
    // AF_INET specifies the address family as IPv4
    // SOCK_DGRAM specifies that this is a UDP (datagram) socket
    // The third argument is protocol, and 0 lets the sys tem choose the appropriate protocol (UDP for SOCK_DGRAM)
    
    // Manipulating the Socket to allow broadcasting
    // The 'setsockopt' function configures the socket to allow broadcasting
    // 'SOL_SOCKET' specifies the socket level
    if (setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast) == -1) {
        // If the functions fails, it will output an error and exit the program
        perror("setsockopt (SO_BROADCAST)");
        exit(EXIT_FAILURE);
    }
    // Setting up the client address structure
    udpClient.sin_family = AF_INET;             // 'sin_family' specifies the address family as IPv4
    udpClient.sin_addr.s_addr = INADDR_ANY;     // 'sin_addr.s_addr' specifies the IP address as INADDR_ANY, allowing the system to use any available network interface
    udpClient.sin_port = 0;                     // 'sin_port' is set to 0, allowing the OS to choose any available port

    // Binding the socket to the client address and port
    bind(udpSocket, (struct sockaddr*)&udpClient, sizeof(udpClient));
    
    // Preparing the magic packet
    /* The first 6 bytes of the magic packet are all 0xFF
    This is the sync stream that alerts the receiving computer that a Wake-on-LAN is incoming*/
    for (i = 0; i < 6; i++) {
        toSend[i] = 0xFF;
    }
    
    // Setting up the MAC address
    // Replace the example MAC address with the actual MAC address of the target computer
    mac[0] = 0xAB;                              // 1st octet of the MAC address
    mac[1] = 0xCD;                              // 2nd octet of the MAC address
    mac[2] = 0xEF;                              // 3rd octet of the MAC address
    mac[3] = 0x12;                              // 4th octet of the MAC address 
    mac[4] = 0x34;                              // 5th octet of the MAC address 
    mac[5] = 0x56;                              // 6th octet of the MAC address 
    // Filling the rest of the magic packet with the MAC address repeated 16 times
    for (i = 1; i <= 16; i++){
        memcpy(&toSend[i*6], &mac, 6*sizeof(unsigned char));
        // 'memcpy' is used to copy the MAC address into the 'toSend' array, starting from the 7th byte (1*6) and repeated 16 times
    }
    // Setting up the server address to send the magic packet to
    udpServer.sin_family = AF_INET;
    udpServer.sin_addr.s_addr = inet_addr("192.168.1.8");
    udpServer.sin_port = htons(9);      // default port for Wake-on-LAN

    // Sending the magic packet
    sendto(udpSocket, &toSend, sizeof(unsigned char) * 102, 0, (struct sockaddr*)&udpServer, sizeof(udpServer));
    // sendto function sends the magic packet through the socket to the specified broadcast address and port

    return 0;
}
