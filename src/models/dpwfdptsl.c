#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <malloc.h>
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_errno.h>
#include "../stehfest/stehfest.h"

#include "dpwfdptsl.h"

/*** DOUBLE POROSITY INFINITE RESERVOIR TRANSIENT INTERPOROSITY FLOW SLABS ***/
/* References:
*
* Bourdet, D. and Gringarten, A.C., Determination of Fissured Volume and Block
* Size in Fractured Reservoirs by Type-Curve Analysis, SPE 9293,1980
*/


/**
* transient interporosity flow function for slabs
*/
double ftsl(const double uD, const double omega, const double lambdas)
{
    double fslabs;


    fslabs = omega + sqrt( lambdas*(1.0 - omega) / (3.0*uD) )
             * tanh( sqrt( 3.0*uD*(1.0 - omega) / lambdas ) );

    return fslabs;
}
/*****************************************************************************/


/**
d(dpwf)/dC function in the Laplace space
*/
double ddpwfdptsl_dCbar(const void *parameters, double u)
{
    double bc_a, dpwfb;
    modelparameters *p;

    p = (modelparameters *)parameters;

    gsl_set_error_handler_off();

    bc_a  = (p->C3) / (p->C1 * p->C2 * p->qB);

    dpwfb = dpwfdpbar(p, u);

    return -bc_a * u * u * (dpwfb * dpwfb);
}
/*****************************************************************************/


/**
* deltapwf (pressure drop per unit constant flow rate) function in physical
* space. numerical inversion of the Laplace transform using Stehfest's
* algorithm
*/
double dpwfdptsl(const modelparameters *p, double t)
{
    double f;

    if(t == 0.0) {
        f = 0.0;
    }
    else {
        f = stehfest_ilt(&dpwfdpbar, p, p->nstehfest, p->v, t);
    }

    return f;
}
/*****************************************************************************/


/**
d(dpwf)/dC function function in physical space. numerical
inversion of the Laplace transform using Stehfest's
algorithm
*/
double ddpwfdptsl_dC(const modelparameters *p, double t)
{
    double f;

    if(t == 0.0) {
        f = 0.0;
    }
    else {
        f = stehfest_ilt(&ddpwfdptsl_dCbar, p, p->nstehfest, p->v, t);
    }

    return f;
}
/*****************************************************************************/
