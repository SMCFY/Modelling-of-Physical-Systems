clearvars
close all
clc

% Simplest Plucked String model: Karplus Strong Algorithm 
		% This matlab function implements the Karplus Strong 
		% algorithm. It uses the following difference equation. 
		% y[n] = x[n] + 0.5 y[n-N] + 0.5 y[n-(N+1)] 

freqHz= 1000;
iterations= 44100;
fs = 44100;

% Designing a Sinusoid
% f = 500;
% t = 1:100;

% Defining variables
A = 0.8; % Weighting variable Low pass filter
B = 1; % Probability for inverse Low pass filter
g = 0.8; % Gain for all pass filter (inharmonicity)
m = 0.9; % Plucked string position
% Echo some useful information?% ---------------------------------------------------------------
N = fs/freqHz; % samples
N = floor(N);

m = ceil((N+1)*m); % m for length of string/delay line

%x = cos(2*pi*f*t/fs); % producing sinusoid

% designing an impulse as excitation
% x = zeros(1,N);
% x(1) = 1;
%x(10) = 1;

% Tipuls
t = -5:5;
%w = 40e-3;
trp = tripuls(t);
x = zeros(1,N);
x(1:length(trp)) = trp;
% x(10:(length(trp)+9)) = trp;
% x(15:(length(trp)+14)) = trp;

% x = 2*rand(1,N); % producing white noise
% x = x - mean(x); % make sure that x < 1

% generate noise and init. delay line?% make sure burst and delay line agree: burst >= delay line?% -------------------- 

%y = zeros(1,N+1); % buffer

y_1 = zeros(1,m); % delay line 1
y_2 = zeros(1,N+1-m); % delay line 2

if iterations > length(x)  
diff = iterations - max(size(x));
    x = [x zeros(1,diff)]; % zero padding
end
% Filtering?% y[n] = x[n] + 0.5 y[n-N] + 0.5 y[n-(N+1)]?% ---------------------------------------------------------------------------
 % init.?
 
 % Defining variables for the first loop
 signal = 0;
 out_low_pre_1 = 0;
 out_all_pre_1 = 0;
 out_low_pre_2 = 0;
 out_all_pre_2 = 0;
 
%  ly_1 = length(y_1);
%  ly_2 = length(y_2);
 
 lengthYOffset_1 = length(y_1)-1;
 lengthYOffset_2 = length(y_2)-1;
for i=1:iterations
    var = rand(1); % create a random 
    % fist delay line
    % check if to use low pass or inverse low pass filter
    if var < B
    out_low_1 = A*y_1(lengthYOffset_1) + (1-A)*y_1(lengthYOffset_1+1); % Low pass filter
    else
        out_low_1 = -(A*y_1(lengthYOffset_1) + (1-A)*y_1(lengthYOffset_1+1)); % inverse low pass filter
    end
    
    % all pass filter
    out_all_1 = -g*out_low_1 + out_low_pre_1 + g*out_all_pre_1;
    % saving values for next loop
    out_low_pre_1 = out_low_1;
    out_all_pre_1 = out_all_1;
    
    var = rand(1); % create a random 
    % second delay line
    % check if to use low pass or inverse low pass filter
    if var < B
    out_low_2 = A*y_2(lengthYOffset_2) + (1-A)*y_2(lengthYOffset_2+1); % Low pass filter
    else
        out_low_2 = -(A*y_2(lengthYOffset_2) + (1-A)*y_2(lengthYOffset_2+1)); % inverse low pass filter
    end
    
    % all pass filter
    out_all_2 = -g*out_low_2 + out_low_pre_2 + g*out_all_pre_2;
    % saving values for next loop
    out_low_pre_2 = out_low_2;
    out_all_pre_2 = out_all_2;
    
    % Combining with input
    total_out_1 = x(i) + out_all_1;
    total_out_2 = x(i) + out_all_2;
    % filling the buffer/delay
 y_1 = [total_out_1, y_1(1:lengthYOffset_1)];
 y_2 = [total_out_1, y_2(1:lengthYOffset_2)];
 
 % summing the two delay lines
 total_out = total_out_1 + total_out_2;
 % creating the final signal
signal = [signal total_out];
end
% filter signal
% Play sound and plot?

% Convolve signal with body
body = audioread('Taylor 314ce - Scheops CMC64G.wav');
signal = conv(signal,body);

figure
 plot(signal)
title('Simple Karplus Strong algorithm')
sound(signal, fs)
pause(1.2)
sound(signal, fs)
pause(1.2)
sound(signal, fs)
