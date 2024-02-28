

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define BLUE    "\033[34m"
#define YELLOW  "\033[33m"

char *buildblock(int size) {
    char *out_str = malloc(size + 1);
    if(out_str == NULL) {
        printf("Error de asignación de memoria\n");
        exit(1);
    }
    for (int i = 0; i < size; i++) {
        out_str[i] = rand() % 26 + 'A';
    }
    out_str[size] = '\0';
    return out_str;
}

void print_progress(char *symbol, int percent) {
    printf("\r[%s%.*s%s%.*s] %d%%", RED, percent/10, "==========", RESET, 10 - percent/10, "          ", percent);
    fflush(stdout);
}

void delay(int seconds) {
    sleep(seconds);
}

void *httpcall(void *arg) {
    char *url = (char *)arg;
    int code = 0;
    char *user_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36";
    char *referer = "http://www.google.com/?q=";
    struct timespec wait_time = {0, 100000000}; // 100 milliseconds

    while(1) {
        if (url[strlen(url) - 1] != '/') {
            strcat(url, "/");
        }
        char *request_url = malloc(strlen(url) + 50); // allocate memory for request URL
        sprintf(request_url, "%s?%s=%s", url, buildblock(rand() % 8 + 3), buildblock(rand() % 8 + 3));
        
        // Send HTTP request
        printf("Sending request to: %s\n", request_url);
        // Simulate some delay
        nanosleep(&wait_time, NULL);
        
        free(request_url);
    }
    return NULL;
}

void *monitor(void *arg) {
    int previous = 0;
    int *request_counter = (int *)arg;
    struct timespec wait_time = {1, 0}; // 1 second

    while (1) {
        if (*request_counter > previous + 100) {
            printf("%d Requests Sent\n", *request_counter);
            previous = *request_counter;
        }
        // Simulate some delay
        nanosleep(&wait_time, NULL);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    char *web;
    int request_counter = 0;
    pthread_t http_thread, monitor_thread;

    printf("\n\n");
    print_progress("", 0);
    delay(1);
    for (int i = 10; i <= 100; i += 10) {
        print_progress("=", i);
        delay(1);
    }
    printf("\n    [===============     ] 85%%\n");
    delay(1);
    printf("    [====================] 100%%\n");
    delay(3);
    printf("Loading %s[%s==========]%s 100%%\n", BLUE, RED, RESET);
    delay(2);

    if (argc < 2) {
        printf("Ingresa Web Objetivo: ");
        scanf("%s", web);
    } else {
        web = argv[1];
    }

    printf("Web elegida: %s\n", web);

    // Create threads
    pthread_create(&http_thread, NULL, httpcall, (void *)web);
    pthread_create(&monitor_thread, NULL, monitor, (void *)&request_counter);

    // Join threads
    pthread_join(http_thread, NULL);
    pthread_join(monitor_thread, NULL);

    return 0;
}
