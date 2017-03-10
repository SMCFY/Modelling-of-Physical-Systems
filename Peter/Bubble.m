clearvars
close all
clc

%% Bubble Sound and Waterfall

% Bubble population
N = 50; % number of bubbles

% maximum and minimum number of radii of bubbles
rmin = 0.2 * 10^(-3);
rmax = 50 * 10^(-3);

% total bubble creation rate
Lambda = 500;

% exponent for bubble creation rate
gamma = 2.5;

% predifinings for loop
r = zeros(1, N);
rel_lambda = zeros(1,N);
lambda_k = zeros(1,N);
t_k = zeros(1,N);
gain = zeros(1,N);

% depth factor
beta = 10;
D = (rand(1))^beta;

% create feeling of distance
alpha = 2;


for a = 1:N
    % create radii vector
    r(a) = rmin * ((rmax/rmin)^((a - 1)/(N-1)));
    % relative bubble creation rate
    rel_lambda(a) = 1/((r(a))^gamma);
    % bubble creation rate for each bubble
    lambda_k(a) = (Lambda * rel_lambda(a))/((sum(rel_lambda)));
    % expected time intervals
    t_k(a) = 1/(lambda_k(a));
    % gain factor
    gain(a) = D * (r(a)^alpha);
end

% for bubble creation
eps = 0.05;

% sampling frequency and length of each bubble
fs = 44100;
dt = 1/fs;
stoptime = 1;
t = (0:dt:stoptime);

% time vector for sound output
time = 8;

% predfine output vector for loop
output = zeros(1,(time + 1) * fs);


% create time vector
for a = 1:(time * fs)
    
    % check for every sample in time vector if it fits to time interval of
    % bubble creation
    for b = 1:N
        if mod(a, floor(t_k(b) * fs)) == 0
            
            % compute the bubble
            ampl = gain(b);
            rad = r(b);
            f0 = 3/rad;
            d = 0.043 * f0 + 0.0014 * (f0^(3/2)); % decay     
            sigma = eps * d;
            ft = f0 * (1 + sigma * t); % variations of frequency
            y = ampl * sin(2 * pi * ft .* t) .* exp(-d * t); % output bubble
            
            % add bubble to overall output at current time instance
            output(a:a + length(y) - 1) = output(a:a + length(y) - 1) + y;
        end
    end
end

% sound output and plot it
soundsc(output, fs)
figure
plot(output)