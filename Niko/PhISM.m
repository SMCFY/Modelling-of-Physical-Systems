

%%
% maracas.m
% Collisions are determined by a Poisson process.
% sound envelope is additively increased by each collision
% number of particles
% size of particles
% energy of excitation (shakeEnergy)

% SYNTHESIS TOOLKIT



fs = 44100;
T = 1 / fs;
N = 1 * fs;
nBeans = 32;
shellF1 = 2500.0;   % frequency in Hz
shellF2 = 4000.0;   % frequency in Hz

shellR = 0.96;
systemDecay = 0.999;
soundDecay = 0.95;
y1 = zeros(1, N);
y2 = zeros(1, N);
nShakes = 8;

Ns = 0.5 * fs;

shakeEnergy = 1.0 - cos(2*pi*[0:Ns-1]/Ns);


shakeEnergy = [ shakeEnergy 0 ];

gain = log( nBeans ) / (log( 4.0 ) * 40.0 * nBeans);

b1 = [1 -1];

a1 = [1, -2 * shellR * cos( 2 * pi * shellF1 * T ), shellR^2];

b2 = [1 -1];

a2 = [1, -2 * shellR * cos( 2 * pi * shellF2 * T ), shellR^2];

sndLevel = 0;
z1 = [0 0];
z2 = [0 0];
k = 1;
skill = 2000/nShakes;
rFactor = floor(-skill + (skill+skill)*rand(1));

for n = 1:N
    
    e = shakeEnergy( k ) * systemDecay;
    if ( rand(1) * 1024 < nBeans )     % if collision
        sndLevel = sndLevel + (gain * e);     % add energy
        x = sndLevel;
    else
        x = 0;
    end
    
    [y1(n), z1] = filter( b1, a1, x, z1 );
    [y2(n), z2] = filter( b2, a2, x, z2 );
    
    sndLevel = sndLevel * soundDecay;
    k = k + 1;
    
    if ( k > fs/nShakes+rFactor || k > Ns)
        rFactor = floor(-skill + (skill+skill)*rand(1));
        k = 1;
    end   % to check shakeEnergy index
    %
    %   if ( k > fs/nShakes+rFactor || k > Ns)
    %       rFactor = floor(-skill + (skill+skill)*rand(1));
    %       k = 1;
    %   end   % to check shakeEnergy index
    %
end

mix = y1+ y2;
ynorm = 0.95 * mix / max(abs(mix));

sound( ynorm, fs );

%%
% Guiro 2 modes
% Collisions are determined by a Poisson process.
% sound envelope is additively increased by each collision
% number of particles
% size of particles
% energy of excitation (shakeEnergy)

% SYNTHESIS TOOLKIT



fs = 44100;
T = 1 / fs;
N = 1 * fs;
nBeans = 10;

shellF1 = 2500.0;   % frequency in Hz
shellF2 = 4000.0;   % frequency in Hz

shellR = 0.97;

systemDecay = 1;%0.999;
soundDecay = 0.95;

y1 = zeros(1, N);
y2 = zeros(1, N);

nShakes = 1;

Ns = 0.5 * fs;

shakeEnergy = 1.0 - cos(2*pi*[0:Ns-1]/Ns);

prob = 1024;
shakeEnergy = [ shakeEnergy 0 ];

gain = log( nBeans ) / (log( 4.0 ) * 40.0 * nBeans);

b1 = [1 -1];

a1 = [1, -2 * shellR * cos( 2 * pi * shellF1 * T ), shellR^2];

b2 = [1 -1];

a2 = [1, -2 * shellR * cos( 2 * pi * shellF2 * T ), shellR^2];

sndLevel = 0;
z1 = [0 0];
z2 = [0 0];
k = 1;
skill = 2000/nShakes;
rFactor = floor(-skill + (skill+skill)*rand(1));

for n = 1:N
    
    e = shakeEnergy( k ) * systemDecay;
    if ( rand(1) < 1/8 )     % if collision
        sndLevel = sndLevel + (gain * e);     % add energy
        x = sndLevel;
    else
        x = 0;
    end
    
    [y1(n), z1] = filter( b1, a1, x, z1 );
    [y2(n), z2] = filter( b2, a2, x, z2 );
    
    sndLevel = sndLevel * soundDecay;
    k = k + 1;
    
    if ( k > Ns)
        %rFactor = floor(-skill + (skill+skill)*rand(1));
        k = 1;
    end   % to check shakeEnergy index
end

mix = y1+ y2;
ynorm = 0.95 * mix / max(abs(mix));

sound( ynorm, fs );

%%
% Tambourine
% Collisions are determined by a Poisson process.
% sound envelope is additively increased by each collision
% number of particles
% size of particles
% energy of excitation (shakeEnergy)

% SYNTHESIS TOOLKIT



fs = 44100;
T = 1 / fs;
N = 1 * fs;
nBeans = 32;

shellF1 = 2300.0;   % frequency in Hz
shellF2 = 5600.0;   % frequency in Hz
shellF3 = 8100.0;   % frequency in Hz

shellR1 = 0.96;
shellR2 = 0.995;

systemDecay = .9985;
soundDecay = 0.95;

y1 = zeros(1, N);
y2 = zeros(1, N);
y3 = zeros(1, N);

nShakes = 8;

Ns = 0.5 * fs;

shakeEnergy = 1.0 - cos(2*pi*[0:Ns-1]/Ns);

prob = 1024;
shakeEnergy = [ shakeEnergy 0 ];

gain = log( nBeans ) / (log( 4.0 ) * 40.0 * nBeans);

b1 = [1 -1];

a1 = [1, -2 * shellR1 * cos( 2 * pi * shellF1 * T ), shellR1^2];

b2 = [1 -1];

a2 = [1, -2 * shellR2 * cos( 2 * pi * shellF2 * T ), shellR2^2];

b3 = [1 -1];

a3 = [1, -2 * shellR2 * cos( 2 * pi * shellF3 * T ), shellR2^2];


sndLevel = 0;
z1 = [0 0];
z2 = [0 0];
z3 = [0 0];
k = 1;
skill = 2000/nShakes;
rFactor = floor(-skill + (skill+skill)*rand(1));
v = 500;

for n = 1:N
    
    e = shakeEnergy( k ) * systemDecay;
    if ( rand(1) < 1/32 )     % if collision
        sndLevel = sndLevel + (gain * e);     % add energy
        x = sndLevel;
    else
        x = 0;
    end
    
    [y1(n), z1] = filter( b1, a1, x, z1 );
    [y2(n), z2] = filter( b2, a2, x, z2 );
    [y3(n), z3] = filter( b3, a3, x, z3 );
    
    sndLevel = sndLevel * soundDecay;
    k = k + 1;
    shellF1 = 2300.0 + (-v + (v+v)*rand(1));
    shellF2 = 5600.0 + (-v + (v+v)*rand(1));
    shellF3 = 8100.0 + (-v + (v+v)*rand(1));

    a1 = [1, -2 * shellR1 * cos( 2 * pi * shellF1 * T ), shellR1^2];
    
    a2 = [1, -2 * shellR2 * cos( 2 * pi * shellF2 * T ), shellR2^2];
    
    a3 = [1, -2 * shellR2 * cos( 2 * pi * shellF3 * T ), shellR2^2];
    
    if ( k > Ns/3)
        rFactor = floor(-skill + (skill+skill)*rand(1));
        k = 1;
    end   % to check shakeEnergy index
end

mix = y1+ y2 + y3;
ynorm = 0.95 * mix / max(abs(mix));

sound( ynorm, fs );