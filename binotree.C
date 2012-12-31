#include <iostream>
#include <iomanip> // this gives us setw 
#include <vector>
#include <cmath> // <x> means look in the preinstalled system 

#include "bs.h" // "x" means look in the current directory

using namespace std;


class payofffunc
{
public:
	virtual double calcpayoff(double s) = 0;
};


class callpayoff : public payofffunc
{
public:
	double calcpayoff(double s) 
	{ 
		return max(0., s-K); 
	}
	callpayoff(double K_) : K(K_) {  } // constructor
	// constructor gets called whenever we create an object of this class 
	// constructor has the same name as the class i.e. callpayoff
	// K(K_) is a data member initializer, saves the strike price (also known as the environment of the function) in the
	// private data member K
private:
	double K;
};

class digcallpayoff : public payofffunc
{
public:
	double calcpayoff(double s) 
	{ 
		return s > K ? 1. : 0.; // conditional operator, very compact if-then, meaning if s>K return 1 otherwise return 0
	}
	digcallpayoff(double K_) : K(K_) {  } // constructor
	// constructor gets called whenever we create an object of this class 
	// constructor has the same name as the class i.e. callpayoff
	// K(K_) is a data member initializer, saves the strike price (also known as the environment of the function) in the
	// private data member K
private:
	double K;
};






double priceoption(payofffunc &payoff, double r, double T, double sig, double S0, int n)
{
	double dt	= T/n;
	double u	= exp(sig*sqrt(dt));
	double d	= 1./u;
	double p	= (exp(dt*r)-d)/(u-d);

	vector<vector<double> > stocktree(n, vector<double>(n, 0.));
	vector<vector<double> > euro(n, vector<double>(n, 0.));

	stocktree[0][0] = S0;
	for(int t = 1; t < n; t++)
	{
		stocktree[0][t] = u * stocktree[0][t-1];
		for(int row = 1; row <= t; row++)
		{
			stocktree[row][t] = d*stocktree[row-1][t-1];
		}
	}


	for(int row = 0; row < n; row++)
	{
		euro[row][n-1] = payoff.calcpayoff(stocktree[row][n-1]);
	}

	for(int t = n-2; t >= 0; t--)
	{
		for(int row = 0; row <= t; row++)
		{
			euro[row][t] = exp(-dt*r) *(p*euro[row][t+1]
			   +(1.-p)*euro[row+1][t+1]);
		}
	}
	

	
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			// setw(8) means leave exactly 8 spaces to
			// print the variable, and pad with extra
			// spaces if necessary.  this helps align
			// the columns in our table
			//cout << setw(8) << stocktree[i][j] << ",";
			//cout << setw(8) << euro[i][j] << ",";
		}
		//cout << endl;
	}

	return euro[0][0];
}

int main()
{
	callpayoff atm(100.), dim(90.), ffm(110.);
	digcallpayoff  datm(100.), ddim(90.), dffm(110.);
	cout << "atm call=" << priceoption(atm, .05, 1., .2, 100., 50) << endl;
	cout << "dim call=" << priceoption(dim, .05, 1., .2, 100., 50) << endl;
	cout << "ffm call=" << priceoption(ffm, .05, 1., .2, 100., 50) << endl;
	cout << "datm call=" << priceoption(datm, .05, 1., .2, 100., 50) << endl;
	cout << "ddim call=" << priceoption(ddim, .05, 1., .2, 100., 50) << endl;
	cout << "dffm call=" << priceoption(dffm, .05, 1., .2, 100., 50) << endl;
	cout << "datm call=" << priceoption(datm, .05, 1., .2, 100., 50)<< endl;
	cout << "bs atmeurocall=" << bscall(.05, 1., .2, 100., 100.) << endl;
	return 0;
}

