%%Extended Karplus Strong Algorithm 
%Jonas Holfelt - SMC 8 - AAU CPH
%Implemented features: non-linearity (all-pass filter), variable plucking position
%convolution with guitar body impulse response
[h Hfs] = audioread('body.wav');
freqHz= 220
fs = 44100
iterations= fs;
% Echo some useful information?% ---------------------------------------------------------------
N = fs/freqHz;
N = floor(N);
%plucking position = m
m = 20;
x = 2*rand(1,N);
x = x - mean(x);


% generate noise and init. delay line?% make sure burst and delay line agree: burst >= delay line?% --------------------

y = [zeros(1,N+1)];

if iterations > length(x)
    diff = iterations - max(size(x));
    x = [x zeros(1,diff)];
end
% Filtering?% y[n] = x[n] + 0.5 y[n-N] + 0.5 y[n-(N+1)]?% ---------------------------------------------------------------------------
% init.?

out =0;

signal = 0;

lengthYOffset = length(y)-1;

a = 0.4;
g = 0.3;

xn1 = 0;
yn1 = 0;
for i=1:iterations
    %Lowpass and delay
    lowOut = x(i) + (a*y(N) + (1-a)*y(N+1));
    
    out = -g * lowOut + xn1 + g*yn1;
    
    %store previous lowOut and out
    xn1 = lowOut;
    yn1 = out;
    
    %fill signal
    y = [out, y(1:lengthYOffset)];
    
    signal = [signal (y(N/2-m) + (-y(N/2+m)))];%pick two taps for plucking position
end
% filter signal
% Play sound and plot?
figure;
plot(signal)
title('Extended Karplus Strong algorithm')
%Convolve signal with guitar body
yout = conv(signal,h(:,1));
yout =  yout/max(max(abs(yout))); %normaliza
figure;
plot(yout);
title('Karplus Strong Convolved with Guitar Body')
sound(yout, fs)