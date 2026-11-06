#include <stdio.h>

#define N 31

int main()
{
    int X[5] = {1, 0, 0, 1, 1}; // 19
    int Y[5] = {1, 1, 0, 1, 0}; // 26
    int Gold[N];
    int i, t;

    printf("Такт | Регистр X | выхX| Регистр Y | выхY| Голд\n");
    printf("-----+-----------+-----+-----------+-----+-----\n");

    for (t = 0; t < N; t++)
    {
        int outX = X[4];
        int outY = Y[4];
        Gold[t] = outX ^ outY;

        printf("%4d | ", t);
        for (i = 0; i < 5; i++)
            printf("%d ", X[i]);
        printf("|  %d  | ", outX);
        for (i = 0; i < 5; i++)
            printf("%d ", Y[i]);
        printf("|  %d  |  %d\n", outY, Gold[t]);

        int newX = X[3] ^ X[4];
        int newY = Y[1] ^ Y[4];

        for (i = 4; i > 0; i--)
            X[i] = X[i - 1];
        X[0] = newX;
        for (i = 4; i > 0; i--)
            Y[i] = Y[i - 1];
        Y[0] = newY;
    }

    printf("\nПолная последовательность Голда:\n");
    for (i = 0; i < N; i++)
        printf("%d", Gold[i]);
    printf("\n\n");

    printf("Автокорреляция\n");
    printf("Сдвиг | Последовательность после сдвига | Совп. | Несовп. |   R(τ)\n");
    printf("------+---------------------------------+-------+---------+----------\n");
    for (int shift = 0; shift < N; shift++)
    {
        int same = 0, diff = 0;
        printf("%5d | ", shift);
        for (i = 0; i < N; i++)
        {
            int bit = Gold[(i + shift) % N];
            printf("%d", bit);
            if (Gold[i] == bit)
                same++;
            else
                diff++;
        }
        double R = (double)(same - diff) / N;
        printf(" | %5d | %7d | %8.5f\n", same, diff, R);
    }

    printf("\n\nВторая последовательность (x = x+1 = 20, y = y-5 = 21)\n");
    int X2[5] = {1, 0, 1, 0, 0};
    int Y2[5] = {1, 0, 1, 0, 1};
    int Gold2[N];
    printf("Такт | Регистр X | выхX| Регистр Y | выхY| Голд\n");
    printf("-----+-----------+-----+-----------+-----+-----\n");

    for (t = 0; t < N; t++)
    {
        int outX = X[4];
        int outY = Y[4];
        Gold2[t] = outX ^ outY;

        printf("%4d | ", t);
        for (i = 0; i < 5; i++)
            printf("%d ", X[i]);
        printf("|  %d  | ", outX);
        for (i = 0; i < 5; i++)
            printf("%d ", Y[i]);
        printf("|  %d  |  %d\n", outY, Gold2[t]);

        int newX = X[3] ^ X[4];
        int newY = Y[1] ^ Y[4];

        for (i = 4; i > 0; i--)
            X[i] = X[i - 1];
        X[0] = newX;
        for (i = 4; i > 0; i--)
            Y[i] = Y[i - 1];
        Y[0] = newY;
    }
    printf("Вторая последовательность Голда:\n");
    for (i = 0; i < N; i++)
        printf("%d", Gold2[i]);
    printf("\n\n");
    int same = 0, diff = 0;
    for (i = 0; i < N; i++)
    {
        if (Gold[i] == Gold2[i])
            same++;
        else
            diff++;
    }
    double Rxy = (double)(same - diff) / N;

    printf("Взаимная корреляция (сдвиг 0):\n");
    printf("R_xy = (%d - %d)/31 = %.5f\n", same, diff, Rxy);

    return 0;
}