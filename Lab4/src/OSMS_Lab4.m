N = 31;
X = [1, 0, 0, 1, 1];   % 19
Y = [1, 1, 0, 1, 0];   % 26

Gold = zeros(1, N);

fprintf('Такт | Регистр X | выхX | Регистр Y | выхY | Голд\n');
fprintf('-----+-----------+------+-----------+------+-----\n');

for t = 0:N-1

    outX = X(5);
    outY = Y(5);

    Gold(t+1) = bitxor(outX, outY);

    fprintf('%4d | ', t);
    fprintf('%d ', X);
    fprintf('|  %d   | ', outX);
    fprintf('%d ', Y);
    fprintf('|  %d   |  %d\n', outY, Gold(t+1));

    newX = bitxor(X(4), X(5));
    newY = bitxor(Y(2), Y(5));

    X = [newX, X(1:4)];
    Y = [newY, Y(1:4)];
end

fprintf('\nПолная последовательность Голда:\n');
fprintf('%d', Gold);
fprintf('\n\n');

fprintf('Автокорреляция\n');
fprintf('Сдвиг | Последовательность после сдвига | Совп. | Несовп. |   R(tau)\n');
fprintf('------+---------------------------------+-------+---------+----------\n');

R_values = zeros(1, N);

for shift = 0:N-1

    same = 0;
    diff = 0;

    fprintf('%5d | ', shift);

    for i = 0:N-1

        bit = Gold(mod(i + shift, N) + 1);

        fprintf('%d', bit);

        if Gold(i+1) == bit
            same = same + 1;
        else
            diff = diff + 1;
        end
    end

    R = (same - diff) / N;

    R_values(shift+1) = R;

    fprintf(' | %5d | %7d | %8.5f\n', same, diff, R);
end

fprintf('\n\nВторая последовательность (x = 20, y = 21)\n');

X2 = [1, 0, 1, 0, 0];   % 20
Y2 = [1, 0, 1, 0, 1];   % 21

Gold2 = zeros(1, N);

fprintf('Такт | Регистр X | выхX | Регистр Y | выхY | Голд\n');
fprintf('-----+-----------+------+-----------+------+-----\n');

for t = 0:N-1
    outX = X2(5);
    outY = Y2(5);

    Gold2(t+1) = bitxor(outX, outY);

    fprintf('%4d | ', t);
    fprintf('%d ', X2);
    fprintf('|  %d   | ', outX);
    fprintf('%d ', Y2);
    fprintf('|  %d   |  %d\n', outY, Gold2(t+1));

    newX = bitxor(X2(4), X2(5));
    newY = bitxor(Y2(2), Y2(5));

    X2 = [newX, X2(1:4)];
    Y2 = [newY, Y2(1:4)];
end

fprintf('\nВторая последовательность Голда:\n');
fprintf('%d', Gold2);
fprintf('\n\n');

same = 0;
diff = 0;

for i = 1:N

    if Gold(i) == Gold2(i)
        same = same + 1;
    else
        diff = diff + 1;
    end
end

Rxy = (same - diff) / N;

fprintf('Взаимная корреляция (сдвиг 0):\n');
fprintf('R_xy = (%d - %d)/%d = %.5f\n', same, diff, N, Rxy);

figure;
stem(0:N-1, R_values, 'Marker', 'none', 'LineWidth', 1.2);
grid on;
xlabel('Задержка (lag)');
ylabel('R(\tau)');
title('Автокорреляционная функция последовательности Голда');
xlim([0 N-1]);
ylim([-1.1 1.1]);

Gold_bipolar = 2 * Gold - 1;

[R_xcorr, lags] = xcorr(Gold_bipolar, 'coeff');

figure;
stem(lags, R_xcorr, 'Marker', 'none', 'LineWidth', 1.2);
grid on;
xlabel('Задержка (lag)');
ylabel('R(\tau)');
title('Автокорреляция с помощью xcorr');

figure;
autocorr(Gold_bipolar, N-1);
grid on;
title('Автокорреляция с помощью autocorr');
