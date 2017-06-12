function y = nonLinString (f0, d, g, a1, a2)
	%ARGUMENTS:
	%	f0 - fundamental frequency
	%	d - decay rate
	%	g - inharmonicity
	%	a1 - nonlinearity coefficient
	% 	a2 - nonlinearity coefficient

	if nargin < 1
    	f0 = 440;
  	end
  	if nargin < 2
    	d = 0.7;
  	end
  	if nargin < 3
    	g = 0.1;
  	end
   	if nargin < 4
    	a1 = 0.01;
  	end
  	if nargin < 5
    	a2 = 0.99;
  	end



	% delay line [W______R]
	fs = 44100;
	%f0 = 440;
	L = floor(fs/f0);
	del = zeros(L+1,1); % z^-L
    
	% I/O
	t = 1;
	x = zeros(t*fs,1); x(1:500) = rand(500,1); x(1:500) = x(1:500)-mean(x(1:500)); %input
	y = zeros(t*fs,1); %output
    pmSin = sin(2*pi*60*[0:1/fs:1])';
	
	%filters (buffers and coefficients)
	nonL = [0 0]; %non-linearity filter(APF) 	y[n] = a0x[n]+x[n-1]-a0y[n-1]	
    k = [a1, a2]; %stiffness coefficients
	a0 = k(1); 
	w = 0; %internal state signal buffer
	%f = 0; %force at the spring termination

	inhar = [0 0]; %dispersion filter(APF) 		y[n] = -gx[n]+x[n-1]+gy[n-1]
	%g = 0.1; %inharmonicity

	loss = [0 0]; %loss filter(LPF) 			y[n] = (1-d)x[n]+d*y[n-1]
	%d = 0.7; %decay rate
	
    
	for i=1:length(y)
        %a0 = pmSin(i);        

		del(2:L+1) = del(1:L); %shifting values from left to right
		del(1) = x(i)+loss(1); %writing to the delay line
	
		nonL(2) = nonL(1); % shifting the values in the buffer
		nonL(1) = a0*del(L)+del(L+1)-a0*nonL(2); %reading the delay lines last sample(x[n]) + its past sample(x[n-1])

        %coefficient modulation
		w(1) = del(L)-a0*nonL(1);
		%f = (1/(1-a0))*(w(1)+w(2));
	    if sign(pmSin(i)) == 1 %change the filter coefficient on sign change
	        a0 = k(1);
	    elseif sign(pmSin(i)) == -1
	        a0 = k(2);
	    end
	   
		inhar(2) = inhar(1);
		inhar(1) = -g*nonL(1)+nonL(2)+g*inhar(2); 
	
		loss(2) = loss(1);
		loss(1) = (1-d)*inhar(1)+d*loss(2);
	
	    y(i) = loss(1); %output (sampled at the spring termination)

	end
	
	y = y/max(y);
	sound(y, fs);
end