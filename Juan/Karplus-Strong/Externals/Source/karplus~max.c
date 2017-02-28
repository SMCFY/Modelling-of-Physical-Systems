/* Common header files ********************************************************/
#include "karplus~common.h"

/* Function prototypes ********************************************************/
void *karplus_new(t_symbol *s, short argc, t_atom *argv);

void karplus_float(t_karplus *x, double farg);
void karplus_assist(t_karplus *x, void *b, long msg, long arg, char *dst);

/* The 'initialization' routine ***********************************************/
int C74_EXPORT main()
{
    /* Initialize the class */
    karplus_class = class_new("karplus~",
                              (method)karplus_new,
                              (method)karplus_free,
                              sizeof(t_karplus), 0, A_GIMME, 0);

    /* Bind the DSP method, which is called when the DACs are turned on */
    class_addmethod(karplus_class, (method)karplus_dsp, "dsp", A_CANT, 0);

    /* Bind the float method, which is called when floats are sent to inlets */
    class_addmethod(karplus_class, (method)karplus_float, "float", A_FLOAT, 0);

    /* Bind the assist method, which is called on mouse-overs to inlets and outlets */
    class_addmethod(karplus_class, (method)karplus_assist, "assist", A_CANT, 0);

    /* Add standard Max methods to the class */
    class_dspinit(karplus_class);

    /* Register the class with Max */
    class_register(CLASS_BOX, karplus_class);

    /* Print message to Max window */
    object_post(NULL, "karplus~ • External was loaded");

    /* Return with no error */
    return 0;
}

/* The 'new instance' routine *************************************************/
void *karplus_new(t_symbol *s, short argc, t_atom *argv)
{
    /* Instantiate a new object */
    t_karplus *x = (t_karplus *)object_alloc(karplus_class);

    return common_new(x, argc, argv);
}

/******************************************************************************/






/* The 'float' method *********************************************************/
void karplus_float(t_karplus *x, double farg)
{
    // nothing
}

/* The 'assist' method ********************************************************/
void karplus_assist(t_karplus *x, void *b, long msg, long arg, char *dst)
{
    /* Document inlet functions */
    if (msg == ASSIST_INLET) {
        switch (arg) {
            case I_PLUCK: sprintf(dst, "(signal) Noise burst"); break;
            case I_FREQ: sprintf(dst, "(signal) Frequency"); break;
            case I_S_COEFF: sprintf(dst, "(signal) Coefficient S (lowpass)"); break;
            case I_P_COEFF: sprintf(dst, "(signal) Coefficient P (lowpass gain)"); break;
            case I_C_COEFF: sprintf(dst, "(signal) Coefficient C (allpass)"); break;
        }
    }

    /* Document outlet functions */
    else if (msg == ASSIST_OUTLET) {
        switch (arg) {
            case O_STRING: sprintf(dst, "(signal) Magnitude"); break;
        }
    }
}

/******************************************************************************/
