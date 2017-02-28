/*
  ==============================================================================

    KarplusStrong.h
    Created: 19 Feb 2017 9:01:30pm
    Author:  Juan Gil

  ==============================================================================
*/

#ifndef KARPLUSSTRONG_H_INCLUDED
#define KARPLUSSTRONG_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class KarplusStrong :
    public Component
{
public:
    KarplusStrong()
    {
        x = new t_karplus;
    }

    ~KarplusStrong()
    {
        /* Free allocated dynamic memory */
        if (x->wavetable != NULL) {
            free(x->wavetable);
        }
        delete (x);
    }

    //==========================================================================
    void prepareToPlay (int samplesPerBlockExpected,
                        double sampleRate)
    {
        /* Initialize state variables */
        x->fs = sampleRate;

        x->delaylength = ceil(x->fs);

        long bytesize = x->delaylength * sizeof(float);
        if (x->wavetable == NULL) {
            x->wavetable = (float *)malloc(bytesize);
        } else {
            x->wavetable = (float *)realloc(x->wavetable, bytesize);
        }
        for (int ii = 0; ii < x->delaylength; ++ii) {
            x->wavetable[ii] = 0.0;
        }

        x->delayline[0] = 0;
        x->delayline[1] = 0;

        x->lowpass[0] = 0;
        x->lowpass[1] = 0;
        
        x->allpass[0] = 0;
        x->allpass[1] = 0;
        
        x->rwindex = 0;
    }

    void releaseResources()
    {
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
    {
        /* Copy signal pointers */
        float* stringL = bufferToFill.buffer->getWritePointer(0);
        float* stringR = bufferToFill.buffer->getWritePointer(1);

        /* Copy the signal vector size */
        int n = bufferToFill.numSamples;

        /* Load state variables */
        float fs = x->fs;
        long delaylength = x->delaylength;

        float* wavetable = x->wavetable;

        float* delayline = x->delayline;
        float* lowpass = x->lowpass;
        float* allpass = x->allpass;

        long rwindex = x->rwindex;

        /* Perform the DSP loop */
        float local_freq;
        for (int ii = 0; ii < n; ++ii) {
            // update effective delaylength
            local_freq = *freq;
            if (local_freq < 1) {
                local_freq = 1;
            }
            delaylength = fs / local_freq;

            // verify read/write index
            while (rwindex >= delaylength) {
                rwindex -= delaylength;
            }

            // read output of delayline
            delayline[1] = delayline[0];
            delayline[0] = wavetable[rwindex];

            // read and write lowpass
            lowpass[1] = lowpass[0];
            lowpass[0] = *p_coeff * (*s_coeff * delayline[0] + (1 - *s_coeff) * delayline[1]);

            // read and write allpass
            allpass[1] = allpass[0];
            allpass[0] = - *c_coeff * lowpass[0] + lowpass[1] + *c_coeff * allpass[1];

            // feedback and playback
            wavetable[rwindex] = allpass[0];
            *stringL++ += wavetable[rwindex];
            *stringR++ += wavetable[rwindex];
            
            // update rwindex
            rwindex++;
        }
        
        /* Update state variables */
        x->delaylength = delaylength;
        x->rwindex = rwindex;
    }

    void pluckString(const int numberOfStrings = 1)
    {
        for (int ii = 0; ii < x->delaylength; ++ii) {
            x->wavetable[ii] = (2 * random.nextFloat() - 1) / numberOfStrings;
        }
    }

    //==========================================================================
    float* freq;
    float* s_coeff;
    float* p_coeff;
    float* c_coeff;

private:
    Random random;

    typedef struct _karplus {
        float fs;
        float freq;

        long delaylength;
        float* wavetable;

        float delayline[2];
        float lowpass[2];
        float allpass[2];
        
        long rwindex;
    } t_karplus;
    t_karplus *x;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KarplusStrong)
};


#endif  // KARPLUSSTRONG_H_INCLUDED
