#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 

void bubbleSort(int arr[], int size);
void selectSort(int arr[], int size);
void insertSort(int arr[], int size);

int main() {
    int length = 100000;
    int mass[length];
    pid_t pids[3];
    srand(time(NULL));
    pid_t pid;

    for (int i = 0; i < length; i++) {
        mass[i] = rand();
    }


    for (int i = 0; i < 3; ++i) {
        pid = fork();
        pids[i] = pid;
        switch(pid) {
            case -1:
                perror("fork");
                exit(1);
                break;
            case 0:
                if (i == 0) {
                    bubbleSort(mass, length);
                }
                else if (i == 1) {
                    insertSort(mass, length);
                }
                else if (i == 2) {
                    selectSort(mass, length);
                }
                exit(0);
                break;
            case 1:
                break;      
        }
    }
    for (int i = 0; i < 3; ++i) {
        waitpid(pids[i], NULL, 0);
    }  
    
}

void bubbleSort(int arr[], int size) {
    int tmp, i, j;

    pid_t pid = getpid();
    time_t start = time(NULL);

    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - 1; ++j) {
            if (arr[j + 1] < arr [j]) {
                tmp = arr[j + 1];
                arr[j + 1] = arr [j];
                arr[j] = tmp;
            }
        }
    }

    time_t result = time(NULL) - start;

    printf("Процесс: %d (bubbleSort). Время сортировки %li секунд\n", pid, result);
}

void selectSort(int arr[], int size) {
    int tmp, i, j, pos;

    pid_t pid = getpid();
    time_t start = time(NULL);

    for (int i = 0; i < size; ++i) {
        pos = i;
        tmp = arr[i];
        for (int j = i + 1; j < size; ++j) {
            if (arr[j] < tmp) {
                pos = j;
                tmp = arr[j];
            }
        }
        arr[pos] = arr[i];
        arr[i] = tmp;
    }

    time_t result = time(NULL) - start;

    printf("Процесс: %d (selectSort). Время сортировки %li секунд\n", pid, result);
}

void insertSort(int arr[], int size) {
    int i, j, tmp;

    pid_t pid = getpid();
    time_t start = time(NULL);

    for (int i = 0; i < size; ++i) {
        tmp = arr[i];
        for (int j = i - 1; j >= 0 && arr[j] > tmp; --j) {
            arr[j + 1] = arr[j];
            arr[j] = tmp;   
        }
    }

    time_t result = time(NULL) - start;

    printf("Процесс: %d (insertSort). Время сортировки %li секунд\n", pid, result);
}
