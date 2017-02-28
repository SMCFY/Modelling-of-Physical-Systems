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

        x->windex = 0;
        x->ready = false;
    }

    void releaseResources()
    {
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
    {
        if (!x->ready) return;

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

        long windex = x->windex;

        /* Perform the DSP loop */
        float local_freq;
        long rindex1;
        long rindex2;
        int pos;
        for (int ii = 0; ii < n; ++ii) {
            // update effective delaylength
            local_freq = *freq;
            if (local_freq < 1) {
                local_freq = 1;
            }
            delaylength = fs / local_freq;

            // select the plucking position
            pos = int(*position * delaylength/2);
            rindex1 = int(windex - delaylength/2) - pos;
            while (rindex1 < 0) {
                rindex1 += delaylength;
            }

            rindex2 = int(windex - delaylength/2) + pos;
            while (rindex2 < 0) {
                rindex2 += delaylength;
            }

            // write output sample
            *stringL++ += wavetable[rindex1] - wavetable[rindex2];
            *stringR++ += wavetable[rindex1] - wavetable[rindex2];

            // read output of delayline
            delayline[1] = delayline[0];
            delayline[0] = wavetable[windex];

            // read and write lowpass
            lowpass[1] = lowpass[0];
            lowpass[0] = *p_coeff * (*s_coeff * delayline[0] + (1 - *s_coeff) * delayline[1]);

            // read and write allpass
            allpass[1] = allpass[0];
            allpass[0] = - *c_coeff * lowpass[0] + lowpass[1] + *c_coeff * allpass[1];

            // feedback and playback
            wavetable[windex] = allpass[0];
            
            // update windex
            windex++;
            while (windex >= delaylength) {
                windex -= delaylength;
            }
        }
        
        /* Update state variables */
        x->delaylength = delaylength;
        x->windex = windex;
    }

    void pluckString(const int numberOfStrings = 1)
    {
        x->ready = false;
        for (int ii = 0; ii < x->delaylength; ++ii) {
            x->wavetable[ii] = (2.0f * random.nextFloat() - 1.0f) / numberOfStrings;
        }
        x->ready = true;
    }

    //==========================================================================
    float* freq;
    float* s_coeff;
    float* p_coeff;
    float* c_coeff;
    float* position;

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

        long windex;
        bool ready;
    } t_karplus;
    t_karplus *x;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KarplusStrong)
};


#endif  // KARPLUSSTRONG_H_INCLUDED
