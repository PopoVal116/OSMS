#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 31
#define M 5

// доп
/*нужно добавить проверку автоматизированную на удовлетворение
вашей М-последовательности трем свойствам М-последовательности,
доказать, что она реально М-последовательность. или не она...*/
void chek(int *seq)
{
    int i;
    int ones = 0, zeros = 0;
    for (i = 0; i < N; i++)
    {
        if (seq[i] == 1)
            ones++;
        else
            zeros++;
    }
    int balance_ok = (abs(ones - zeros) <= 1);
    printf("Проверка свойств последовательности\n\n");
    printf("1) Баланс (balance):\n");
    printf("единиц = %d, нулей = %d, |разница| = %d\n", ones, zeros, abs(ones - zeros));
    printf("Результат: %s\n\n", balance_ok ? "выполнено" : "не выполнено");

    int run_len[N + 1] = {0};
    int start = 0;
    while (start < N && seq[start] == seq[(start - 1 + N) % N])
    {
        start++;
    }
    int current_len = 1;
    for (i = 1; i <= N; i++)
    {
        int curr = seq[(start + i) % N];
        int prev = seq[(start + i - 1) % N];
        if (curr == prev)
        {
            current_len++;
        }
        else
        {
            run_len[current_len]++;
            current_len = 1;
        }
    }
    printf("2) Цикличность (runs):\n");
    printf("   Длина | Кол-во | Доля     | Ожидается\n");
    printf("   ------+--------+----------+----------\n");

    int total = 0;
    for (int k = 1; k <= N; k++)
        total += run_len[k];
    int runs_ok = 1;
    for (int k = 1; k <= 6; k++)
    {
        double share = (total > 0) ? (double)run_len[k] / total : 0;
        double expected = 1.0 / (1 << k);

        printf("   %5d | %6d | %6.1f%%  | %5.1f%%\n",
               k, run_len[k], share * 100, expected * 100);
        if (k <= 3 && fabs(share - expected) > 0.1)
            runs_ok = 0;
    }

    printf("   Результат: %s\n\n", runs_ok ? "Выполнео" : "Не выполнено");

    printf("3) Автокорреляция:\n");
    printf("Сдвиг | Совпадений | Несовпадений |   R(τ)\n");
    printf("------+------------+--------------+---------\n");
    int m_auto_ok = 1;
    int gold_auto_ok = 1;
    double ideal_m = -1.0 / N;
    double level_1 = -1.0 / N;
    double level_2 = -9.0 / N;
    double level_3 = 7.0 / N;
    for (int shift = 0; shift < N; shift++)
    {
        int same = 0;
        int diff = 0;
        for (int i = 0; i < N; i++)
        {
            int shifted_bit = seq[(i + shift) % N];
            if (seq[i] == shifted_bit)
                same++;
            else
                diff++;
        }
        double R = (double)(same - diff) / N;
        printf("%5d | %10d | %12d | %7.4f", shift, same, diff, R);

        if (shift == 0)
        {
            printf("Пик (1.0)\n");
            if (fabs(R - 1.0) > 1e-6)
            {
                m_auto_ok = 0;
                gold_auto_ok = 0;
            }
        }
        else
        {
            int is_gold_level = (fabs(R - level_1) < 1e-6) ||
                                (fabs(R - level_2) < 1e-6) ||
                                (fabs(R - level_3) < 1e-6);
            if (!is_gold_level)
                gold_auto_ok = 0;
            if (fabs(R - ideal_m) > 1e-6)
                m_auto_ok = 0;
        }
    }
    printf("\n   Итог как для М-последовательности: %s\n", m_auto_ok ? "Выполнено" : "Не выполнено");
    printf("   Итог как для кода Голда: %s\n", gold_auto_ok ? "Выполнено" : "Не выполнено");
}

int main()
{
    int X[5] = {1, 0, 0, 0, 0}; // 19
    int Y[5] = {1, 0, 0, 0, 0}; // 26
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

        int newX = X[1] ^ X[4];
        int newY = Y[1] ^ Y[2] ^ Y[3] ^ Y[4];

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
        int outX = X2[4];
        int outY = Y2[4];
        Gold2[t] = outX ^ outY;

        printf("%4d | ", t);
        for (i = 0; i < 5; i++)
            printf("%d ", X2[i]);
        printf("|  %d  | ", outX);
        for (i = 0; i < 5; i++)
            printf("%d ", Y2[i]);
        printf("|  %d  |  %d\n", outY, Gold2[t]);

        int newX = X2[1] ^ X2[4];
        int newY = Y2[1] ^ Y2[2] ^ Y[3] ^ Y[4];

        for (i = 4; i > 0; i--)
            X2[i] = X2[i - 1];
        X2[0] = newX;
        for (i = 4; i > 0; i--)
            Y2[i] = Y2[i - 1];
        Y2[0] = newY;
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
    chek(Gold);
    chek(Gold2);

    return 0;
}