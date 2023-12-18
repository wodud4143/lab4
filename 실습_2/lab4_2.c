#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2

int buffer[BUFFER_SIZE];
int in = 0, out = 0;
sem_t empty, full, mutex;

void *생산자(void *arg) {
    int 아이템;
    while (1) {
        아이템 = rand() % 100; // 랜덤한 아이템 생성

        sem_wait(&empty);
        sem_wait(&mutex);

        buffer[in] = 아이템;
        printf("생산자 %ld가 아이템 %d을(를) 생산했습니다\n", (long)arg, 아이템);
        in = (in + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&full);

        // 임의의 시간 동안 대기 (생산 속도 조절)
        usleep(100000);
    }
}

void *소비자(void *arg) {
    int 아이템;
    while (1) {
        sem_wait(&full);
        sem_wait(&mutex);

        아이템 = buffer[out];
        printf("소비자 %ld가 아이템 %d을(를) 소비했습니다\n", (long)arg, 아이템);
        out = (out + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&empty);

        // 임의의 시간 동안 대기 (소비 속도 조절)
        usleep(150000);
    }
}

int main() {
    pthread_t 생산자들[NUM_PRODUCERS];
    pthread_t 소비자들[NUM_CONSUMERS];

    // 세마포어 초기화
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    // 생산자 스레드 생성
    for (long i = 0; i < NUM_PRODUCERS; i++) {
        pthread_create(&생산자들[i], NULL, 생산자, (void *)i);
    }

    // 소비자 스레드 생성
    for (long i = 0; i < NUM_CONSUMERS; i++) {
        pthread_create(&소비자들[i], NULL, 소비자, (void *)i);
    }

    // 스레드 종료 대기
    for (long i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(생산자들[i], NULL);
    }

    for (long i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(소비자들[i], NULL);
    }

    // 세마포어 정리
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}

