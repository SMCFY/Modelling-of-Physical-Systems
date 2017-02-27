% Simplest Plucked String model: Karplus Strong Algorithm
% This matlab function implements the Karplus Strong
% algorithm. It uses the following difference equation.
% y[n] = x[n] + 0.5 y[n-N] + 0.5 y[n-(N+1)]

data_path='C:\Users\Aleix\Desktop\MPS\Lecture1';
addpath(data_path);
body = audioread('body.wav');
% figure;plot(body);
plucked = audioread('plucked.wav');

freqHz= 440;
iterations= 44100;
fs = 44100;
N = fs/freqHz;
N = floor(N);
x = 2*rand(1,N);
x = x - mean(x);
% x = plucked';       % For using a recorded Plucked

y = [zeros(1,N+1)];

if iterations > length(x)
    diff = iterations - max(size(x));
    x = [x zeros(1,diff)];
end
% Filtering?% 
% y[n] = x[n] + 0.5 y[n-N] + 0.5 y[n-(N+1)]

out =0;
signal = 0;
signal2 = 0;
lengthYOffset = length(y)-1;
a = 0.5;
mu = 1/2;
% y = x[n] - x[n-mu*N]                         %Pick Position Comb Filter
for i=1:iterations
    x2 = x(i) - y(floor(mu*N));
    y2 = [x2, y(1:lengthYOffset)];             
    signal2 = [signal2 x2];
end
out(1) = signal2(1) + (a*y(N) + (1-a)*y(N+1));
y = [out, y(1:lengthYOffset)];

for i=2:iterations  
%      out = x(i) + (0.5*y(N) + 0.5*y(N+1));            %Lowpass
%     y[n] = ax[n] + (1-a)x[n-1]                        %Lowpass Equation
     out(2) = signal2(i) + (a*y(N) + (1-a)*y(N+1));     %Outpt from Lowpass
%      y[n] = -gx[n] + x[n-1] + gy[n-1]                 %Allpass Equation
    out2 = -g*out(2)+out(1)+g*y(N+1);                   %Output from Allpass
    out(1) = out(2);
    y = [out2, y(1:lengthYOffset)];                     %Delay Line
    signal = [signal out2];
end
% Add distortion
for i = 1:length(signal)
    if (signal(i)*10) >= 1
        signal(i) = 2/3;
    elseif (signal(i)*10) >-1 && signal(i)<1
            signal(i) = signal(i) - signal(i)^3/3;
    elseif (signal(i)*10) <= -1
            signal(i) = -2/3;
    end
end
% Play sound and plot
convolved = conv(body,signal);
figure; plot(convolved)
title('Simple Karplus Strong algorithm')
sound(convolved, fs)