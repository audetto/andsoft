#include <asi/utils.h>
#include <stdexcept>

using namespace std;

namespace ASI
{

    void MatrixDeleter::operator()(gsl_matrix * matrix) const
    {
	if (matrix)
	    gsl_matrix_free(matrix);
    }

    void VectorDeleter::operator()(gsl_vector * vector) const
    {
	if (vector)
	    gsl_vector_free(vector);
    }

    void error(const string & msg)
    {
	printf("Something bad happened: %s\n", msg.c_str());
	throw std::runtime_error(msg);
    }

    void handler (const char * reason, const char * file, int line, int gsl_errno)
    {
	printf("GSL ERROR: %s (%s:%d)\n", reason, file, line);
	error(reason);
    }

    const cpl I(0.0, 1.0);
}
