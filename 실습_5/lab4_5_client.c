#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("소켓 생성 실패");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("연결 실패");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("전송할 메시지를 입력하세요 ('quit'를 입력하여 종료): ");
        fgets(buffer, sizeof(buffer), stdin);

        if (strcmp(buffer, "quit\n") == 0) {
            printf("종료합니다...\n");
            break;
        }

        send(client_socket, buffer, strlen(buffer), 0);

        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received == -1) {
            perror("데이터 수신 실패");
            exit(EXIT_FAILURE);
        }

        buffer[bytes_received] = '\0';
        printf("서버로부터 받은 응답: %s\n", buffer);
    }

    close(client_socket);

    return 0;
}

