#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("소켓 생성 실패");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("바인딩 실패");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) == -1) {
        perror("대기 실패");
        exit(EXIT_FAILURE);
    }

    printf("서버가 포트 %d에서 대기 중입니다...\n", PORT);

    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket == -1) {
        perror("연결 수락 실패");
        exit(EXIT_FAILURE);
    }

    int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received == -1) {
        perror("데이터 수신 실패");
        exit(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0';
    printf("클라이언트로부터 받은 메시지: %s\n", buffer);

    const char *response = "Hello, Client!";
    send(client_socket, response, strlen(response), 0);

    close(client_socket);
    close(server_socket);

    return 0;
}

