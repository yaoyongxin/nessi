#ifndef CNTR_DYSON_DECL_H
#define CNTR_DYSON_DECL_H

#include "cntr_global_settings.hpp"
#include "integration.hpp"

namespace cntr {

  template <typename T> class function;
  template <typename T> class herm_matrix;

/*###########################################################################################
#
#   DYSON EQUATION [ id/dt + mu - H(t) ] G(t,t') - [Sigma*G](t,t') = 1(t,t')
#
#   H,G,Sigma are assumed to be hermitian
#   ... implemented only for herm_matrix
#   implemented in green_cntr_dyson.hpp
#
###########################################################################################*/
// internal interface 
  template <typename T>
  void dyson_mat_fourier(herm_matrix<T> &G, herm_matrix<T> &Sigma, T mu, function<T> &H, T beta,
    int order = 3);
  template <typename T>
  void dyson_mat_fourier(herm_matrix<T> &G, herm_matrix<T> &Sigma, T mu, function<T> &H,
       function<T> &SigmaMF, T beta,int order = 3);

  template <typename T>
  void dyson_mat_fixpoint(herm_matrix<T> &G, herm_matrix<T> &Sigma, T mu, function<T> &H,
        integration::Integrator<T> &I, T beta, int fixpiter = 6);
  template <typename T>
  void dyson_mat_fixpoint(herm_matrix<T> &G, herm_matrix<T> &Sigma, T mu, function<T> &H,
        function<T> &SigmaMF, integration::Integrator<T> &I, T beta,
        int fixpiter = 6);

  template <typename T>
  void dyson_mat_steep(herm_matrix<T> &G, herm_matrix<T> &Sigma, T mu, function<T> &H,
           integration::Integrator<T> &I, T beta, int maxiter = 10, T tol=1.0e-16);
  template <typename T>
  void dyson_mat_steep(herm_matrix<T> &G, herm_matrix<T> &Sigma, T mu, function<T> &H,
           function<T> &SigmaMF, integration::Integrator<T> &I, T beta,
           int maxiter = 10, T tol=1.0e-16);

  template <typename T>
  void dyson_mat(herm_matrix<T> &G, herm_matrix<T> &Sigma, T mu, function<T> &H,
     integration::Integrator<T> &I, T beta, const int method=CNTR_MAT_FIXPOINT,
     const bool force_hermitian=true);
  template <typename T>
  void dyson_mat(herm_matrix<T> &G, herm_matrix<T> &Sigma, T mu, function<T> &H,
     function<T> &SigmaMF, integration::Integrator<T> &I, T beta,
     const int method=CNTR_MAT_FIXPOINT,const bool force_hermitian=true);

  template <typename T>
  void dyson_start(herm_matrix<T> &G, T mu, function<T> &H, herm_matrix<T> &Sigma,
    integration::Integrator<T> &I, T beta, T h);

  template <typename T>
  void dyson_timestep(int n, herm_matrix<T> &G, T mu, function<T> &H, herm_matrix<T> &Sigma,
    integration::Integrator<T> &I, T beta, T h);

  template <typename T>
  void dyson(herm_matrix<T> &G, T mu, function<T> &H, herm_matrix<T> &Sigma,
    integration::Integrator<T> &I, T beta, T h, const int matsubara_method=CNTR_MAT_FIXPOINT,
    const bool force_hermitian=true);

// documented user interfaces

  template <typename T>
  void dyson_mat(T beta, T mu, herm_matrix<T> &G, function<T> &H, herm_matrix<T> &Sigma, 
     const int kt=MAX_ORDER, const int method=CNTR_MAT_FIXPOINT,
     const bool force_hermitian=true);

  template <typename T>
  void dyson_mat(T beta, T mu, herm_matrix<T> &G, function<T> &H, function<T> &SigmaMF, 
     herm_matrix<T> &Sigma, const int kt=MAX_ORDER, const int method=CNTR_MAT_FIXPOINT,
     const bool force_hermitian=true);

  template <typename T>
  void dyson_start(T beta, T mu, T h, herm_matrix<T> &G, function<T> &H, herm_matrix<T> &Sigma,
    const int kt=MAX_ORDER);

  template <typename T>
  void dyson_timestep(int n, T beta, T mu, T h, herm_matrix<T> &G, function<T> &H, herm_matrix<T> &Sigma,
    const int kt=MAX_ORDER);

  template <typename T>
  void dyson(T beta, T mu, T h, herm_matrix<T> &G, function<T> &H, herm_matrix<T> &Sigma,
    const int kt=MAX_ORDER, const int matsubara_method=CNTR_MAT_FIXPOINT,
    const bool force_hermitian=true);
  
} // namespace cntr

#endif  // CNTR_DYSON_DECL_H