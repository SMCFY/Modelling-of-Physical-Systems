/*
  ==============================================================================

    MeshSynthesizer.h
    Created: 4 Apr 2017 9:19:33am
    Author:  sd

  ==============================================================================
*/

#ifndef MESHSYNTHESIZER_H_INCLUDED
#define MESHSYNTHESIZER_H_INCLUDED

/**
    A very basic generator of a simulated plucked string sound, implementing
    the Karplus-Strong algorithm.

    Not performance-optimised!
*/
class MeshSynthesizer
{
public:
    //==============================================================================
    /** Constructor.

        @param sampleRate      The audio sample rate to use.
        @param frequencyInHz   The fundamental frequency of the simulated string in
                               Hertz.
    */
    MeshSynthesizer (double sampleRate, double frequencyInHz)
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

        // cycle through the delay line and apply a simple averaging filter
        for (int i = 0; i < numSamples; ++i)
        {
            /// const int nextPos = (pos + 1) % delayLine.size();
            ///
            /// delayLine[nextPos] = (float) (decay * 0.5 * (delayLine[nextPos] + delayLine[pos]));
            /// outBuffer[i] += delayLine[pos];
            ///
            /// pos = nextPos;

            tickcount++;
            int x, y;
            if (tickcount % 2 == 0) { // tick0 - every even iteration
              /// % Update junction velocity
              /// v = 0.5 * (vxp(1:NJ-1,1:NJ-1) + vxm(1:NJ-1,2:NJ) + ...
              ///            vyp(1:NJ-1,1:NJ-1) + vym(2:NJ,1:NJ-1));
              for (x=0; x<NJ-1; x++) {
                for (y=0; y<NJ-1; y++) {
                  v_[x][y] = ( vxp_[x][y] + vxm_[x+1][y] +
                              vyp_[x][y] + vym_[x][y+1] ) * 0.5;
                }
              }
              /// % Update outgoing junction wave components.
              ///
              /// vxp1(1:NJ-1,2:NJ)   = v - vxm(1:NJ-1,2:NJ);
              /// vyp1(2:NJ,1:NJ-1)   = v - vym(2:NJ,1:NJ-1);
              /// vxm1(1:NJ-1,1:NJ-1) = v - vxp(1:NJ-1,1:NJ-1);
              /// vym1(1:NJ-1,1:NJ-1) = v - vyp(1:NJ-1,1:NJ-1);
              for (x=0; x<NJ-1; x++) {
                for (y=0; y<NJ-1; y++) {
                  float vxy = v_[x][y];
                  // Update positive-going waves.
                  vxp1_[x+1][y] = vxy - vxm_[x+1][y];
                  vyp1_[x][y+1] = vxy - vym_[x][y+1];
                  // Update minus-going waves.
                  vxm1_[x][y] = vxy - vxp_[x][y];
                  vym1_[x][y] = vxy - vyp_[x][y];
                }
              }
              /// % Compute nodes next to boundaries (reflection).
              /// % west
              /// vxp1(1:NJ-1,1)  = decayFactor * filter( num, denom,   vxm(1:NJ-1,1));
              /// % east
              /// vxm1(1:NJ-1,NJ) = decayFactor * filter( num, denom,   vxp(1:NJ-1,NJ));
              /// % north
              /// vyp1(1,1:NJ-1)  = decayFactor * filter( num, denom,   vym(1,1:NJ-1));
              /// % south
              /// vym1(NJ,1:NJ-1) = decayFactor * filter( num, denom,   vyp(NJ,1:NJ-1));
              // here just decayfactor, no filter
              for (y=0; y<NJ-1; y++) {
                vxp1_[0][y] = decayFactor* vxm_[0][y];
                vxm1_[NJ-1][y] = decayFactor* vxp_[NJ-1][y];
              }
              for (x=0; x<NJ-1; x++) {
                vyp1_[x][0] = decayFactor*vym_[x][0];
                vym1_[x][NJ-1] = decayFactor*vyp_[x][NJ-1];
              }
              ///% Compute perfect reflection - skipping
              // get output sample (sum of outgoing waves at a x,y position on grid); take the corner here
              // should be vxp_ and vyp_ in two-pass algo; in one-pass use the *1_ to at least hear something
              //~ outBuffer[i] = vxp_[NJ-1][NJ-1] + vyp_[NJ-1][NJ-1];
              outBuffer[i] = vxp_[NJ/2][NJ/2] + vyp_[NJ/2][NJ/2];
              //~ DBG( outBuffer[i] );
            } // end if tick0
            else { // tick1
              // Update junction velocities.
              for (x=0; x<NJ-1; x++) {
                for (y=0; y<NJ-1; y++) {
                  v_[x][y] = ( vxp1_[x][y] + vxm1_[x+1][y] +
                              vyp1_[x][y] + vym1_[x][y+1] ) * 0.5;
                }
              }
              // Update junction outgoing waves,
              // using alternate wave-variable buffers.
              for (x=0; x<NJ-1; x++) {
                for (y=0; y<NJ-1; y++) {
                  float vxy = v_[x][y];

                  // Update positive-going waves.
                  vxp_[x+1][y] = vxy - vxm1_[x+1][y];
                  vyp_[x][y+1] = vxy - vym1_[x][y+1];

                  // Update minus-going waves.
                  vxm_[x][y] = vxy - vxp1_[x][y];
                  vym_[x][y] = vxy - vyp1_[x][y];
                }
              }
              // Loop over velocity-junction boundary faces, update edge
              // reflections, with filtering.  We're only filtering on one x and y
              // edge here and even this could be made much sparser.
              // no filter here - just decayFactor*
              for (y=0; y<NJ-1; y++) {
                vxp_[0][y] = decayFactor* vxm1_[0][y];
                vxm_[NJ-1][y] = vxp1_[NJ-1][y];
              }
              for (x=0; x<NJ-1; x++) {
                vyp_[x][0] = decayFactor*vym1_[x][0];
                vym_[x][NJ-1] = vyp1_[x][NJ-1];
              }
              // Output = sum of outgoing waves at far corner.
              //~ outBuffer[i] = vxp1_[NJ-1][NJ-1] + vyp1_[NJ-1][NJ-1];
              outBuffer[i] = amp*(vxp1_[NJ/2][NJ/2] + vyp1_[NJ/2][NJ/2]);
            } // end if tick1
            // set debug image:
            float tval; for (x=0; x<meshStateImage.getWidth(); x++) {
              for (y=0; y<meshStateImage.getHeight(); y++) {
                tval = (vxp1_[x][y] + vyp1_[x][y]); //;
                meshStateImage.setPixelAt(x,y, Colour::fromFloatRGBA( ((tval < 0) ? -tval : 0.0f)*amp, ((tval >= 0) ? tval : 0.0f)*amp, 0.0f, 1.0f));
                meshStateImage2.setPixelAt(x,y, Colour::fromFloatRGBA( ((v_[x][y] < 0) ? -v_[x][y] : 0.0f)*amp, ((v_[x][y] >= 0) ? v_[x][y] : 0.0f)*amp, 0.0f, 1.0f));
              }
            }
            //~ DBG( outBuffer[i] );
        }
    }

    void updateMeshSizeNJ(int inNJ)
    {
      NJ = inNJ;
      // .resize(no_of_rows, std::vector<int>(no_of_cols, initial_value));
      v_.resize(NJ-1, std::vector<float>(NJ-1, 0.0f));

      vxp_.resize(NJ, std::vector<float>(NJ, 0.0f));
      vxm_.resize(NJ, std::vector<float>(NJ, 0.0f));
      vyp_.resize(NJ, std::vector<float>(NJ, 0.0f));
      vym_.resize(NJ, std::vector<float>(NJ, 0.0f));

      vxp1_.resize(NJ, std::vector<float>(NJ, 0.0f));
      vxm1_.resize(NJ, std::vector<float>(NJ, 0.0f));
      vyp1_.resize(NJ, std::vector<float>(NJ, 0.0f));
      vym1_.resize(NJ, std::vector<float>(NJ, 0.0f));

      meshStateImage = Image(Image::RGB, NJ-1, NJ-1, true); // for v_
      meshStateImage2 = Image(Image::RGB, NJ-1, NJ-1, true); // for v_
    }

    Image meshStateImage, meshStateImage2;
    float amp = 1.0f;

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

        /// excitationSample.resize (delayLineLength);
        ///
        /// // as the excitation sample we use random noise between -1 and 1
        /// // (as a simple approximation to a plucking excitation)
        ///
        /// std::generate (excitationSample.begin(),
        ///                excitationSample.end(),
        ///                [] { return (Random::getSystemRandom().nextFloat() * 2.0f) - 1.0f; } );

        excitationSample.resize(3);
        excitationSample[0] = 0.5f; excitationSample[1] = 1.0f; excitationSample[2] = 0.5f;

        // default resize (NJ by default is 2, as the slider):
        updateMeshSizeNJ(NJ);
    }

    void exciteInternalBuffer()
    {
        // fill the buffer with the precomputed excitation sound (scaled with amplitude)

        jassert (delayLine.size() >= excitationSample.size());

        std::transform (excitationSample.begin(),
                        excitationSample.end(),
                        delayLine.begin(),
                        [this] (double sample) { return amplitude * sample; } );

        ///% fill the alternating matrix with transposed values
        ///% note: updates the alternate matrix, as it is the matrix used for the
        ///% first step
        ///
        ///vxp1(1:NJ-1,1:NJ-1) = valm;
        ///vyp1(1:NJ-1,1:NJ-1) = valm;
        ///vxm1(1:NJ-1,2:NJ) = valm;
        ///vym1(2:NJ,1:NJ-1) = valm;

        // just excite the middle three elements of each 2D array; use direct indices, transform can get too complicated with iterators
        // however, this can cause segfault for changes of array size:
        int midind = NJ/2;
        /// int valstofill = (midind >= 3) ? 3 : midind;
        /// for (int ir=0; ir<valstofill; ir++) {
        ///   for (int ic=0; ic<valstofill; ic++) {
        ///     vxp1_[midind+ir-1][midind+ic-1] = excitationSample[ic];
        ///     vxm1_[midind+ir-1][midind+ic-1] = excitationSample[ic];
        ///     vyp1_[midind+ir-1][midind+ic-1] = excitationSample[ic];
        ///     vym1_[midind+ir-1][midind+ic-1] = excitationSample[ic];
        ///   }
        /// }
        // so try a single sample excitation in middle:
        vxp1_[midind][midind] = 1.0f;
        vxm1_[midind][midind] = 1.0f;
        vyp1_[midind][midind] = 1.0f;
        vym1_[midind][midind] = 1.0f;
    };

    //==============================================================================
    const double decay = 0.998;
    double amplitude = 0.0;

    Atomic<int> doPluckForNextBuffer;

    std::vector<float> excitationSample, delayLine;
    std::vector< std::vector<float> > v_;    // [NXMAX-1][NYMAX-1]; // junction velocities

    std::vector< std::vector<float> > vxp_;  // [NXMAX][NYMAX];   // positive-x velocity wave
    std::vector< std::vector<float> > vxm_;  // [NXMAX][NYMAX];   // negative-x velocity wave
    std::vector< std::vector<float> > vyp_;  // [NXMAX][NYMAX];   // positive-y velocity wave
    std::vector< std::vector<float> > vym_;  // [NXMAX][NYMAX];   // negative-y velocity wave

    // Alternate buffer
    std::vector< std::vector<float> > vxp1_;  // [NXMAX][NYMAX];   // positive-x velocity wave
    std::vector< std::vector<float> > vxm1_;  // [NXMAX][NYMAX];   // negative-x velocity wave
    std::vector< std::vector<float> > vyp1_;  // [NXMAX][NYMAX];   // positive-y velocity wave
    std::vector< std::vector<float> > vym1_;  // [NXMAX][NYMAX];   // negative-y velocity wave

    // NB: the y = zeros(1,N=44100); in matlab will be outBuffer in generateAndAddData!

    int pos = 0;
    int NJ = 32;
    float decayFactor = 0.4f;
    int tickcount = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeshSynthesizer)
};




#endif  // MESHSYNTHESIZER_H_INCLUDED
