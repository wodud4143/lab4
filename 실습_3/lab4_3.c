#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_CLIENTS 10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int num_clients = 0;
int message = 0;

void *client_thread(void *arg) {
    int client_id = *((int *)arg);

    while (1) {
        pthread_mutex_lock(&mutex);
        num_clients++;
        printf("클라이언트 %d가 메시지를 보내기를 요청했습니다\n", client_id);
        pthread_cond_wait(&cond, &mutex);
        num_clients--;

        printf("클라이언트 %d가 메시지를 받았습니다: %d\n", client_id, message);
        pthread_mutex_unlock(&mutex);
    }
}

void *server_thread(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        message++;
        printf("서버가 메시지를 방송했습니다: %d\n", message);
        pthread_cond_broadcast(&cond);

        pthread_mutex_unlock(&mutex);

        usleep(1000000);
    }
}

int main() {
    pthread_t server, clients[MAX_CLIENTS];
    int client_ids[MAX_CLIENTS];

    pthread_create(&server, NULL, server_thread, NULL);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_ids[i] = i;
        pthread_create(&clients[i], NULL, client_thread, &client_ids[i]);
    }

    pthread_join(server, NULL);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        pthread_join(clients[i], NULL);
    }

    return 0;
}

