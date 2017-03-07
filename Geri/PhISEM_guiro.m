% PhISEM guiro
% Collisions are determined by a Poisson process.
% Sound envelope is additively increased by each collision

fs = 44100;
T = 1 / fs;
N = 1 * fs; %output length
nBeans = 10; % number of beans

shellF1 = 2500.0;   % shell frequency in Hz
shellR1 = 0.97; % shell resonance (damping)
shellF2 = 4000.0;   % shell frequency in Hz
shellR2 = 0.97; % shell resonance (damping)

systemDecay = 1; % decay of shake energy
soundDecay = 0.95; %decay of produced sound
y = zeros(1, N); % output

Ns = 0.5 * fs; % length of excitation
shakeEnergy = 1.0 - cos(2*pi*[0:Ns-1]/Ns); % excitation signal

gain = log( nBeans ) / (log( 4.0 ) * 40.0 * nBeans);

%filter coefficients
b = [1 -1];
a1 = [1 -2 * shellR1 * cos( 2 * pi * shellF1 * T ) shellR1^2];
a2 = [1 -2 * shellR2 * cos( 2 * pi * shellF2 * T ) shellR2^2];

sndLevel = 0; %sound produced on collision
z1 = [0 0]; % filter buffer
z2 = [0 0]; % filter buffer
k = 1; % excitation signal index
lengthVar = 6; % variability in shake lenght
shakeLength = Ns/(round((rand(1)*lengthVar))+6); % shake length

for n = 1:N
  
  e = shakeEnergy(k) * systemDecay;   
  if ( rand(1) * 1/8 < nBeans ) % collision  
    sndLevel = sndLevel + (gain * e); % energy according to excitation 
    x = sndLevel;
  else
    x = 0;
  end

  [temp z1] = filter( [1 -1], a1, x, z1 ); % filtered excitation
  [y(n) z2] = filter( [1 -1], a2, temp, z2 ); % filtered excitation
  
  sndLevel = sndLevel * soundDecay; % decay energy for stability
  
  k = k + 1; % increment excitation signal index
  if ( k > shakeLength)
      k = 1; % initialize excitation signal index
      shakeLength = Ns/(round((rand(1)*lengthVar))+6); % reinitialize condition
  end
  
end

ynorm = 0.95 * y / max(abs(y)); % normalised output
sound( ynorm, fs );