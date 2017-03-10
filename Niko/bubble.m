fs = 44100;
N = 2 * fs;
dt = 1/fs;
stoptime = 0.5 + (-0.2 + (0.2+0.2)*rand(1));
t = (0:dt:stoptime);
a = 1;
r = 0.005 + (-0.001 + (0.001+0.001)*rand(1));
f0 = 3/r;
d = 0.043*f0+0.0014*f0^(3/2);
eps = 0.1;
sigma = eps*d;
ft =  f0*(1+sigma*t);
x = a*sin(2*pi*ft.*t).*exp(-d*t);

sound(x,fs)

y = [];
amount = 1;
for i = 1:amount
    y = [y, x];
    rnd = rand(1);
    stoptime = 0.5 + (-0.2 + (0.2+0.2)*rnd);
    t = (0:dt:stoptime);
    r = 0.005 + (-0.001 + (0.001+0.001)*rnd);
    f0 = 3/r;
    d = 0.043*f0+0.0014*f0^(3/2);
    eps = 0.1;
    a = 1-rnd;
    sigma = eps*d;
    ft =  f0*(1+sigma*t);
    x = a*sin(2*pi*ft.*t).*exp(-d*t);
end

sound(y,fs)

%% Waterfall

fs = 44100;
N = 2 * fs;
dt = 1/fs;
stoptime = 0.5 + (-0.2 + (0.2+0.2)*rand(1));
t = (0:dt:stoptime);
a = 1;
r = 0.005 + (-0.001 + (0.001+0.001)*rand(1));
f0 = 3/r;
d = 0.043*f0+0.0014*f0^(3/2);
eps = 0.1;
sigma = eps*d;
ft =  f0*(1+sigma*t);
x = a*sin(2*pi*ft.*t).*exp(-d*t);

%sound(x,fs)


w = zeros(1,N*2);
amount = 2;
y = zeros(amount,N*2);
z = zeros(1,N*2);
for k=1:10
    numero=rand(1);
    for i = 1:amount
        stepsize = floor(rand(1) * 50000 + 1);
        
        for j = 1:stepsize:N/2
            if(rand(1) < 1/2)
                rnd = rand(1);
                stoptime = 0.5 + (-0.2 + (0.2+0.2)*rnd);
                t = (0:dt:stoptime);
                l = length(t);
                r = 0.005 + (-0.001 + (0.001+0.001)*rnd);
                f0 = 3/r;
                d = 0.043*f0+0.0014*f0^(3/2);
                eps = 0.1;
                a = 1-rnd;
                sigma = eps*d;
                ft =  f0*(1+sigma*t);
                x = a*sin(2*pi*ft.*t).*exp(-d*t);
                z(1,j:l+j-1) = z(1,j:l+j-1) + x;
            end
        end
        % z = 0.95 * z / max(z);
        y(i,:) = z;
    end
    
    w=w+sum(y);
end
w = real(.95 * w /max(w));
sound(w,fs)



%sound(y,fs)


