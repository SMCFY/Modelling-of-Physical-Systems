/* Common header files ********************************************************/
#include "karplus~common.h"

/* The common 'new instance' routine ******************************************/
void *common_new(t_karplus *x, short argc, t_atom *argv)
{
#ifdef TARGET_IS_MAX
    /* Create inlets */
    dsp_setup((t_pxobject *)x, NUM_INLETS);

    /* Create signal outlets */
    outlet_new((t_object *)x, "signal");

    /* Avoid sharing memory among audio vectors */
    x->obj.z_misc |= Z_NO_INPLACE;

#elif TARGET_IS_PD
    /* Create inlets */
    inlet_new(&x->obj, &x->obj.ob_pd, gensym("signal"), gensym("signal"));
    inlet_new(&x->obj, &x->obj.ob_pd, gensym("signal"), gensym("signal"));
    inlet_new(&x->obj, &x->obj.ob_pd, gensym("signal"), gensym("signal"));
    inlet_new(&x->obj, &x->obj.ob_pd, gensym("signal"), gensym("signal"));

    /* Create signal outlets */
    outlet_new(&x->obj, gensym("signal"));

#endif

    /* Print message to Max window */
    post("karplus~ • Object was created");

    /* Return a pointer to the new object */
    return x;
}

/* The 'free instance' routine ************************************************/
void karplus_free(t_karplus *x)
{
#ifdef TARGET_IS_MAX
    /* Remove the object from the DSP chain */
    dsp_free((t_pxobject *)x);
#endif

    /* Free allocated dynamic memory */
    if (x->wavetable != NULL) {
        free(x->wavetable);
    }

    /* Print message to Max window */
    post("karplus~ • Memory was freed");
}

/******************************************************************************/






/* The 'DSP' method ***********************************************************/
void karplus_dsp(t_karplus *x, t_signal **sp, short *count)
{
    /* Initialize state variables */
    x->fs = sp[0]->s_sr;

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

    /* Attach the object to the DSP chain */
    dsp_add(karplus_perform, NEXT-1, x,
            sp[0]->s_vec,
            sp[1]->s_vec,
            sp[2]->s_vec,
            sp[3]->s_vec,
            sp[4]->s_vec,
            sp[5]->s_vec,
            sp[0]->s_n);

    /* Print message to Max window */
    post("karplus~ • Executing 32-bit perform routine");
}

/* The 'perform' routine ******************************************************/
t_int *karplus_perform(t_int *w)
{
    /* Copy the object pointer */
    t_karplus *x = (t_karplus *)w[OBJECT];

    /* Copy signal pointers */
    t_float *pluck = (t_float *)w[PLUCK];
    t_float *freq = (t_float *)w[FREQ];
    t_float *s_coeff = (t_float *)w[S_COEFF];
    t_float *p_coeff = (t_float *)w[P_COEFF];
    t_float *c_coeff = (t_float *)w[C_COEFF];
    t_float *string = (t_float *)w[STRING];

    /* Copy the signal vector size */
    t_int n = w[VECTOR_SIZE];

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
        local_freq = *freq++;
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
        lowpass[0] = *p_coeff++ * (*s_coeff * delayline[0] + (1 - *s_coeff) * delayline[1]);
        *s_coeff++;

        // read and write allpass
        allpass[1] = allpass[0];
        allpass[0] = - *c_coeff * lowpass[0] + lowpass[1] + *c_coeff * allpass[1];
        *c_coeff++;

        // feedback and playback
        wavetable[rwindex] = *pluck++ + allpass[0];
        *string++ = wavetable[rwindex];

        // update rwindex
        rwindex++;
    }


    /* Update state variables */
    x->delaylength = delaylength;
    x->rwindex = rwindex;

    /* Return the next address in the DSP chain */
    return w + NEXT;
}

/******************************************************************************/
