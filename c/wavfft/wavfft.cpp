#include <popt.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <iostream>

#include "stuff.hpp"

using namespace std;

static char *input_name = NULL;
static char *output_name = NULL;
static int window = -1;
static int points = -1;
static int method = 0;

static struct poptOption optionsTable[] = {
  {"input", 'i', POPT_ARG_STRING, &input_name, 0, "Input .WAV (stdin)", "wav"},
  {"output", 'o', POPT_ARG_STRING, &output_name, 0, "Output .WAV (stdout)", "wav"},
  {"window", 'w', POPT_ARG_INT, &window, 0, "Window size (should be 2^n)", "window"},
  {"points", 'p', POPT_ARG_INT, &points, 0, "Points used (less than half window for FFT)", "points"},
  {"method", 'm', POPT_ARG_NONE, &method, 0, "force DWT (instead of FFT)", "method"},
  POPT_AUTOHELP
  {NULL, 0, 0, NULL, 0, NULL, NULL}
}; 

static void print_copyright() {
  cerr << "Wav FFT Encoder" << endl;
  cerr << "(C)opyRight AndSoft Inc., 2003-04" << endl;
  cerr << "Compiled: " << __DATE__ << ", " << __TIME__ << endl << endl;
}

int main(int argc, const char *argv[]) {
  poptContext optCon;
  FILE *in_f, *ou_f;

  print_copyright();

  optCon = poptGetContext(NULL, argc, argv, optionsTable, 0);
  while (poptGetNextOpt(optCon) >= 0);

  if (window > 0 && points > 0) {
    if (input_name) {
      in_f = fopen(input_name, "r");
      if (!in_f) cerr << input_name << ": " << strerror(errno) << endl;
    } else {
      in_f = stdin;
    }

    if (output_name) {
      ou_f = fopen(output_name, "w");
      if (!ou_f) cerr << output_name << ": " << strerror(errno) << endl;
    } else {
      ou_f = stdout;
    }

    if (in_f && ou_f)
      do_stuff(in_f, ou_f, window, points, method);

    if (ou_f) fclose(ou_f);
    if (in_f) fclose(in_f);
  } else {
    poptPrintUsage(optCon, stderr, 0);
  }

  poptFreeContext(optCon);

  return 0;
}
