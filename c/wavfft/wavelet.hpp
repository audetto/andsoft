#ifndef WAVELET_HPP
#define WAVELET_HPP

template <typename DP> void wt1(DP a[], int n, int isign, void (*wstep)(DP [], int, int)) {
  int nn;
  if (n < 4) return;
  if (isign >= 0)
    for (nn = n; nn >= 4; nn >>= 1)
      (*wstep)(a, nn, isign);
  else
    for (nn = 4; nn <= n; nn <<= 1)
      (*wstep)(a, nn, isign);
}

template <typename DP> void daub4(DP a[], int n, int isign) {
  const DP C0 =  0.4829629131445341;
  const DP C1 =  0.8365163037378079;
  const DP C2 =  0.2241438680420134;
  const DP C3 = -0.1294095225512604;
		
  int nh, i, j;
		
  if (n < 4) return;
		
  DP *wksp = new DP [n];
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
  memcpy(a, wksp, sizeof(DP) * n);
  delete [] wksp;
}

template <typename DP> class wavefilt {
private:
  std::vector<DP> *cc_p, *cr_p;
public:
  int ncof, ioff, joff;
  std::vector<DP> &cc, &cr;
  wavefilt() : cc(*cc_p), cr(*cr_p) {}
  wavefilt(const DP a[], int n) :
    cc_p(new std::vector<DP>(n)), cr_p(new std::vector<DP>(n)), 
    ncof(n), ioff( -(n << 1)), joff( -(n >> 1)), cc(*cc_p), cr(*cr_p) {
    int i;
    for (i = 0; i < n; i++)
      cc[i] = *a++;
    DP sig = - 1.0;
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

/*

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

*/

#endif
