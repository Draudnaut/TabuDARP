#pragma once
class Parameter
{
private:
	double alpha, beta, gamma, tao, delta,eta;
public:
	Parameter() {};
	Parameter(double _alpha, double _beta, double _gamma, double _tao, double _delta, double _eta);
	Parameter(const Parameter &p);
	double get_alpha();
	double get_beta();
	double get_gamma();
	double get_tao();
	double get_delta();
	double get_eta();
	void update(bool feasible);
};