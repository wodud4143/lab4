#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define PORT 12345
#define MAX_CLIENTS 10

int main() {
    int server_socket, client_sockets[MAX_CLIENTS];
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[1024];
    fd_set readfds, current_fds;

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

    FD_ZERO(&current_fds);
    FD_SET(server_socket, &current_fds);

    int max_fd = server_socket;

    while (1) {
        readfds = current_fds;

        if (select(max_fd + 1, &readfds, NULL, NULL, NULL) == -1) {
            perror("Select 실패");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i <= max_fd; i++) {
            if (FD_ISSET(i, &readfds)) {
                if (i == server_socket) { 
                    int new_client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
                    if (new_client_socket == -1) {
                        perror("연결 수락 실패");
                        exit(EXIT_FAILURE);
                    }

                    printf("새로운 클라이언트 연결됨: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                    FD_SET(new_client_socket, &current_fds);
                    if (new_client_socket > max_fd) {
                        max_fd = new_client_socket;
                    }
                } else { 
                    int bytes_received = recv(i, buffer, sizeof(buffer), 0);
                    if (bytes_received <= 0) {
                        close(i);
                        FD_CLR(i, &current_fds);
                        printf("클라이언트 연결 해제됨\n");
                    } else {
                        buffer[bytes_received] = '\0';
                        printf("클라이언트로부터 받은 메시지: %s\n", buffer);

                    
                        for (int j = 0; j <= max_fd; j++) {
                            if (FD_ISSET(j, &current_fds) && j != server_socket && j != i) {
                                send(j, buffer, bytes_received, 0);
                            }
                        }
                    }
                }
            }
        }
    }

    close(server_socket);

    return 0;
}

