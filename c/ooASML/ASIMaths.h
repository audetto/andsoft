/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

static Reference< XIdlClass > ourIDLClass;

//XMaths
OUString SAL_CALL getType( const Any& data ) throw (RuntimeException);

Sequence< Sequence< OUString > > SAL_CALL getExceptions( const Any &  ) throw (RuntimeException);

OUString SAL_CALL saveValue( const OUString & name, double value ) throw (RuntimeException);
double SAL_CALL getValue( const OUString & name ) throw (RuntimeException);

Sequence<Sequence<double> > SAL_CALL projection( const Sequence<Sequence<double> > & x, const Sequence<Sequence<double> > & a, const Sequence<Sequence<double> > & b) throw (RuntimeException);

Sequence<Sequence<double> > SAL_CALL dykstra( const Sequence<Sequence<double> > & x, const Sequence<Sequence<double> > & a, const Sequence<Sequence<double> > & b) throw (RuntimeException);

Sequence<Sequence<double> > SAL_CALL svdSolve( const Sequence<Sequence<double> > & a, const Sequence<Sequence<double> > & b) throw (RuntimeException);

Sequence<Sequence<double> > SAL_CALL fastExp( double t, const Sequence<Sequence<double> > & a, sal_Int32 method, double theta) throw (RuntimeException);

double SAL_CALL finiteDifference( const Sequence<Sequence<double> > & xa, const Sequence<Sequence<double> > & ya, double x, sal_Int32 order) throw (RuntimeException);

double SAL_CALL heston( double strike, double time1, double time2, double sigma, double kappa, double theta, double alpha, double rho, sal_Int32 points) throw (RuntimeException);

double SAL_CALL black( double strike, double time, double sigma) throw (RuntimeException);

double SAL_CALL impliedVolatility( double price, double strike, double time ) throw (RuntimeException);

Sequence<Sequence<double> > SAL_CALL variousBS( double time, double strike, double sigma) throw (RuntimeException);

Sequence<Sequence<double> > SAL_CALL fft( const Sequence<Sequence<double> > & data) throw (RuntimeException);

Sequence<Sequence<double> > SAL_CALL ifft( const Sequence<Sequence<double> > & data) throw (RuntimeException);

Sequence<Sequence<OUString> > SAL_CALL fftUnpack( const Sequence<Sequence<double> > & data) throw (RuntimeException);

Sequence<Sequence<double> > SAL_CALL fftPack( const Sequence<Sequence<OUString> > & data) throw (RuntimeException);

Sequence<Sequence<double> > SAL_CALL hestonFFT( double time1, double time2, double sigma, double kappa, double theta, double alpha, double rho, sal_Int32 N, double stdDev) throw (RuntimeException);

Sequence<Sequence<OUString > > SAL_CALL regExp( const OUString& regexp, const OUString& text ) throw (RuntimeException);

OUString SAL_CALL createMarketData( const OUString& name, double date, const Sequence< Sequence< OUString > >& ccys, const Sequence< Sequence< double > >& rates, const Sequence< Sequence< OUString > >& stocks, const Sequence< Sequence< double > >& spots, const Sequence< Sequence< OUString > >& domestics, const Sequence< Sequence< double > >& repos, const Sequence< Sequence< double > >& vols, const Sequence< Sequence< double > >& correlations ) throw (RuntimeException);

OUString SAL_CALL createPayoff( const OUString& name, const Sequence< Sequence< OUString > >& includes, const OUString& filename ) throw (RuntimeException);

Sequence<Sequence<double> > SAL_CALL mcPricer( const OUString& ooPayoff, const OUString & ooCcy, const Sequence< Sequence< OUString > >& ooNames, const OUString & ooMarketData, const Sequence< Sequence< Any > >& details ) throw (RuntimeException);

OUString SAL_CALL createEventPayoff( const OUString& name, const Sequence< Sequence< OUString > >& includes, const Sequence< Sequence< double > >& dates, const Sequence< Sequence< OUString > >& events ) throw (RuntimeException);
