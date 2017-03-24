% ideal membrane frequency ratios (first 3 modes)
m1 = 1; m2 = 1.594; m3 = 2.136;

f1 = m1*200; f2 = m2*f1; f3 = m3*f1; % modal frequencies [Hz]
d1 = 30; d2 = 55; d3 = 60; % damping coefficients [Hz/rads] (material)
a1 = 1; a2 = 0.998; a3 =0.996; % gain coeffiecients

% modal matrix (3 modes, 1 contact point)
% M = [f1, d1, a11;
%      f2, d2, a21;
%      f3, d3, a31];
 
fs = 44100;
N = 10000; % number of samples

%excitation (noise burst)
exc = rand(1,50);
exc = exc - mean(exc); % remove offset (center around mean)
exc = [exc zeros(1,N-length(exc))]; % zero padding

%init modes
mode1 = zeros(1,N); mode2 = zeros(1,N); mode3 = zeros(1,N);

% filter pole radius and pole angle
R1 = a1*exp(-d1/fs); R2 = a2*exp(-d2/fs); R3 = a3*exp(-d3/fs);
theta1 = 2*pi*f1/fs; theta2 = 2*pi*f2/fs; theta3 = 2*pi*f3/fs; % [rads/sample] 

mode1n1 = 0; mode1n2 = 0; % x[n-1], x[n-2]
mode2n1 = 0; mode2n2 = 0;
mode3n1 = 0; mode3n2 = 0;

for i=1:N
   
    mode1(i) = 2*R1*cos(theta1)*mode1n1 - R1*R1*mode1n2+exc(i); % resonant filter
    
    mode1n2 = mode1n1;
    mode1n1 = mode1(i);
    
    mode2(i) = 2*R2*cos(theta2)*mode2n1 - R2*R2*mode2n2+exc(i);
    
    mode2n2 = mode2n1;
    mode2n1 = mode2(i);
    
    mode3(i) = 2*R3*cos(theta3)*mode3n1 - R3*R3*mode3n2+exc(i);
    
    mode3n2 = mode3n1;
    mode3n1 = mode3(i);
    
end

signal = mode1 + mode2 +mode3; % summation of modes

plot(mode1); hold on; plot(mode2); hold on; plot(mode3);
legend('mode 1', 'mode 2', 'mode 3');
sound(signal, fs);