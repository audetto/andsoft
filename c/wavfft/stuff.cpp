#include "stuff.hpp"
#include "encoder.hpp"

#include "cycle.h"

#include <iomanip>
#include <iostream>

using namespace std;

struct WAV {
  char riff[4];  // "RIFF"
  int  size;     // remaining length
  char wave[8];  // "WAVfmt "
  char boh1[6];
  short int channels;
  int rate;
  int bps;
  short int align;
  short int bits;
  char data[4];  // "data"  
  int  length;
} __attribute__ ((packed));

void do_stuff(FILE *in, FILE *out, int window, int points, int method) {

  WAV header;

  size_t res;
  int letti = 0;
  int bytes;
  char *p;
  size_t size;

  ticks t0, t1;
  double p1, st;

  res = fread(&header, sizeof(header), 1, in);

  int samples = header.length / (header.channels * header.bits / 8);
  int transform = int(samples / window);

  res = fwrite(&header, sizeof(header), 1, out);

  bytes = header.bits / 8;

  size = window * header.channels * bytes;
  p = new char [size]; 

  WAV_Encoder *encoder;

  t0 = getticks();
  if (method == 1)
    encoder = new DWT_Encoder(header.channels, bytes, window, points);
  else
    encoder = new FFT_Encoder(header.channels, bytes, window, points);
  t1 = getticks();

  p1 = double(t1 - t0);

  cerr << "Total points:  " << setw(10) << samples << endl;
  cerr << "Total windows: " << setw(10) << transform << endl << endl;

  t0 = getticks();
  do {
    res = fread(p, 1, size, in);
    if (res == size) {
      encoder->encode(p); 
    }
    fwrite(p, 1, res, out);
    letti += res;
    cerr << "Processed:     " << setw(10) << letti / (header.channels * header.bits / 8);
    cerr << " Up: " << setw(10) << encoder->err_up << " Down: " << setw(10) << encoder->err_down << '\r';
  } while (letti < header.length);
  t1 = getticks();
  st = double(t1 - t0);
  cerr << endl << endl;

  cerr << "Init:   " << p1 << endl;
  cerr << "Suff:   " << st << endl;
  cerr << "Total:  " << p1 + st << endl;

  delete encoder;
  delete [] p;

}
