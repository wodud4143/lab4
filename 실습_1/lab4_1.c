#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5

void *thread_function(void *arg) {
    int thread_id = *((int *)arg);
    printf("스레드 %d가 실행 중입니다\n", thread_id);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i;
        int result = pthread_create(&threads[i], NULL, thread_function, &thread_args[i]);
        if (result) {
            fprintf(stderr, "스레드 %d 생성 오류\n", i);
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        int result = pthread_join(threads[i], NULL);
        if (result) {
            fprintf(stderr, "스레드 %d 대기 오류\n", i);
            return 1;
        }
    }

    printf("모든 스레드가 완료되었습니다.\n");
    return 0;
}

