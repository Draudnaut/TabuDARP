#pragma once
class Parameter
{
private:
	double alpha, beta, gamma, tao, delta,eta,theta;
	int ke=15;
public:
	Parameter() {};
	Parameter(double _alpha, double _beta, double _gamma, double _tao, double _delta, double _eta, double _theta,int _ke);
	Parameter(const Parameter &p);
	double get_alpha();
	double get_beta();
	double get_gamma();
	double get_tao();
	double get_delta();
	double get_eta();
	int get_ke();
	int get_theta();
	void update(bool feasible);
	void output() { std::cout << alpha << " " << beta << " " << gamma << " " << tao << std::endl; }
};