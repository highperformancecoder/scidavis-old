#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <math.h>

struct data {
	size_t n;
	size_t p;
	double * X;
	double * Y;
	double * sigma;
};

char * name() { return "ExponentialSaturation"; }

char * function() { return "b1*(1-exp(-b2*x))"; }

char * parameters() { return "b1,b2"; }

double function_eval(double x, double * params)
{
	return params[0]*(1-exp(-params[1]*x));
}

int function_f(const gsl_vector * params, void * void_data, gsl_vector * f)
{
	struct data * d = (struct data*) void_data;
	double b1 = gsl_vector_get(params, 0);
	double b2 = gsl_vector_get(params, 1);
	size_t i;
	for (i=0; i<d->n; i++)
		gsl_vector_set(f, i, (b1*(1-exp(-b2*d->X[i])) - d->Y[i])/d->sigma[i]);
	return GSL_SUCCESS;
}

int function_df(const gsl_vector * params, void * void_data, gsl_matrix *J)
{
	struct data * d = (struct data*) void_data;
	double b1 = gsl_vector_get(params, 0);
	double b2 = gsl_vector_get(params, 1);
	size_t i;
	for (i=0; i<d->n; i++) {
		double x = d->X[i];
		gsl_matrix_set(J, i, 0, (1-exp(-b2*x)));
		gsl_matrix_set(J, i, 1, b1*x*exp(-b2*x));
	}
	return GSL_SUCCESS;
}

int function_fdf(const gsl_vector * params, void * void_data, gsl_vector * f, gsl_matrix * J)
{
	function_f(params, void_data, f);
	function_df(params, void_data, J);
	return GSL_SUCCESS;
}

double function_d(const gsl_vector * params, void * void_data)
{
	struct data * d = (struct data*) void_data;
	gsl_vector * f = gsl_vector_alloc(d->n);
	double result = 0;
	size_t i;

	function_f(params, void_data, f);
	for (i=0; i<d->n; i++)
		result += pow(gsl_vector_get(f, i), 2);

	gsl_vector_free(f);
	return result;
}

