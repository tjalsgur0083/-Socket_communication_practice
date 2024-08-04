#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
using namespace std;

int main() {
    int server_sockfd = socket(PF_INET, SOCK_STREAM, 0);
    /*
        socket function
        function prototype
        int socket(int domain, int type, int protocol);
        1. communication domain. PF_INET = AP_INET (IPv4)
        2. socket type. SOCK_STREAM(TCPA) SOCK_DGPAM(UDP)
        3. Protocol to be actually used. IPPROTO_TCP(user TCP) IPPROTO_UDP(user UDP) 0(auto)
    */
    
    if(server_sockfd < 0) {
        perror("abnormal socket");
        exit(1); //1 = Failure
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr)); //memset prototype void* memset(void* ptr, int value, size_t num);
    //memset => memory initialization
    server_addr.sin_family = AF_INET; //AF_INET(IPv4)
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY; // any access allow

    //socket binding
    if(bind(server_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding  failed");
        close(server_sockfd);
        exit(1);
    }
    //int bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);

    if(listen(server_sockfd, 5) == -1) { // listen function is passive mode 수신 대기 설정
        perror("Listening failed");
        close(server_sockfd);
        exit(1);
    }
    
    printf("Server is Listening on port 8080....");

    //Accept client connection
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_addr, &client_addr_len);
    /*
        accept function
        function prototype
        int accept(int sockfd, struct sockaddr* addr, socklen_t *addrlen)
    */

    if(client_sockfd == -1) {
        perror("Client connection acceptance failde");
        close(server_sockfd);
        exit(1);
    }

    char buffer[1024];
    int bytes_recevied = recv(client_sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_recevied > 0) {
        buffer[bytes_recevied] = '\0';
        cout << "Received from client: " << buffer << endl;

        const char* response = "Hello World";
        send(client_sockfd, response, strlen(response), 0);
    }

    close(client_sockfd);
    close(server_sockfd);
    
    return 0;
}