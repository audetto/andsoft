#include <utili\errori.h>

#include <varie\fourier.h>

#include <ql\quantlib.hpp>

namespace Aletti {

	/*	Il primo è SEMPRE REALE
			I restanti sono complessi con le parti immaginarie che cominciano al 
				contrario dal fondo
			Quello che avanza (se) è REALE */

	void RFFT_multiply(double x[], double y[], int N) {
		
		int M = N / 2;
		
		// DC reale
		x[0] = x[0] * y[0];
		for (int i = 1; i < (N + 1) / 2; i++) {
			double xi = x[i];		// serve per la trasformazione "in place"
			int j = N - i;
			// real
			x[i] = xi * y[i] - x[j] * y[j];
			// imaginary
			x[j] = xi * y[j] + x[j] * y[i];
		}
		// se è pari c'è il punto sulla frequenza di Nyquist che è reale
		if ((N & 1) == 0)
			x[M] = x[M] * y[M];
	}

	void RFFT_one(double z[], int N) {
		memset(z, 0, sizeof(double) * N);
		double tmp = 1.0 / N;

		for (int i = 0; i < (N + 1) / 2; i++)
			z[i] = tmp;
		
		// se è pari c'è il punto sulla frequenza di Nyquist che è reale
		if ((N & 1) == 0)	z[N / 2] = tmp;
	}

// ******************************************************************************

	void wt1(double a[], int n, int isign, void (*wtstep)(double [], int, int)) {
		int nn;

		if (1 << int(log(n) / log(2)) != n)
			errore("DWT must be a power of 2!");

		if (n < 4) return;
		if (isign >= 0) {
			for (nn = n; nn >= 4; nn >>= 1) (*wtstep)(a, nn, isign);
		} else {
			for (nn = 4; nn <= n; nn <<= 1) (*wtstep)(a, nn, isign);
		}
	}
	
	void daub4(double a[], int n, int isign) {
		const double C0 =  0.4829629131445341;
		const double C1 =  0.8365163037378079;
		const double C2 =  0.2241438680420134;
		const double C3 = -0.1294095225512604;
		
		int nh, i, j;
		
		if (n < 4) return;
		
		double *wksp = new double [n];
		nh = n >> 1;
		if (isign >= 0) {
			for (i = 0, j = 0; j < n - 3; j += 2, i++) {
				wksp[i] =      C0 * a[j] + C1 * a[j + 1] + C2 * a[j + 2] + C3 * a[j + 3];
				wksp[i + nh] = C3 * a[j] - C2 * a[j + 1] + C1 * a[j + 2] - C0 * a[j + 3];
			}
			wksp[i] =      C0 * a[n - 2] + C1 * a[n - 1] + C2 * a[0] + C3 * a[1];
			wksp[i + nh] = C3 * a[n - 2] - C2 * a[n - 1] + C1 * a[0] - C0 * a[1];
		} else {
			wksp[0] = C2 * a[nh - 1] + C1 * a[n - 1] + C0 * a[0] + C3 * a[nh];
			wksp[1] = C3 * a[nh - 1] - C0 * a[n - 1] + C1 * a[0] - C2 * a[nh];
			for (i = 0, j = 2; i < nh - 1; i++) {
				wksp[j++] = C2 * a[i] + C1 * a[i + nh] + C0 * a[i + 1] + C3 * a[i + nh + 1];
				wksp[j++] = C3 * a[i] - C0 * a[i + nh] + C1 * a[i + 1] - C2 * a[i + nh + 1];
			}
		}
		//	for (i = 0; i < n; i++) a[i] = wksp[i];
		memcpy(a, wksp, sizeof(double) * n);
		delete [] wksp;
	}
	
	class wavefilt {
	private:
		std::vector<double> *cc_p, *cr_p;
	public:
		int ncof, ioff, joff;
		std::vector<double> &cc, &cr;
		wavefilt() : cc(*cc_p), cr(*cr_p) {}
		wavefilt(const double a[], int n) :
		cc_p(new std::vector<double>(n)), cr_p(new std::vector<double>(n)), 
			ncof(n), ioff( -(n << 1)), joff( -(n >> 1)), cc(*cc_p), cr(*cr_p) {
			int i;
			for (i = 0; i < n; i++)
				cc[i] = *a++;
			double sig = - 1.0;
			for (i = 0; i < n; i++) {
				cr[n - 1 - i] = sig * cc[i];
				sig = - sig;
			}
		}
		~wavefilt() {
			if (cc_p != NULL) delete cc_p;
			if (cr_p != NULL) delete cr_p;
		}
	};

	static QuantLib::Handle<wavefilt> wfilt_p;

	void pwtset(int n) {

		const static double c4_d[4] = {
			 0.4829629131445341,
			 0.8365163037378079,
			 0.2241438680420134,
			-0.1294095225512604
		};
		const static double c12_d[12] = {
			 0.111540743350,
			 0.494623890398,
			 0.751133908021,
			 0.315250351709,
			-0.226264693965,
			-0.129766867567,
			 0.097501605587,
			 0.027522865530,
			-0.031582039318,
			 0.000553842201,
			 0.004777257511,
			-0.001077301085
		};
		const static double c20_d[20] = {
			 0.026670057901,
			 0.188176800078,
			 0.527201188932,
			 0.688459039454,
			 0.281172343661,
			-0.249846424327,
			-0.195946274377,
			 0.127369340336,
			 0.093057364604,
			-0.071394147166,
			-0.029457536822,
			 0.033212674059,
			 0.003606553567,
			-0.010733175483,
			 0.001395351747,
			 0.001992405295,
			-0.000685856695,
			-0.000116466855,
			 0.000093588670,
			-0.000013264203
		};
	
		if (n == 4)
			wfilt_p = QuantLib::Handle<wavefilt>(new wavefilt(c4_d, n));
		else if (n == 12)
			wfilt_p = QuantLib::Handle<wavefilt>(new wavefilt(c12_d, n));
		else if (n == 20)
			wfilt_p = QuantLib::Handle<wavefilt>(new wavefilt(c20_d, n));
		else
			errore("unimplemented value, n in pwtset");
	}

	void pwt(double a[], int n, int isign) {
		double ai, ai1;
		int i, ii, j, jf, jr, k, n1, ni, nj, nh, nmod;
		
		if (n < 4) return;
		wavefilt &wfilt = *wfilt_p;
		double *wksp = new double [n];

		nmod = wfilt.ncof * n;
		n1 = n - 1;
		nh = n >> 1;
		for (j = 0; j < n; j++) wksp[j] = 0.0;
		if (isign >= 0) {
			for (ii = 0, i = 0; i < n; i += 2, ii++) {
				ni = i + 1 + nmod + wfilt.ioff;
				nj = i + 1 + nmod + wfilt.joff;
				for (k = 0; k < wfilt.ncof; k++) {
					jf = n1 & (ni + k + 1);
					jr = n1 & (nj + k + 1);
					wksp[ii] += wfilt.cc[k] * a[jf];
					wksp[ii + nh] += wfilt.cr[k] * a[jr];
				}
			}
		} else {
			for (ii = 0, i = 0; i < n; i += 2, ii++) {
				ai = a[ii];
				ai1 = a[ii + nh];
				ni = i + 1 + nmod + wfilt.ioff;
				nj = i + 1 + nmod + wfilt.joff;
				for (k = 0; k < wfilt.ncof; k++) {
					jf = n1 & (ni + k + 1);
					jr = n1 & (nj + k + 1);
					wksp[jf] += wfilt.cc[k] * ai;
					wksp[jr] += wfilt.cr[k] * ai1;
				}
			}
		}
		for (j = 0; j < n;j++) a[j] = wksp[j];
		delete [] wksp;
	}

	void wtn(double a[], int nn[], int ndim, int isign, void (*wtstep)(double [], int, int)) {
		int idim, i1, i2, i3, k, n, nnew, nprev = 1, nt, ntot = 1;
		
		for (idim = 0; idim < ndim; idim++) ntot *= nn[idim];
		double *wksp = new double [ntot];
		for (idim = 0; idim < ndim; idim++) {
			n = nn[idim];
			nnew = n * nprev;
			if (n > 4) {
				for (i2 = 0; i2 < ntot; i2 += nnew) {
					for (i1 = 0; i1 < nprev; i1++) {
						for (i3 = i1 + i2, k = 0; k < n; k++, i3 += nprev) wksp[k] = a[i3];
						if (isign >= 0) {
							for (nt = n; nt >= 4; nt >>= 1)
								(*wtstep)(wksp, nt, isign);
						} else {
							for (nt = 4; nt <= n; nt <<= 1)
								(*wtstep)(wksp, nt, isign);
						}
						for (i3 = i1 + i2, k = 0; k < n; k++, i3 += nprev) a[i3] = wksp[k];
					}
				}
			}
			nprev = nnew;
		}
		delete [] wksp;
	}

}