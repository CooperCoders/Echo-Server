#include <iostream>
#include <winsock2.h>
#include <stdlib.h>
#define ECHOMAX 255

using namespace std;

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in echoServAddr;
    struct sockaddr_in echoClntAddr;
    char echoBuffer[ECHOMAX];
    unsigned short echoServPort;
    int cliLen;
    int recvMsgSize;
    WSADATA wsaData;
    /*Port for the server to recieve messages.
    If coming from a non local IP, you will ned to set up port forwarding*/
    echoServPort = 32000;
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
    {
        cout << "WSAStartup() failed\n";
        exit(1);
    }
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        cout << "Socket creation failed\n";
        exit(0);
    }

    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    echoServAddr.sin_port = htons(echoServPort);

    if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
    {
        cout << "Bind failed\n";
        exit(0);
    }

    while(1)
    {

        cliLen = sizeof(echoClntAddr);

        if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0,(struct sockaddr *) &echoClntAddr, &cliLen)) < 0)
        {
            cout << "recvfrom failed\n";
            exit(0);
        }
        cout << "Handling client: " << inet_ntoa(echoClntAddr.sin_addr) << endl;
        echoBuffer[recvMsgSize] = 0;
        cout << echoBuffer << endl;

        if (sendto(sock, echoBuffer, recvMsgSize, 0, (struct sockaddr *) &echoClntAddr,sizeof(echoClntAddr)) != recvMsgSize)
        {
            cout << "sendto failed\n";
            exit(0);
        }
    }
    closesocket(sock);
    WSACleanup();
}
