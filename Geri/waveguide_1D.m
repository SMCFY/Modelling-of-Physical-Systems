% struck string
fs = 44100; % sample rate
it = 44100; % iterations (1 second output)
p = 440; % periodicity parameter (f0)

N = fs/p; N = floor(N); % delay length

x = rand(1, N); x = x - mean(x); % noise excitation

% inversly loaded delay lines (struck)
L1 = [x zeros(1,N-length(x))]; % delay line (feedforward)
L2 = [-x zeros(1,N-length(x))]; % delay line (feedback)

m = N/2; % sampling position
signal = zeros(1, it);

for i=1:it

L1 = [-1*L2(N), L1(1:length(L1)-1)];
L2 = [-0.95*L1(N), L2(1:length(L2)-1)];

signal(i) = L1(N-m)+L2(N);

end

plot(signal);
sound(signal,fs);