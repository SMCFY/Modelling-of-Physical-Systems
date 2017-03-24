fs = 44100;
t = [0:1/fs:1];

% gain coefficients 
A = [0.99 1 0.98;
    1 0.98 0.99;
    0.99 0.99 1];

% modal frequencies and modal decay coefficients
fd =[180 30;
    240 40;
    300 60];

% modal matrix
M = [fd A];

signal = 0;
ynk = 0; % impulse response of a mode at k points

for n=1:3 % iterate through modes
    
    for k=3:5 % iteratate through points
    y = M(n,k)*exp(-M(n,2)*t).*sin(2*pi*M(n,1)*t);
    
    ynk = ynk + y;
    end
    
    signal = signal + ynk;
    
end

plot(signal);
sound(signal, fs);