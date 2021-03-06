/*################################################################################
  ##
  ##   Copyright (C) 2016-2018 Keith O'Hara
  ##
  ##   This file is part of the GCE-Math C++ library.
  ##
  ##   GCE-Math is free software: you can redistribute it and/or modify
  ##   it under the terms of the GNU General Public License as published by
  ##   the Free Software Foundation, either version 2 of the License, or
  ##   (at your option) any later version.
  ##
  ##   GCE-Math is distributed in the hope that it will be useful,
  ##   but WITHOUT ANY WARRANTY; without even the implied warranty of
  ##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  ##   GNU General Public License for more details.
  ##
  ################################################################################*/

/*
 * inverse of the incomplete beta function
 */

#ifndef _gcem_incomplete_beta_inv_HPP
#define _gcem_incomplete_beta_inv_HPP

template<typename T>
constexpr T incomplete_beta_inv_decision(const T value, const T alpha_par, const T beta_par, const T p, const T direc, const T lb_val, const int iter_count);

//
// initial value for Halley

//
// a,b > 1 case

template<typename T>
constexpr
T
incomplete_beta_inv_initial_val_1_tval(const T p)
{ // a > 1.0
    return ( p > T(0.5) ? sqrt(-T(2.0)*log(T(1.0) - p)) : sqrt(-T(2.0)*log(p)) );
}

template<typename T>
constexpr
T
incomplete_beta_inv_initial_val_1_int_begin(const T t_val)
{ // internal for a > 1.0
    return ( t_val - (T(2.515517) + T(0.802853)*t_val + T(0.010328)*t_val*t_val)/(T(1.0) + T(1.432788)*t_val + T(0.189269)*t_val*t_val + T(0.001308)*t_val*t_val*t_val) );
}

template<typename T>
constexpr
T
incomplete_beta_inv_initial_val_1_int_ab1(const T alpha_par, const T beta_par)
{
    return ( T(1.0)/(2*alpha_par - T(1.0)) + T(1.0)/(2*beta_par - T(1.0)) );
}

template<typename T>
constexpr
T
incomplete_beta_inv_initial_val_1_int_ab2(const T alpha_par, const T beta_par)
{
    return ( T(1.0)/(2*beta_par - T(1.0)) - T(1.0)/(2*alpha_par - T(1.0)) );
}

// constexpr
// T
// incomplete_beta_inv_initial_val_1_int_lambda(const T value)
// {
//     return ( (value*value - 3.0)/6.0 );
// }

template<typename T>
constexpr
T
incomplete_beta_inv_initial_val_1_int_h(const T ab_term_1)
{
    return ( T(2.0) / ab_term_1 );
}

template<typename T>
constexpr
T
incomplete_beta_inv_initial_val_1_int_w(const T value, const T ab_term_2, const T h_term)
{
    // return ( value * sqrt(h_term + lambda)/h_term - ab_term_2*(lambda + 5.0/6.0 -2.0/(3.0*h_term)) );
    return ( value * sqrt(h_term + (value*value - T(3.0))/T(6.0))/h_term - ab_term_2*((value*value - T(3.0))/T(6.0) + T(5.0)/T(6.0) - T(2.0)/(T(3.0)*h_term)) );
}

template<typename T>
constexpr
T
incomplete_beta_inv_initial_val_1_int_end(const T alpha_par, const T beta_par, const T w_term)
{
    return ( alpha_par / (alpha_par + beta_par*exp(2*w_term)) );
}

template<typename T>
constexpr
T
incomplete_beta_inv_initial_val_1(const T alpha_par, const T beta_par, const T p, const T t_val)
{ // a > 1.0
    return ( p > T(0.5) ? incomplete_beta_inv_initial_val_1_int_end( alpha_par, beta_par, incomplete_beta_inv_initial_val_1_int_w(-incomplete_beta_inv_initial_val_1_int_begin(t_val),incomplete_beta_inv_initial_val_1_int_ab2(alpha_par,beta_par),incomplete_beta_inv_initial_val_1_int_h(incomplete_beta_inv_initial_val_1_int_ab1(alpha_par,beta_par))) ) :
                          incomplete_beta_inv_initial_val_1_int_end( alpha_par, beta_par, incomplete_beta_inv_initial_val_1_int_w( incomplete_beta_inv_initial_val_1_int_begin(t_val),incomplete_beta_inv_initial_val_1_int_ab2(alpha_par,beta_par),incomplete_beta_inv_initial_val_1_int_h(incomplete_beta_inv_initial_val_1_int_ab1(alpha_par,beta_par))) ) );
}

//
// a,b else

template<typename T>
constexpr
T
incomplete_beta_inv_initial_val_2_s1(const T alpha_par, const T beta_par)
{
    return ( pow_dbl(alpha_par/(alpha_par+beta_par),alpha_par) / alpha_par );
}

template<typename T>
constexpr
T
incomplete_beta_inv_initial_val_2_s2(const T alpha_par, const T beta_par)
{
    return ( pow_dbl(beta_par/(alpha_par+beta_par),beta_par) / beta_par );
}

template<typename T>
constexpr
T
incomplete_beta_inv_initial_val_2(const T alpha_par, const T beta_par, const T p, const T s_1, const T s_2)
{
    return ( p <= s_1/(s_1 + s_2) ? pow_dbl(p*(s_1+s_2)*alpha_par,T(1.0)/alpha_par) :
                                    T(1.0) - pow_dbl(p*(s_1+s_2)*beta_par,T(1.0)/beta_par) );
}

// initial value

template<typename T>
constexpr
T
incomplete_beta_inv_initial_val(const T alpha_par, const T beta_par, const T p)
{
    return ( (alpha_par > T(1.0) && beta_par > T(1.0)) ? incomplete_beta_inv_initial_val_1(alpha_par,beta_par,p,incomplete_beta_inv_initial_val_1_tval(p)) :
                                            p > T(0.5) ? T(1.0) - incomplete_beta_inv_initial_val_2(beta_par,alpha_par,T(1.0) - p,incomplete_beta_inv_initial_val_2_s1(beta_par,alpha_par),incomplete_beta_inv_initial_val_2_s2(beta_par,alpha_par)) : // regularization
                                                         incomplete_beta_inv_initial_val_2(alpha_par,beta_par,p,incomplete_beta_inv_initial_val_2_s1(alpha_par,beta_par),incomplete_beta_inv_initial_val_2_s2(alpha_par,beta_par)));
}

//
// Halley recursion

template<typename T>
constexpr
T
incomplete_beta_inv_err_val(const T value, const T alpha_par, const T beta_par, const T p)
{ // err_val = f(x)
    return ( incomplete_beta(alpha_par,beta_par,value) - p );
}

template<typename T>
constexpr
T
incomplete_beta_inv_deriv_1(const T value, const T alpha_par, const T beta_par, const T lb_val)
{ // derivative of the incomplete beta function w.r.t. x
    return ( exp( (alpha_par - T(1.0))*log(value) + (beta_par - T(1.0))*log(T(1.0) - value) - lb_val ) );
}

template<typename T>
constexpr
T
incomplete_beta_inv_deriv_2(const T value, const T alpha_par, const T beta_par, const T deriv_1)
{ // second derivative of the incomplete beta function w.r.t. x
    return ( deriv_1*((alpha_par - T(1.0))/value - (beta_par - T(1.0))/(T(1.0) - value)) );
}

template<typename T>
constexpr
T
incomplete_beta_inv_ratio_val_1(const T value, const T alpha_par, const T beta_par, const T p, const T deriv_1)
{
    return ( incomplete_beta_inv_err_val(value,alpha_par,beta_par,p) / deriv_1 );
}

template<typename T>
constexpr
T
incomplete_beta_inv_ratio_val_2(const T value, const T alpha_par, const T beta_par, const T p, const T deriv_1)
{
    return ( incomplete_beta_inv_deriv_2(value,alpha_par,beta_par,deriv_1) / deriv_1 );
}

template<typename T>
constexpr
T
incomplete_beta_inv_halley(const T ratio_val_1, const T ratio_val_2)
{
    return ( ratio_val_1 / max( T(0.8), min( T(1.2), T(1.0) - T(0.5)*ratio_val_1*ratio_val_2 ) ) );
}

template<typename T>
constexpr
T
incomplete_beta_inv_recur(const T value, const T alpha_par, const T beta_par, const T p, const T deriv_1, const T lb_val, const int iter_count)
{
    return ( incomplete_beta_inv_decision( value, alpha_par, beta_par, p, incomplete_beta_inv_halley(incomplete_beta_inv_ratio_val_1(value,alpha_par,beta_par,p,deriv_1),incomplete_beta_inv_ratio_val_2(value,alpha_par,beta_par,p,deriv_1)), lb_val, iter_count ) );
}

template<typename T>
constexpr
T
incomplete_beta_inv_decision(const T value, const T alpha_par, const T beta_par, const T p, const T direc, const T lb_val, const int iter_count)
{
    return ( iter_count <= GCEM_INCML_BETA_INV_MAX_ITER ? incomplete_beta_inv_recur(value-direc,alpha_par,beta_par,p, incomplete_beta_inv_deriv_1(value,alpha_par,beta_par,lb_val), lb_val, iter_count+1) : value - direc );
}

template<typename T>
constexpr
T
incomplete_beta_inv_int(const T initial_val, const T alpha_par, const T beta_par, const T p, const T lb_val)
{
    return ( incomplete_beta_inv_recur(initial_val,alpha_par,beta_par,p,incomplete_beta_inv_deriv_1(initial_val,alpha_par,beta_par,lb_val),lb_val,1) );
}

template<typename T>
constexpr
T
incomplete_beta_inv(const T alpha_par, const T beta_par, const T p)
{
    return ( incomplete_beta_inv_int(incomplete_beta_inv_initial_val(alpha_par,beta_par,p),alpha_par,beta_par,p,lbeta(alpha_par,beta_par)) );
}

#endif
