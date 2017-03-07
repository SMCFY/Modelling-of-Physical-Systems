%% 2 modes impact model
%   mode
m = 0.2;
fs = 44100;
T = 1/fs;
N = fs*1;
k = 500000;
m2 = 0.1;
k2 = 500000;

x = zeros(1,N);
x2 = zeros(1,N);

f = 1/(2*pi) * sqrt(k/m)% resonant frequency
f2 = 1/(2*pi) * sqrt(k2/m2)% resonant frequency

fmax = 1;
T2 = 0.001;

t=[0:1/fs:T2];

for ind = 1:max(size(t))
    F(ind) = fmax *(1 - cos ((2*pi*t(ind)/T2)));
end
noise = F;

offset = length(x)-length(noise);
if length(x) > length(noise)
    noise =[noise, zeros(1,offset)];
end

R = 200;
R2 = 100;

c1 = m + R*T + k *(T*T); c2 = 2*m+R*T;
c3 = m2 + R2*T + k2 *(T*T); c4 = 2*m2+R2*T;

xm1 = 0;
xm2 = xm1;

xm11 = 0;
xm21 = xm11;

for i = 1:N
    x(i) = c2/c1*xm1 - xm2 * (m)/c1;
    x2(i) = c4/c3*xm11 - xm21 * (m2)/c3;
    
    xm2 = xm1;
    xm1 = x(i) + noise(i);
    
    xm21 = xm11;
    xm11 = x2(i) + noise(i);
end

figure
s = (x/max(x));
s2 = (x2/max(x2));
mix = (s+s2)*0.5;
plot(mix)
sound(mix,fs)
