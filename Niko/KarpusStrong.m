% Karpus strong with inharmonicity, body and pluck

fs = 44100; %fs/2
f = 220; % Hz
N = fs/f;
%N = floor(N);
N = floor(N/2); %
m = 49; % pluck position

x = 2*rand(1,10);
x = x - mean(x); % remove DC off
%pluck = linspace(0,N);
%x = (pluck);
%x = [1, zeros(1,N-1)]; % Impulse response
y = zeros(1,N+1); %  = [zeros(1,fs + delay)];
y2 = zeros(1, N+1); % delayline

t = 44100; % length of sound in samples

out = 0;
signal = 0;

lengthYOffset = length(y)-1;

a = 0.5; % lowpass coefficient

if t > length(x)
    diff = t - max(size(x));
    x = [x zeros(1,diff)];
end

g = 0.5; % allpass coefficient

yn1 = 0; % buffers
xn1 = 0; % buffers

for i = 1:t % t is the iterations
    % delay --> lowpass
    temp = x(i) + (a*y(N) + (1-a)*y(N+1));
    % --> allpass
    out = -g*temp + xn1 + g*yn1;
    yn1 = out; % feedback coef, y (after allpass)
    xn1 = temp; % x (before allpas)
    % --> delay 2
    y2 = [out, y2(1:N)];
    % feed delay 2 into delay 1
    y = [y2(N+1), y(1:N)];
    % tap the delaylines at N-m and m.
    signal = [signal y(N-m)+(-y2(m))];
end

[imp,fs1] = audioread('aguitar.wav');
% convolve
s = conv(signal*0.2,imp(:,2));
plot(s(1,:))
%s = signal*0.2;
sound(s,fs);
%%
fs = 44100; %fs/2
f = 220; % Hz
N = fs/f;
%N = floor(N);
N = floor(N/2); %
m = 20; % pluck position

x = 2*rand(1,N);
x = x - mean(x); % remove DC off
%pluck = linspace(0,N);
%x = (pluck);
%x = [1, zeros(1,N-1)]; % Impulse response
y = zeros(1,N+1); %  = [zeros(1,fs + delay)];
y2 = zeros(1, N+1); % delayline

t = 44100; % length of sound in samples

out = 0;
signal = 0;

lengthYOffset = length(y)-1;

a = 0.5; % lowpass coefficient

if t > length(x)
    diff = t - max(size(x));
    x = [x zeros(1,diff)];
end

g = 0.5; % allpass coefficient

yn1 = 0; % buffers
xn1 = 0; % buffers

for i = 1:t % t is the iterations
    % delay --> lowpass
    temp = x(i) + (a*y(N) + (1-a)*y(N+1));
    % --> allpass
    out = -g*temp + xn1 + g*yn1;
    yn1 = out; % feedback coef, y (after allpass)
    xn1 = temp; % x (before allpas)
    % --> delay 2
    y2 = [out, y2(1:N)];
    % feed delay 2 into delay 1
    y = [y2(N+1), y(1:N)];
    % tap the delaylines at N-m and m.
    signal = [signal y(N-m)+(-y2(m))];
end

dist = zeros(size(signal));
for i=1:length(signal)
    if signal(i)*10 >= 1
        dist(i) = (2/3);
    elseif signal(i)*10 < 1 && signal(i)*10 > -1
        dist(i) = signal(i) - signal(i)^3/3;
    elseif signal(i)*10 <= -1
        dist(i) = -(2/3);
    end
end

[imp,fs1] = audioread('aguitar.wav');
% convolve
s = conv(dist,imp(:,2));
soundsc(s, fs)