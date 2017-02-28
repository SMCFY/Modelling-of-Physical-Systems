#ifndef karplus_common_h
#define karplus_common_h

/* Header files required by Max and Pd ****************************************/
#ifdef TARGET_IS_MAX
#include "ext.h"
#include "z_dsp.h"
#include "ext_obex.h"
#elif TARGET_IS_PD
#include "m_pd.h"
#include "math.h"
#endif

#include <stdlib.h>

/* The object structure *******************************************************/
typedef struct _karplus {
#ifdef TARGET_IS_MAX
    t_pxobject obj;
#elif TARGET_IS_PD
    t_object obj;
    t_float x_f;
#endif

    float fs;

    long delaylength;
    float* wavetable;

    float delayline[2];
    float lowpass[2];
    float allpass[2];

    long rwindex;
} t_karplus;

/* The arguments/inlets/outlets/vectors indexes *******************************/
enum ARGUMENTS { NONE };
enum INLETS { I_PLUCK, I_FREQ, I_S_COEFF, I_P_COEFF, I_C_COEFF, NUM_INLETS };
enum OUTLETS { O_STRING, NUM_OUTLETS };
enum DSP { PERFORM, OBJECT,
           PLUCK, FREQ, S_COEFF, P_COEFF, C_COEFF, STRING,
           VECTOR_SIZE, NEXT };

/* The class pointer **********************************************************/
static t_class *karplus_class;

/* Function prototypes ********************************************************/
void *common_new(t_karplus *x, short argc, t_atom *argv);
void karplus_free(t_karplus *x);

void karplus_dsp(t_karplus *x, t_signal **sp, short *count);
t_int *karplus_perform(t_int *w);

/******************************************************************************/

#endif /* karplus_common_h */
