#include "encoder.hpp"
#include "wavelet.hpp"
#include "index.hpp"

#include <math.h>

WAV_Encoder::WAV_Encoder(short int channels, int bytes, int window, int points) :
  channels_(channels), bytes_(bytes), window_(window), points_(points), err_up(0), err_down(0) {
  
  wave_ = new float [window_];
}

WAV_Encoder::~WAV_Encoder() {
  delete [] wave_;
}

void WAV_Encoder::encode(char p[]) {
  if (channels_ == 2 && bytes_ == 2) {

    int i;

    short int *data = (short int *)p;

    for (i = 0; i < window_; i++) {
      wave_[i] = data[i * 2] / 32767.0;
    }
    bool res = encode_single();
    if (res) {
      for (i = 0; i < window_; i++) {
	int tmp = wave_[i] * 32767;
	if (tmp > 32767) {
	  tmp = 32767;
	  err_up++;
	}
	if (tmp < -32767) {
	  tmp = -32767;
	  err_down++;
	}
	data[i * 2] = tmp;
      }
    }

    for (i = 0; i < window_; i++) {
      wave_[i] = data[i * 2 + 1] / 32767.0;
    }
    res = encode_single();
    if (res) {
      for (i = 0; i < window_; i++) {
	int tmp = wave_[i] * 32767;
	if (tmp > 32767) {
	  tmp = 32767;
	  err_up++;
	}
	if (tmp < -32767) {
	  tmp = -32767;
	  err_down++;
	}
	data[i * 2 + 1] = tmp;
      }
    }
  }
}

FFT_Encoder::FFT_Encoder(short int channels, int bytes, int window, int points) :
  WAV_Encoder(channels, bytes, window, points) { 

  fft_ = new float [window_];
  
  n_ = window / 2 + 1;
  residui_ = n_ - points_;

  power_.resize(n_);
  indx_.resize(n_);

  cerr << "Discrete Fourier Transform" << endl;
  cerr << "Approximate reduction (size): " << window / (points * 3.0) << endl;  
  cerr << "Approximate reduction (info): " << window / (points * 2.0) << endl << endl;  

  cerr << "FFTW..." << flush;
  real2complex_ = fftwf_plan_r2r_1d(window_, wave_, fft_, FFTW_R2HC, FFTW_MEASURE);
  cerr << "second..." << flush;
  complex2real_ = fftwf_plan_r2r_1d(window_, fft_, wave_, FFTW_HC2R, FFTW_MEASURE);
  cerr << "done!" << endl << endl;
}

FFT_Encoder::~FFT_Encoder() {
  fftwf_destroy_plan(complex2real_);
  fftwf_destroy_plan(real2complex_);
  delete [] fft_;
}

bool FFT_Encoder::encode_single() {

  fftwf_execute(real2complex_);

  int i;

  power_[0] = fft_[0] * fft_[0];
  for (i = 1; i < window_ / 2; i++) {
    power_[i] = fft_[i] * fft_[i] + fft_[window_ - i] * fft_[window_ - i];
  }
  if (window_ & 1 == 0)
    power_[window_ / 2] = fft_[window_ / 2] * fft_[window_ / 2];

  bool res = indexx<float>(n_, power_.begin(), indx_.begin());

  if (res) {
    if (indx_[0] <= residui_)
      fft_[0] = 0.0;
    for (i = 1; i < window_ / 2; i++) {
      if (indx_[i] <= residui_) {
	fft_[i] = 0.0;
	fft_[window_ - i] = 0.0;
      }
    }
    if (window_ & 1 == 0)
      if (indx_[window_ / 2] <= residui_)
	fft_[window_ / 2] = 0.0;

    for (i = 0; i < window_; i++)
      fft_[i] /= window_;

    fftwf_execute(complex2real_);
  }
  return res;
}

DWT_Encoder::DWT_Encoder(short int channels, int bytes, int window, int points) :
  WAV_Encoder(channels, bytes, window, points) { 

  n_ = window_;
  residui_ = n_ - points_;

  power_.resize(n_);
  indx_.resize(n_);

  cerr << "Discrete Wavelet Transform" << endl;
  cerr << "Approximate reduction (size): " << window / (points * 2.0) << endl;  
  cerr << "Approximate reduction (info): " << window / (points * 1.0) << endl << endl;  
}

bool DWT_Encoder::encode_single() {
  int i;

  wt1<float>(wave_, window_, 1, daub4);

  for (i = 0; i < window_; i++)
    power_[i] = fabsf(wave_[i]);

  bool res = indexx<float>(window_, power_.begin(), indx_.begin());

  if (res)
    for (i = 0; i < window_; i++)
      if (indx_[i] <= residui_)
	wave_[i] = 0.0;
  
  wt1<float>(wave_, window_, -1, daub4);
  return res;
}
