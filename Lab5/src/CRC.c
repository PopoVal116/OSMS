#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N_LEN 39 // 20+19
#define CRC_LEN 7
#define POLY_LEN 8
// G=x7+x6+x+1  11000011

#define N_NEW_LEN 250

const int G[POLY_LEN] = {1, 1, 0, 0, 0, 0, 1, 1};

void calc_crc(int data[], int data_len, int result[])
{
    int temp[data_len + CRC_LEN + 5];
    int i, j;
    for (i = 0; i < data_len; i++)
    {
        temp[i] = data[i];
    }
    for (i = data_len; i < data_len + CRC_LEN; i++)
    {
        temp[i] = 0;
    }
    for (i = 0; i < data_len; i++)
    {
        if (temp[i] == 1)
        {
            for (j = 0; j < POLY_LEN; j++)
            {
                temp[i + j] = temp[i + j] ^ G[j];
            }
        }
    }
    for (i = 0; i < CRC_LEN; i++)
    {
        result[i] = temp[data_len + i];
    }
}

void generate_data(int data[], int N)
{
    for (int i = 0; i < N; i++)
        data[i] = rand() % 2;
}

void create_packet(int data[], int crc[], int packet[], int N)
{
    for (int i = 0; i < N; i++)
        packet[i] = data[i];

    for (int i = 0; i < CRC_LEN; i++)
        packet[N + i] = crc[i];
}

void copy_array(int source[], int destination[], int size)
{
    for (int i = 0; i < size; i++)
        destination[i] = source[i];
}

void run_crc_test(int N)
{
    int data[N];
    int crc[CRC_LEN];
    int i;
    int packet[N + CRC_LEN];
    generate_data(data, N);
    calc_crc(data, N, crc);
    printf("Данные (%d бит): ", N);
    for (i = 0; i < N; i++)
        printf("%d", data[i]);
    printf("\nCRC:    ");
    for (i = 0; i < CRC_LEN; i++)
        printf("%d", crc[i]);
    printf("\n");
    create_packet(data, crc, packet, N);

    int temp[N + CRC_LEN];

    copy_array(packet, temp, N + CRC_LEN);
    for (i = 0; i < N; i++)
    {
        if (temp[i] == 1)
        {
            for (int j = 0; j < POLY_LEN; j++)
                temp[i + j] ^= G[j];
        }
    }

    printf("Проверка CRC: ");
    int error = 0;

    for (i = N; i < N + CRC_LEN; i++)
    {
        printf("%d", temp[i]);

        if (temp[i] != 0)
            error = 1;
    }

    printf("\n");

    if (error == 0)
    {
        printf("Ошибок нет\n");
    }
    else
    {
        printf("Обнаружена ошибка\n");
    }
}

void run_error_test(int N)
{
    int data[N];
    int crc[CRC_LEN];
    int packet[N + CRC_LEN];
    int received[N + CRC_LEN];

    int detected = 0;
    int not_detected = 0;

    generate_data(data, N);
    calc_crc(data, N, crc);
    create_packet(data, crc, packet, N);

    for (int i = 0; i < N + CRC_LEN; i++)
    {
        copy_array(packet, received, N + CRC_LEN);

        received[i] ^= 1;

        int temp[N + CRC_LEN];

        copy_array(received, temp, N + CRC_LEN);

        for (int j = 0; j < N; j++)
        {
            if (temp[j] == 1)
            {
                for (int k = 0; k < POLY_LEN; k++)
                    temp[j + k] ^= G[k];
            }
        }

        int error = 0;

        for (int j = N; j < N + CRC_LEN; j++)
        {
            if (temp[j] != 0)
                error = 1;
        }

        if (error)
            detected++;
        else
            not_detected++;
    }
    printf("Всего проверено битов: %d\n", N + CRC_LEN);
    printf("Ошибок обнаружено: %d\n", detected);
    printf("Ошибок не обнаружено: %d\n", not_detected);
}

int main()
{
    run_crc_test(N_LEN);
    run_crc_test(N_NEW_LEN);
    run_error_test(N_NEW_LEN);
}
