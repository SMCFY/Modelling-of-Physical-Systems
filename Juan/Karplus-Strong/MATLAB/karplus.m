function [y, fs] = karplus(freq, dur, S, P, C, fs)
    if nargin < 1
        freq = 300;
    end
    if nargin < 2
        dur = 2;
    end
    if nargin < 3
        S = 0.5; % 0 < S < 1
    end
    if nargin < 4
        P = 0.999; % 0 < S < 1
    end
    if nargin < 5
        C = 0; % abs(C) < 1
    end
    if nargin < 6
        fs = 44100;
    end

    %% allocate output signal and delay lines
    y = zeros(fs * dur, 1);

    delaylength = ceil(fs / freq);
    wavetable = rand(delaylength, 1) - 0.5;

    delayline = zeros(2, 1);
    lowpass = zeros(2, 1);
    allpass = zeros(2, 1);

    %% dsp loop
    rwindex = 1;

    n = length(y);
    for ii = 1 : n
        % verify read/write index
        while rwindex > delaylength
            rwindex = rwindex - delaylength;
        end
        
        % read output of delayline
        delayline(2) = delayline(1);
        delayline(1) = wavetable(rwindex);

        % read and write lowpass
        lowpass(2) = lowpass(1);
        lowpass(1) = P * (S * delayline(1) + (1 - S) * delayline(2));

        % read and write allpass
        allpass(2) = allpass(1);
        allpass(1) = - C * lowpass(1) + lowpass(2) + C * allpass(2);

        % feedback and playback
        wavetable(rwindex) = allpass(1);
        y(ii) = wavetable(rwindex);

        % update rwindex
        rwindex = rwindex + 1;
    end
end
