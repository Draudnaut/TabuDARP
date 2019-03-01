#include "pch.h"
#include "parameter.h"

Parameter::Parameter(double _alpha, double _beta, double _gamma, double _tao, double _delta, double _eta)
{
	alpha = _alpha;
	beta = _beta;
	gamma = _gamma;
	tao = _tao;
	delta = _delta;
	eta = _eta;
}

Parameter::Parameter(const Parameter & p)
{
	alpha = p.alpha;
	beta = p.beta;
	gamma = p.gamma;
	tao = p.tao;
	delta = p.delta;
	eta = p.eta;
}

double Parameter::get_alpha()
{
	return alpha;
}

double Parameter::get_beta()
{
	return beta;
}

double Parameter::get_gamma()
{
	return gamma;
}

double Parameter::get_tao()
{
	return tao;
}

double Parameter::get_delta()
{
	return delta;
}

double Parameter::get_eta()
{
	return eta;
}

void Parameter::update(bool feasible)
{
	if (feasible)
	{
		alpha /= (1 + delta);
		beta /= (1 + delta);
		gamma /= (1 + delta);
		tao /= (1 + delta);
	}
	else
	{
		alpha *= (1 + delta);
		beta *= (1 + delta);
		gamma *= (1 + delta);
		tao *= (1 + delta);
	}
}
