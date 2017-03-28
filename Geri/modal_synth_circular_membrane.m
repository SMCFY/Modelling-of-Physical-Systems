%% modal synthesis (circluar membrane)

fs = 44100; % sampling rate
t = [0:1/fs:1]; % sample length

% estimated gain coefficients (N*K shape matrix) (circular distribution of contact points
% at half the membrane radius, and one contact point in the center)
A = [
    1 0.8 0.8 0.8 0.8 0.8 0.8 0.8 0.8;
    0 0 0.9 1 0.9 0 0.9 1 0.9;
    0 0 1 0 1 0 1 0 1;
    1 0 0 0 0 0 0 0 0;
    0 0 0.8 1 0.8 0 0.8 1 0.8;
    0 0 0.4 0.6 0.4 0 0.4 0.6 0.4
    ];

% modal frequencies for ideal circular membrane[1] and estimated decay coefficients
% r = ; % radius [mm]
% sigma = ; % area density [mm^2]
% T = ; % surface tension
% c = sqrt(T/sigma); % transverse wave equation, with velocity "c"
% f0 = 2.405/2*pi*r*c; [Hz]
f0 = 140;

fd =[
    f0*1.000 25;
    f0*1.594 30;
    f0*2.136 35;
    f0*2.296 40;
    f0*2.653 45;
    f0*2.918 50
    ];

% modal parameter matrix
M = [fd A];

% resonator parameters
theta = zeros(length(A(:,1)),1); % pole angle [rads/sample]
for i=1:length(A(:,1))
theta(i) = 2*pi*M(i,1)/fs;
end

R = zeros(length(A(:,1)),length(A(1,:))); % pole radius
for i=1:length(A(:,1)) % number of modes
    for j=1:length(A(1,:)) %number of contact point
        R(i,j) = A(i,j)*exp(-M(i,2)/fs);
    end
end

% excitation (impact)
x = rand(1,50);
x = x - mean(x); % remove offset (center around mean)
x = x/max(x); % normalisation
x = [x zeros(1,length(t)-length(x))]; % zero padding

% output
signal = zeros(1,length(t)); % output signal
y = zeros(1,length(t)); % resonance of a mode at a given contact point

% processed samples
xn1 = 0; % x[n-1]
xn2 = 0; % x[n-2]

for n=1:length(A(:,1)) % iterate through modes
    
    for k=1:length(A(1,:)) % iteratate through contact points
        for i=1:length(t) % iterate through samples
            y(i) = 2*R(n,k)*cos(theta(n))*xn1 - R(n,k)*R(n,k)*xn2+x(i);
            xn2 = xn1;
            xn1 = y(i);
        end
        signal = signal + y;
    end
    
end

plot(signal);
sound(signal, fs);

% [1] Fletcher N. H., Rossing T. D. The Physics of Musical Instruments. 1998. Springer