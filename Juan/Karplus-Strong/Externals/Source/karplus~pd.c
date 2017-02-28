/* Common header files ********************************************************/
#include "karplus~common.h"

/* Function prototypes ********************************************************/
void *karplus_new(t_symbol *s, short argc, t_atom *argv);

/* The 'initialization' routine ***********************************************/
#ifdef WIN32
__declspec(dllexport) void karplus_tilde_setup(void);
#endif
void karplus_tilde_setup(void)
{
    /* Initialize the class */
    karplus_class = class_new(gensym("karplus~"),
                              (t_newmethod)karplus_new,
                              (t_method)karplus_free,
                              sizeof(t_karplus), 0, A_GIMME, 0);

    /* Specify signal input, with automatic float to signal conversion */
    CLASS_MAINSIGNALIN(karplus_class, t_karplus, x_f);

    /* Bind the DSP method, which is called when the DACs are turned on */
    class_addmethod(karplus_class, (t_method)karplus_dsp, gensym("dsp"), 0);

    /* Bind the object-specific methods */
    // nothing

    /* Print message to Max window */
    post("karplus~ • External was loaded");
}

/* The 'new instance' routine *************************************************/
void *karplus_new(t_symbol *s, short argc, t_atom *argv)
{
    /* Instantiate a new object */
    t_karplus *x = (t_karplus *)pd_new(karplus_class);

    return common_new(x, argc, argv);
}

/******************************************************************************/
