
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
            //outBuffer[i] += delayLine[pos]; // normal karplus
            outBuffer[i] += sample; // distortion
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
    int d = 2;
    
    Atomic<int> doPluckForNextBuffer;

    std::vector<float> excitationSample, delayLine;
    int pos = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StringSynthesiser)
};
