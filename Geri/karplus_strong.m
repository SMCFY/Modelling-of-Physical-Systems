fs = 44100; % sample rate
it = 44100; % iterations
p = 440; % periodicity parameter (wavetable length)

N = fs/p; N = floor(N); % delay length

x = 2*rand(1,N); % input noise
x = x - mean(x); % remove DC offset
x = x / max(x); % normalisation

y = [zeros(1,N+1)]; % delay line

% zero pad input noise if the lenght of the signal is exceeded (burst of noise)
if it > length(x)
    diff = it - max(size(x)); 
    x = [x zeros(1,diff)];
end

outLp = [0, 0]; %lpf output
outAp = 0; %apf output
signal = 0; %output signal

a = 0.5; % lowpass coeff
g = 0.7; % allpass coeff

% plucked string
 for i = 1:it

     % y[n] = ax[n]+(1-a)x[n-1] - lowpass filter(decay)
     % y[n]=-gx[n]+x[n-1]+gy[n-1] - allpass filter(dispersion)
     
     outLp(1) = x(i) + a*y(N) + (1-a)*y(N+1); %lpf
     
     outAp = -g*outLp(1) + outLp(2) + g*outAp; %apf
     
     signal = [signal, outAp]; % constructing the output signal
     
     y = [outLp(1), y(1:length(y)-1)]; % writing new values to the delay line - [write___read]
     
     circshift(outLp, 1); % shifting the values in the buffer
 end

plot(signal);
sound (signal, fs);



    