#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
using namespace std;

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1) {
        perror("Socket creation failed");
        exit(1);
    }
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int result = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(result == -1) {
        perror("Connection failed");
        close(sockfd);
        exit(1);
    }

    const char* message = "hello server";
    send(sockfd, message, strlen(message), 0);

    char buffer[1024];
    int bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if(bytes_received > 0) {
        buffer[bytes_received] = '\0';
        cout << "Received" << buffer << endl;
    }

    close(sockfd);

    return 0;
}