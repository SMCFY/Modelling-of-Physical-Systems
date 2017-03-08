clearvars
close all
clc

%% Marimba Strike Position

% Strike position of marimba: 0 and 1 are the ends, 0.5 center etc.
pos = 0.5; % must be between 0 and 1
% first mode
gain1 = 0.12;
temp1 = gain1 * sin(1.0 * pos * pi);
% second mode
gain2 = 0.02;
temp2 = gain2 * sin(0.03 + 3.9 * pos * pi);
% third mode
gain3 = 0.03;
temp3 = gain3 * sin(0.05 + 9.3 * pos * pi);

fs = 44100; % sampling frequency in hertz
%f = 1000; % played freqency in hertz
t = 1:1/fs:1.2; % time vector
% frequency sinusoid
%sin_f = cos(2*pi*t*f);
% applying decay
dec = -20;
dec_exp = exp(dec*t);
off = 5;

% creating the marimba sound signal with multiple sounds
f = [440 493.88 523.25 493.88 440 440 493.88 523.25 493.88 440 440 493.88 523.25 493.88 440];
sin_f = zeros(length(f),length(t));
for a = 1:length(f)
    sin_f(a,1:length(t)) = cos(2*pi*t*f(a));
sig = ((temp1 * sin_f(a,1:length(t))) + (temp2 * sin_f(a,1:length(t))) + (temp3 * sin_f(a,1:length(t)))) .* dec_exp;
soundsc(sig);
if a == off || a == 2 * off
pause(0.9)
else
    pause(0.2)
end
end