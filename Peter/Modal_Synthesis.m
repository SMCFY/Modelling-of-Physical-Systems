clearvars
close all
clc

%% Modal Synthesis

% Define white noise with length N
N = 44100; y = 2*rand(1,N); y = y - mean(y);

% Define Frequencies for two modes
freq_1 = 250.98;
freq_2 = 1000;

% Define decaying factors for two modes
bw_1 = 60;
bw_2 = 100;

% Sampling frequency and length of modes
fs = 44100;
t=0:1/fs:1;

% Predefine modes for later computation
mode1 = zeros(1,length(t));
mode2 = zeros(1,length(t));

% Define decaying variable
R1 = exp(-bw_1/fs);
R2 = exp(-bw_2/fs);

% Define radiant frequency
omega1 = 2*pi*freq_1;
omega2 = 2*pi*freq_2;

% Define normalized radiant frequency
theta1 = omega1/fs;
theta2 = omega2/fs;

% Delaying mode values
mode1n1 = 0;
mode1n2 = 0;
mode2n1 = 0;
mode2n2 = 0;

% Calculation of the two modes
for i = 1:N
mode1(i) = 2*R1*cos(theta1)*mode1n1 - R1*R1*mode1n2+y(i);
mode1n2 = mode1n1;
mode1n1 = mode1(i);

mode2(i) = 2*R2*cos(theta2)*mode2n1 - R2*R2*mode2n2+y(i);
mode2n2 = mode2n1;
mode2n1 = mode2(i);

end

% Combine playback and plot two modes
sig = mode1+mode2;
soundsc(sig,fs);
figure
hold on
plot(mode1)
plot(mode2)
hold off