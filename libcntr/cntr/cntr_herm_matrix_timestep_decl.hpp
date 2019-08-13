#ifndef CNTR_HERM_MATRIX_TIMESTEP_DECL_H
#define CNTR_HERM_MATRIX_TIMESTEP_DECL_H

#include "cntr_global_settings.hpp"

namespace cntr {

template <typename T> class function;
/// @private
template <typename T> class cyclic_timestep;
template <typename T> class herm_matrix;
template <typename T> class herm_matrix_timestep_view;

// NOTE: the bose/fermi sign for the herm_matrix_timestep (needed to compute
// vt from tv etc.)
// is currently not consistently treated ... safe only for fermionic GF
template <typename T>
/** \brief <b> Class `herm_matrix_timestep` deals with a timeslice \f$\mathcal{T}[C]_n\f$ contour objects \f$ C(t,t') \f$
 * at a particular timestep \f$n\f$.</b>
 *
 * <!-- ====== DOCUMENTATION ====== -->
 *
 *  \par Purpose
 * <!-- ========= -->
 *
 *  The class 'herm_matrix_timestep' has almost the same functionality
 * as the class 'herm_matrix'. Here, one considers however contour objects \f$ C(t,t') \f$
 * at a particular timestep \f$t_n\f$, denoted as time slices \f$\mathcal{T}[C]_n\f$.
 * The contour function \f$ C(t,t') \f$ can be of scalar type or matrix-valued.
 *
 */
class herm_matrix_timestep {
  public:
    typedef std::complex<T> cplx;
    typedef T scalar_type;
    /* construction, destruction */
    herm_matrix_timestep();
    ~herm_matrix_timestep();
    herm_matrix_timestep(int tstp,int ntau,int size1=1);
	  herm_matrix_timestep(int tstp,int ntau,int size1,int sig);
	  herm_matrix_timestep(int tstp,int ntau,int size1,int size2,int sig);
    herm_matrix_timestep(const herm_matrix_timestep &g);
	  herm_matrix_timestep & operator=(const herm_matrix_timestep &g);
#if __cplusplus >= 201103L
    herm_matrix_timestep(herm_matrix_timestep &&g) noexcept;
    herm_matrix_timestep &operator=(herm_matrix_timestep &&g) noexcept;
#endif
    /* resize */
    void resize(int nt, int ntau, int size1);
    void clear(void);
    /* access size etc ... */
    int size1(void) const { return size1_; }
    int size2(void) const { return size2_; }
    int tstp(void) const { return tstp_; }
    int ntau(void) const { return ntau_; }
    int sig(void) const { return sig_; }
    void get_matrixelement(int i1, int i2, herm_matrix<T> &g);
    void set_timestep(cyclic_timestep<T> &timestep,
                      cyclic_timestep<T> &timestep_cc, int sig);
    // Here timestep is tstp of pseudo ("ret=gtr")
    void set_timestep_pseudo(cyclic_timestep<T> &timestep,
                             cyclic_timestep<T> &timestep_cc, int sig);
    // assumes hermitan:
    void set_timestep(cyclic_timestep<T> &timestep, int sig);
    void set_timestep_pseudo(cyclic_timestep<T> &timestep, int sig);
    inline cplx *retptr(int i) { return data_ + i * element_size_; }
    inline cplx *tvptr(int i) {
        return data_ + (tstp_ + 1 + i) * element_size_;
    }
    inline cplx *lesptr(int i) {
        return data_ + (tstp_ + 1 + ntau_ + 1 + i) * element_size_;
    };
    // TODO: Think about the structure  - should we have separate set_ret and set_ret_t_tstp

    template <class Matrix>
    void set_ret(int tstp,Matrix &M);
    template <class Matrix>
    void set_les(int tstp,Matrix &M);
    template <class Matrix>
    void set_tv(int tstp,Matrix &M);
    template <class Matrix>
    void set_mat(int tstp,Matrix &M);

    ///// get_les_t_tstp(int i,Matrix &M) M = Gles(i*h,tstp) etc.
    template <class Matrix>
    void get_les_t_tstp(int i, Matrix &M);
    template <class Matrix>
    void get_les_tstp_t(int i, Matrix &M);
    template <class Matrix>
    void get_ret_tstp_t(int j, Matrix &M);
    template <class Matrix>
    void get_ret_t_tstp(int i, Matrix &M);
    template <class Matrix>
    void get_tv(int j, Matrix &M);
    template <class Matrix>
    void get_vt(int i, Matrix &M, int sig);
    template <class Matrix>
    void get_mat(int i, Matrix &M);
    template <class Matrix>
    void get_matminus(int i, Matrix &M, int sig);
    template <class Matrix>
    void get_gtr_tstp_t(int i, Matrix &M);
    template <class Matrix>
    void get_gtr_t_tstp(int i, Matrix &M);
    // reading complex numbers:
    // these will adress only (0,0) element for dim>1:
    // !!! these assume heremitian symmetry !!!
    // one dummy argument (must be tstp), in order to have some interface as
    // herm_matrix
    inline void get_les(int i, int j, cplx &x);
    inline void get_gtr(int i, int j, cplx &x);
    inline void get_ret(int i, int j, cplx &x);
    inline void get_vt(int i, int j, cplx &x);
    inline void get_tv(int i, int j, cplx &x);
    inline void get_mat(int i, cplx &x);
    inline void get_matminus(int i, cplx &x);
    cplx density_matrix(int tstp);
    template <class Matrix>
    void density_matrix(Matrix &M);
    /////
    inline cplx *matptr(int i) { return data_ + i * element_size_; }
    // multiplication with function
    void left_multiply(cplx *f0, cplx *ft, T weight = 1.0);
    void right_multiply(cplx *f0, cplx *ft, T weight = 1.0);
    void left_multiply(function<T> &ft, T weight = 1.0);
    void right_multiply(function<T> &ft, T weight = 1.0);
    void incr(herm_matrix_timestep<T> &g1, T weight);
    void incr(herm_matrix<T> &g, T weight = 1.0);
    void smul(T weight);
    void set_matrixelement(int i1, int i2, herm_matrix_timestep_view<T> &g,
                           int j1, int j2);
    void set_matrixelement(int i1, int i2, herm_matrix_timestep<T> &g, int j1,
                           int j2);
    void set_matrixelement(int i1, int i2, herm_matrix<T> &g, int j1, int j2);
// MPI UTILS
#if CNTR_USE_MPI == 1
    void MPI_Reduce(int root);
    void Bcast_timestep(int tstp, int ntau, int size1, int root);
    void Send_timestep(int tstp, int ntau, int size1, int dest, int tag);
    void Recv_timestep(int tstp, int ntau, int size1, int root, int tag);
#endif
// HDF5 I/O
#if CNTR_USE_HDF5 == 1
    void write_to_hdf5(hid_t group_id);
    void write_to_hdf5(hid_t group_id, const char *groupname);
    void write_to_hdf5(const char *filename, const char *groupname);
    // does a resize:
    void read_from_hdf5(hid_t group_id, const char *groupname);
    void read_from_hdf5(hid_t group_id);
    void read_from_hdf5(const char *filename, const char *groupname);
#endif
    /// @private
    /** \brief <b> Pointer to the data for the time step (tstp). 'data_+\f$ t\f$ * element_size' correponds to (0,0)-component of  \f$ G^R(tstp,t)\f$, 'data_+\f$(tstp+1+\tau)\f$ * element_size' correponds to (0,0)-component of \f$ G^\rceil(tstp,\tau)\f$, 'data_+\f$(tstp+1+n_{\tau}+1+t)\f$ * element_size' correponds to (0,0)-component of \f$ G^<(t,tstp)\f$. </b> */
    cplx *data_;
    /// @private
    /** \brief <b> time step.</b> */
    int tstp_;
    /// @private
    /** \brief <b> Number of the time grids on the Matsubara axis.</b> */
    int ntau_;
    /// @private
    /** \brief <b> Number of the colums in the Matrix form.</b> */
    int size1_;
    /// @private
    /** \brief <b> Number of the rows in the Matrix form.</b> */
    int size2_;
    /// @private
    /** \brief <b> Size of the Matrix form; size1*size2. </b> */
    int element_size_;
    /// @private
    /** \brief <b> Size of the data stored for the time step; \f$ (2*(tstp+1)+(n_{\tau}+1)) \f$ \;* element_size </b> */
    int total_size_;
    /// @private
    /** \brief <b> Bose = +1, Fermi =-1. </b> */
    int sig_;
};

} // namespace cntr

#endif  // CNTR_HERM_MATRIX_TIMESTEP_DECL_H