clearvars
close all
clc

%% Lecture Exercise

m = 100;
fs = 44100;
T = 1/fs;
N = 1000000;
R = 100;
k = 500000000;
c1 = m + R*T + k*(T^2);
c2 = 2*m + R*T;
x = zeros(1,N);
%xm1 = 0.5;
noise = rand(1,N);
noise = noise - mean(noise);
noise(1:2) = 0.5;
xm1 = 1;
xm2 = xm1;
f = (1/2*pi)*(sqrt(k/m));

for a = 1:N
    %x(a) = (2*m)/(m+k*T*T)*xm1 - xm2*(m)/(m+k*T*T);
    %xm1 = noise(a);
    %xm2 = noise(a-1);
    x(a) = c2/c1*xm1 - xm2*(m)/c1 + noise(a);
    xm2 = xm1;
    xm1 = x(a);
end

figure
plot(x)
grid
xlabel('Time Steps (samples)');
ylabel('Mas Displacement (m)')

soundsc(x,fs);