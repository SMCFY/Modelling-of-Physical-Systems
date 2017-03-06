
/**
    A very basic generator of a simulated plucked string sound, implementing
    the Karplus-Strong algorithm.

    Not performance-optimised!
*/
#include <math.h>

class StringSynthesiser
{
public:
    //==============================================================================
    /** Constructor.

        @param sampleRate      The audio sample rate to use.
        @param frequencyInHz   The fundamental frequency of the simulated string in
                               Hertz.
    */
    StringSynthesiser (double sampleRate, double frequencyInHz)
    {
        fs = sampleRate;
        doPluckForNextBuffer.set (false);
        prepareSynthesiserState (sampleRate, frequencyInHz);
    }

    //==============================================================================
    /** Excite the simulated string by plucking it at a given position.

        @param pluckPosition The position of the plucking, relative to the length
                             of the string. Must be between 0 and 1.
    */
    void stringPlucked (float pluckPosition)
    {
        jassert (pluckPosition >= 0.0 && pluckPosition <= 1.0);

        // we choose a very simple approach to communicate with the audio thread:
        // simply tell the synth to perform the plucking excitation at the beginning
        // of the next buffer (= when generateAndAddData is called the next time).

        if (doPluckForNextBuffer.compareAndSetBool (1, 0))
        {
            // plucking in the middle gives the largest amplitude;
            // plucking at the very ends will do nothing.
            amplitude = std::sin (float_Pi * pluckPosition);
        }
    }

    //==============================================================================
    /** Generate next chunk of mono audio output and add it into a buffer.

        @param outBuffer  Buffer to fill (one channel only). New sound will be
                          added to existing content of the buffer (instead of
                          replacing it).
        @param numSamples Number of samples to generate (make sure that outBuffer
                          has enough space).
    */
    void generateAndAddData (float* outBuffer, int numSamples)
    {
        
        if (doPluckForNextBuffer.compareAndSetBool (0, 1))
            exciteInternalBuffer();
        
        //std::cout << amplitude << "\n";
        
        float delta1 = freq1 * 2 * float_Pi / fs;
        float delta2 = freq2 * 2 * float_Pi / fs;
        float delta3 = freq3 * 2 * float_Pi / fs;
        float delta4 = freq4 * 2 * float_Pi / fs;
        
        float a21 = pow(r1,2);
        float a22 = pow(r2,2);
        float a23 = pow(r3,2);
        float a24 = pow(r4,2);
        
        float a11 = -2*r1*cos(phase1);
        float a12 = -2*r2*cos(phase2);
        float a13 = -2*r3*cos(phase3);
        float a14 = -2*r4*cos(phase4);
        
        // cycle through the delay line and apply a simple averaging filter + allpass
        for (int i = 0; i < numSamples; ++i)
        {
            const int nextPos = (pos + 1) % delayLine.size();
            
            m = amplitude*0.5;
            
            int pluck = int(m * delayLine.size()/2);
            int tap1 = ((pos - delayLine.size()/2) - pluck);
            int tap2 = ((pos - delayLine.size()/2) + pluck);
            
            if(tap1 < 0)
                tap1 += delayLine.size();
            if(tap2 < 0)
                tap2 += delayLine.size();
            // Lowpass
            delayLine[nextPos] = (float) (decay * 0.5 * (delayLine[nextPos] + delayLine[pos]));
            // Allpass
            APdelay[1] = -g*delayLine[nextPos] + APdelay[0] + g*APdelay[1];
            APdelay[0] = delayLine[nextPos];
            
            delayLine[pos] = APdelay[1];
            
            float sample = delayLine[tap1]+(-delayLine[tap2]);
            
            if(sample*d >= 1)
            {
                sample = 2.0/3.0;
            }
            else if(sample*d < 1 && sample*d > -1)
            {
                sample = sample - pow(sample,3.0)/3.0;
            }
            else if(sample*d <= -1)
            {
                sample = -(2.0/3.0);
            }
            // obj.AP1(1) = a21 * x(i,1) + a11 * obj.xn1 + obj.xn2 - a11 * obj.AP1(2) - a21 * obj.AP1(3);

            // Add phasor
            APphasor1[0] = a21 * sample + a11 * xn1 + xn2 - a11 * APphasor1[1] - a21 * APphasor1[2];
            APphasor2[0] = a22 * APphasor1[0] + a12 * APphasor1[1] + APphasor1[2] - a12 * APphasor2[1] - a22 * APphasor2[2];
            APphasor3[0] = a23 * APphasor2[0] + a13 * APphasor2[1] + APphasor2[2] - a13 * APphasor3[1] - a23 * APphasor3[2];
            APphasor4[0] = a24 * APphasor3[0] + a14 * APphasor3[1] + APphasor3[2] - a14 * APphasor4[1] - a24 * APphasor4[2];

            // Delay line stuff
            xn2 = xn1;
            xn1 = sample;
            
            APphasor1[2] = APphasor1[1];
            APphasor1[1] = APphasor1[0];
            
            APphasor2[2] = APphasor2[1];
            APphasor2[1] = APphasor2[0];
            
            APphasor3[2] = APphasor3[1];
            APphasor3[1] = APphasor3[0];
            
            APphasor4[2] = APphasor4[1];
            APphasor4[1] = APphasor4[0];
            
            // increment phase
            phase1 += delta1;
            if(phase1 > 2*float_Pi)
                phase1 -= 2*float_Pi;
            
            phase2 += delta2;
            if(phase2 > 2*float_Pi)
                phase2 -= 2*float_Pi;

            phase3 += delta3;
            if(phase3 > 2*float_Pi)
                phase3 -= 2*float_Pi;

            phase4 += delta4;
            if(phase4 > 2*float_Pi)
                phase4 -= 2*float_Pi;

            
            // Calculate new coefficient
            a11 = -2*r1*cos(phase1);
            a12 = -2*r1*cos(phase2);
            a13 = -2*r1*cos(phase3);
            a14 = -2*r1*cos(phase4);
            
            //outBuffer[i] += delayLine[pos]; // normal karplus
            //outBuffer[i] += sample; // distortion
            outBuffer[i] += 0.5*sample+0.5*APphasor4[0]; // with phasor
            //outBuffer[i] += delayLine[tap1]+(-delayLine[tap2]) // pluck pos;
            pos = nextPos;
        }
    }

private:
    //==============================================================================
    void prepareSynthesiserState (double sampleRate, double frequencyInHz)
    {
        size_t delayLineLength = (size_t) roundToInt (sampleRate / frequencyInHz);

        // we need a minimum delay line length to get a reasonable synthesis.
        // if you hit this assert, increase sample rate or decrease frequency!
        jassert (delayLineLength > 50);

        delayLine.resize (delayLineLength);
        std::fill (delayLine.begin(), delayLine.end(), 0.0f);
        
        excitationSample.resize (delayLineLength);

        // as the excitation sample we use random noise between -1 and 1
        // (as a simple approximation to a plucking excitation)

        std::generate (excitationSample.begin(),
                       excitationSample.end(),
                       [] { return (Random::getSystemRandom().nextFloat() * 2.0f) - 1.0f; } );
    }

    void exciteInternalBuffer()
    {
        // fill the buffer with the precomputed excitation sound (scaled with amplitude)

        jassert (delayLine.size() >= excitationSample.size());

        std::transform (excitationSample.begin(),
                        excitationSample.end(),
                        delayLine.begin(),
                        [this] (double sample) { return amplitude * sample; } );
    };

    //==============================================================================
    const double decay = 0.998;
    double amplitude = 0.0;
    
    // Allpass
    const double g = 0.3;
    float APdelay[2] = {0,0};
    
    float m = 0.5;
    int d = 1;
    
    // Analog phaser
    float APphasor1[3] = {0,0,0};
    float APphasor2[3] = {0,0,0};
    float APphasor3[3] = {0,0,0};
    float APphasor4[3] = {0,0,0};
    
    float xn1 = 0;
    float xn2 = 0;
    
    float phase1 = 0;
    float phase2 = 0;
    float phase3 = 0;
    float phase4 = 0;
    
    float r1 = 0.8;
    float r2 = 0.8;
    float r3 = 0.8;
    float r4 = 0.8;
    
    float freq1 = 0.5;
    float freq2 = 2;
    float freq3 = 3;
    float freq4 = 5;
    
    int fs = 44100;
    
    Atomic<int> doPluckForNextBuffer;

    std::vector<float> excitationSample, delayLine;
    int pos = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StringSynthesiser)
};
