clearvars
close all
clc

%% Phisical informed objects
% Collisions are determined by a Poisson process.
% sound envelope is additively increased by each collision
% number of particles
% size of particles
% energy of excitation (shakeEnergy)

% SYNTHESIS TOOLKIT

% Sekere
% nBeans = 64
% System decay = 0.999
% Sound decy = 0.96
% shellF = 5500
% shellR = 0.6

% Cabasa
% nBeans = 512
% System decay = 0.997
% Sound decay = 0.95
% shellF = 3000
% shellR = 0.7

% Guiro
% nBeans = 128
% System decay = 0
% Sound decay = 0.95
% shellF_1 = 2500
% shellF_2 = 4000
% shellR_1 = 0.97
% shellR_2 = 0.97

% Tambourine
% nBeans = 32
% System decay = 0.9985
% Sound decay = 0.95
% shellF_1 = 2300
% shellF_2 = 5600
% shellF_3 = 8100
% shellR_1 = 0.96
% shellR_2 = 0.995
% shellR_3 = 0.995

% Water drops
% nBeans = 0.1250
% System decay = 0.999
% Sound decay = 0.95
% shellF_1 = 450
% shellF_2 = 600
% shellF_3 = 750
% shellR_1 = 0.9985
% shellR_2 = 0.9985
% shellR_3 = 0.9985


fs = 22050;
T = 1 / fs;
N = 1 * fs;
nBeans = 32;
shellF_int = 2300;   % frequency in Hz
shellF_2_int = 5600;
shellF_3_int = 8100;
shellR = 0.96;
shellR_2 = 0.995;
shellR_3 = 0.995;
per = 0.01;
systemDecay = 0.9985;
soundDecay = 0.95;
y = zeros(1, N);
y_2 = zeros(1, N);
y_3 = zeros(1, N);

Ns = 0.5 * fs;

shakeEnergy = 1.0 - cos(2*pi*[0:Ns-1]/Ns); 


shakeEnergy = [ shakeEnergy 0 ];

gain = log( nBeans ) / (log( 4.0 ) * 40.0 * nBeans);

b = [1 -1];

% a = [1 -2 * shellR * cos( 2 * pi * shellF * T ) shellR^2];
% a_2 = [1 -2 * shellR_2 * cos( 2 * pi * shellF_2 * T ) shellR_2^2];

sndLevel = 0;
z = [0 0];
z_2 = [0 0];
z_3 = [0 0];
% how fast you shake it change it also inside the loop
k = 1;

for n = 1:N
  
  e = shakeEnergy( k ) * systemDecay;
  % check this with probability
  if ( rand(1) * 1024 < nBeans )     % if collision  
    sndLevel = sndLevel + (gain * e);     % add energy
    x = sndLevel;
  else
    x = 0;
  end

  % apply randomness to frequencies
  shellF = shellF_int + ((-1 + (1+1) * rand(1)) * shellF_int * per);
  shellF_2 = shellF_2_int + ((-1 + (1+1) * rand(1)) * shellF_2_int * per);
  shellF_3 = shellF_3_int + ((-1 + (1+1) * rand(1)) * shellF_3_int * per);
  
  % apply filter
  a = [1 -2 * shellR * cos( 2 * pi * shellF * T ) shellR^2];
  a_2 = [1 -2 * shellR_2 * cos( 2 * pi * shellF_2 * T ) shellR_2^2];
  a_3 = [1 -2 * shellR_3 * cos( 2 * pi * shellF_3 * T ) shellR_3^2];

  [y(n), z] = filter( b, a, x, z );
  [y_2(n), z_2] = filter( b, a_2, x, z_2 );
  [y_3(n), z_3] = filter( b, a_3, x, z_3 );
  
  sndLevel = sndLevel * soundDecay;
  k = k + 1;
  % change number of k how fast you shake it
  if ( k > 4050 ) k = 1; end   % to check shakeEnergy index
  
end

y_out = y + y_2 + y_3;
ynorm = 0.95 * y_out / max(abs(y_out));
% ynorm_2 = 0.95 * y_2 / max(abs(y_2));
% ynorm_3 = 0.95 * y_3 / max(abs(y_3));
% y_out = ynorm + ynorm_2 + ynorm_3;
sound( ynorm, fs );