#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <fftw3.h>
#include <vector>

using namespace std;

class WAV_Encoder {
protected:
  short int channels_;
  int bytes_;
  int window_;
  int points_;
  float *wave_;
  
  virtual bool encode_single() = 0;
public:
  int err_up;
  int err_down;

  WAV_Encoder(short int channels, int bytes, int window, int points);
  virtual ~WAV_Encoder();
  void encode(char p[]);
};

class FFT_Encoder : public WAV_Encoder {
private:
  fftwf_plan real2complex_, complex2real_;
  int n_;
  int residui_;
  vector<float> power_;
  vector<int> indx_;
  float *fft_;
protected:
  virtual bool encode_single();
public:
  FFT_Encoder(short int channels, int bytes, int window, int points);
  virtual ~FFT_Encoder();
};

class DWT_Encoder : public WAV_Encoder {
private:
  int n_;
  int residui_;
  vector<float> power_;
  vector<int> indx_;
protected:
  virtual bool encode_single();
public:
  DWT_Encoder(short int channels, int bytes, int window, int points);
};

#endif
