#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <malloc.h>
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_errno.h>
#include "../stehfest/stehfest.h"

#include "dpwfnfob.h"

/********************** RADIAL FLOW, NO FLOW OUTER BOUNDARY ******************/
/* References:
*
* A.F. Van Everdingen and W. Hurst, The Application of the Laplace
* Transformation to Flow Problems in Reservoirs, SPE-949305-G, 1949
*/

/**
delta pwf function in the Laplace space, for a homogeneous circular reservoir,
with skin factor and wellbore storage effects, with no flow outer boundary
*/
double dpwfnfobbar(const void *parameters, double u)
{
    double aux0, aux1, aux2;
    double alpha, zeta, gamma;
    double denom, numer, a, b, CD, rws, z;
    modelparameters *p;

    p = (modelparameters *)parameters;

    gsl_set_error_handler_off();

    rws = p->rw*exp(-p->S);
    a   = (p->qB * p->mu * p->C2) / (p->h * p->k);
    b   = (p->phi * p->mu * p->ct * rws * rws) / (p->k * p->C1);
    z   = (p->phi * p->mu * p->ct)             / (p->k * p->C1);
    CD  = (p->C * p->C3) / (p->phi * p->h  * p->ct * rws * rws);

    aux0  = (u * b);
    aux1  = sqrt(aux0);
    aux2  = p->re * sqrt(u * z);

    alpha  = gsl_sf_bessel_K0(aux1)/(aux0*aux1*gsl_sf_bessel_K1(aux1));
    zeta   = gsl_sf_bessel_I0(aux1)/(aux0*aux1*gsl_sf_bessel_I1(aux1));
    gamma  = (gsl_sf_bessel_I1(aux2)*gsl_sf_bessel_K1(aux1))
             / (gsl_sf_bessel_I1(aux1)*gsl_sf_bessel_K1(aux2));

    numer  = alpha/(1.0 - (1.0/gamma)) - zeta/(1.0 - gamma);
    denom  = 1.0 + aux0 * aux0 * CD * numer;

    return (a*b) * (numer / denom);
}
/*****************************************************************************/


/**
d(dpwf)/dC function in the Laplace space
*/
double ddpwfnfob_dCbar(const void *parameters, double u)
{
    double bc_a, dpwfbar;
    modelparameters *p;

    p = (modelparameters *)parameters;

    gsl_set_error_handler_off();

    bc_a    = (p->C3) / (p->C1 * p->C2 * p->qB);

    dpwfbar = dpwfnfobbar(p, u);

    return -bc_a * u * u * (dpwfbar * dpwfbar);
}
/*****************************************************************************/


/**
d(dpwf)/dS function in the Laplace space
*/
double ddpwfnfob_dSbar(const void *parameters, double u)
{
    double aux0, aux1, aux2;
    double alpha, zeta, gamma;
    double denom, denom_sqr, numer, a, b, CD, rws, z;
    modelparameters *p;

    p = (modelparameters *)parameters;

    gsl_set_error_handler_off();

    rws = p->rw*exp(-p->S);
    a   = (p->qB * p->mu * p->C2) / (p->h * p->k);
    b   = (p->phi * p->mu * p->ct * rws * rws) / (p->k * p->C1);
    z   = (p->phi * p->mu * p->ct)             / (p->k * p->C1);
    CD  = (p->C * p->C3) / (p->phi * p->h  * p->ct * rws * rws);

    aux0  = (u * b);
    aux1  = sqrt(aux0);
    aux2  = p->re * sqrt(u * z);

    alpha  = gsl_sf_bessel_K0(aux1)/(aux0*aux1*gsl_sf_bessel_K1(aux1));
    zeta   = gsl_sf_bessel_I0(aux1)/(aux0*aux1*gsl_sf_bessel_I1(aux1));
    gamma  = (gsl_sf_bessel_I1(aux2)*gsl_sf_bessel_K1(aux1))
             / (gsl_sf_bessel_I1(aux1)*gsl_sf_bessel_K1(aux2));

    numer  = alpha/(1.0 - (1.0/gamma)) - zeta/(1.0 - gamma);
    denom  = 1.0 + aux0 * aux0 * CD * numer;

    denom_sqr = denom * denom;

    return (a / u) * (1.0 - aux0*aux0*aux0 * numer*numer) / denom_sqr;
}
/*****************************************************************************/


/**
d(dpwf)/dre function in the Laplace space
*/
double ddpwfnfob_drebar(const void *parameters, double u)
{
    double aux0, aux1, aux2, aux3;
    double alpha, zeta, gamma;
    double denom, denom_sqr, numer, a, b, CD, rws, z;
    modelparameters *p;

    p = (modelparameters *)parameters;

    gsl_set_error_handler_off();

    rws = p->rw*exp(-p->S);
    a   = (p->qB * p->mu * p->C2) / (p->h * p->k);
    b   = (p->phi * p->mu * p->ct * rws * rws) / (p->k * p->C1);
    z   = (p->phi * p->mu * p->ct)             / (p->k * p->C1);
    CD  = (p->C * p->C3) / (p->phi * p->h  * p->ct * rws * rws);

    aux0  = (u * b);
    aux1  = sqrt(aux0);
    aux2  = p->re * sqrt(u * z);

    alpha  = gsl_sf_bessel_K0(aux1)/(aux0*aux1*gsl_sf_bessel_K1(aux1));
    zeta   = gsl_sf_bessel_I0(aux1)/(aux0*aux1*gsl_sf_bessel_I1(aux1));
    gamma  = (gsl_sf_bessel_I1(aux2)*gsl_sf_bessel_K1(aux1))
             / (gsl_sf_bessel_I1(aux1)*gsl_sf_bessel_K1(aux2));

    aux3   = gsl_sf_bessel_I1(aux2)*gsl_sf_bessel_K1(aux1)
             - gsl_sf_bessel_I1(aux1)*gsl_sf_bessel_K1(aux2);
    aux3  *= aux3;

    numer  = alpha/(1.0 - (1.0/gamma)) - zeta/(1.0 - gamma);
    denom  = 1.0 + aux0 * aux0 * CD * numer;

    denom_sqr = denom * denom;

    return -(a * b) / (aux3 * p->re * aux0 * aux0 * denom_sqr);
}
/*****************************************************************************/


/**
d(dpwf)/dk function in the Laplace space
*/
double ddpwfnfob_dkbar(const void *parameters, double u)
{
    double aux0, aux1, aux2, aux3;
    double alpha, zeta, gamma;
    double denom, denom_sqr, numer, a, b, CD, rws, z;
    modelparameters *p;

    p = (modelparameters *)parameters;

    gsl_set_error_handler_off();

    rws = p->rw*exp(-p->S);
    a   = (p->qB * p->mu * p->C2) / (p->h * p->k);
    b   = (p->phi * p->mu * p->ct * rws * rws) / (p->k * p->C1);
    z   = (p->phi * p->mu * p->ct)             / (p->k * p->C1);
    CD  = (p->C * p->C3) / (p->phi * p->h  * p->ct * rws * rws);

    aux0  = (u * b);
    aux1  = sqrt(aux0);
    aux2  = p->re * sqrt(u * z);

    alpha  = gsl_sf_bessel_K0(aux1)/(aux0*aux1*gsl_sf_bessel_K1(aux1));
    zeta   = gsl_sf_bessel_I0(aux1)/(aux0*aux1*gsl_sf_bessel_I1(aux1));
    gamma  = (gsl_sf_bessel_I1(aux2)*gsl_sf_bessel_K1(aux1))
             / (gsl_sf_bessel_I1(aux1)*gsl_sf_bessel_K1(aux2));

    aux3   = gsl_sf_bessel_I1(aux2)*gsl_sf_bessel_K1(aux1)
             - gsl_sf_bessel_I1(aux1)*gsl_sf_bessel_K1(aux2);
    aux3  *= aux3;

    numer  = alpha/(1.0 - (1.0/gamma)) - zeta/(1.0 - gamma);
    denom  = 1.0 + aux0 * aux0 * CD * numer;

    denom_sqr = denom * denom;

    return ( -(a * b/p->k)*numer + a/(u*p->k)
             - (a*b*aux0*aux0/(2.0*p->k))*numer*numer
             + a*b/(2*p->k*aux0*aux0*aux3) ) / denom_sqr;
}
/*****************************************************************************/


/**
deltapwf function in physical space. numerical inversion of the Laplace
transform using Stehfest's algorithm
*/
double dpwfnfob(const modelparameters *p, double t)
{
    double f;

    if(t == 0.0)
    {
        f = 0.0;
    }
    else
    {
        f = stehfest_ilt(&dpwfnfobbar, p, p->nstehfest, p->v, t);
    }

    return f;
}
/*****************************************************************************/


/**
d(dpwf)/dC function in in physical space. numerical inversion of the Laplace
transform using Stehfest's algorithm
*/
double ddpwfnfob_dC(const modelparameters *p, double t)
{
    double f;

    if(t == 0.0)
    {
        f = 0.0;
    }
    else
    {
        f = stehfest_ilt(&ddpwfnfob_dCbar, p, p->nstehfest, p->v, t);
    }

    return f;
}
/*****************************************************************************/


/**
d(dpwf)/dS function in in physical space. numerical inversion of the Laplace
transform using Stehfest's algorithm
*/
double ddpwfnfob_dS(const modelparameters *p, double t)
{
    double f;

    if(t == 0.0)
    {
        f = 0.0;
    }
    else
    {
        f = stehfest_ilt(&ddpwfnfob_dSbar, p, p->nstehfest, p->v, t);
    }

    return f;
}
/*****************************************************************************/


/**
d(dpwf)/dre function in in physical space. numerical inversion of the Laplace
transform using Stehfest's algorithm
*/
double ddpwfnfob_dre(const modelparameters *p, double t)
{
    double f;

    if(t == 0.0)
    {
        f = 0.0;
    }
    else
    {
        f = stehfest_ilt(&ddpwfnfob_drebar, p, p->nstehfest, p->v, t);
    }

    return f;
}
/*****************************************************************************/


/**
d(dpwf)/dk function in in physical space. numerical inversion of the Laplace
transform using Stehfest's algorithm
*/
double ddpwfnfob_dk(const modelparameters *p, double t)
{
    double f;

    if(t == 0.0)
    {
        f = 0.0;
    }
    else
    {
        f = stehfest_ilt(&ddpwfnfob_dkbar, p, p->nstehfest, p->v, t);
    }

    return f;
}
/*****************************************************************************/
