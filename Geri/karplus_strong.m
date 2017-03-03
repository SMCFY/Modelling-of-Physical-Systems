fs = 44100; % sample rate
it = 44100; % iterations
p = 440; % periodicity parameter (wavetable length)

N = fs/p; N = floor(N); % delay length
y = [zeros(1,N+1)]; % delay line

x = rand(1,N); % input noise
x = x - mean(x); % remove DC offset
%x = x / max(x); % normalisation
x = [x zeros(1,it-length(it))];% zero pad input noise (burst of noise)

outLp = [0, 0]; % lpf output
outAp = 0; % apf output
signal = 0; % output signal

a = 0.5; % lowpass coeff
g = 0.7; % allpass coeff

m = N/2 % plucking position

% plucked string
 for i = 1:it

     % y[n] = ax[n]+(1-a)x[n-1] - lowpass filter(decay)
     % y[n]=-gx[n]+x[n-1]+gy[n-1] - allpass filter(dispersion)
     
     outLp(1) = x(i) + a*y(N) + (1-a)*y(N+1); % lpf + delay
     
     outAp = -g*outLp(1) + outLp(2) + g*outAp; % apf
     
     signal = [signal, outAp]; % constructing the output signal
     
     y = [outLp(1), y(1:length(y)-1)]; % writing new values to the delay line - [write___read]
     
     circshift(outLp, 1); % shifting the values in the buffer
 end

h = audioread('Taylor_314ce.wav'); % IR of a Taylor 314ce acoustic guitar 
signal = conv(signal, h);

plot(signal);
sound (signal, fs);
    