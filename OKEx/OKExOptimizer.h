#pragma once
class OKExOptimizer
{
	OKExOptimizer() {};
	OKExOptimizer(const OKExOptimizer&) {};
	OKExOptimizer& operator=(const OKExOptimizer&) {};
public:
	~OKExOptimizer();



	static OKExOptimizer* getInstance(void)
	{
		static OKExOptimizer* singleton;
		if (!singleton)
		{
			singleton = new OKExOptimizer();
		}
		return singleton;
	}
};


extern OKExOptimizer* optimizer;
