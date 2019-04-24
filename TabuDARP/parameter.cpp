#include "pch.h"
#include "parameter.h"

Parameter::Parameter(double _alpha, double _beta, double _gamma, double _tao, double _delta, double _eta, double _theta, int _ke)
{
	alpha = _alpha;
	beta = _beta;
	gamma = _gamma;
	tao = _tao;
	delta = _delta;
	eta = _eta;
	ke = _ke;
	theta = _theta;
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

int Parameter::get_ke()
{
	return ke;
}

int Parameter::get_theta()
{
	return (int)floor(theta);
}

void Parameter::update(bool feasible)
{
	if (feasible)
	{
		//printf("update feasible\n");
		if (alpha == 0 or beta == 0 or gamma == 0 or tao == 0) return;
		alpha /= (1 + delta);
		beta /= (1 + delta);
		gamma /= (1 + delta);
		tao /= (1 + delta);
	}
	else
	{
		//printf("update infeasible\n");
		alpha *= (1 + delta);
		beta *= (1 + delta);
		gamma *= (1 + delta);
		tao *= (1 + delta);
	}
}
