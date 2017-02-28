clear;
close all;
clc;

[y, fs] = karplus(300);

hold on;
plot(y, 'b');
ylim([-1 1])
sound(y, fs);
