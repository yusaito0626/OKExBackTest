#pragma once

#include "OKExInstrument.h"
#include "VirtualOMS.h"
#include "OKExEnums.h"

class OKExOptimizer
{
	OKExOptimizer() {};
	OKExOptimizer(const OKExOptimizer&) {};
	OKExOptimizer& operator=(const OKExOptimizer&) {};
public:
	~OKExOptimizer() {};
	void initilaize(std::map<std::string, OKExInstrument*>* _insList);

	std::map<std::string, OKExInstrument*>* insList;

	void optimize(OKExInstrument* ins);

	void test(OKExInstrument* ins);
	void arb(OKExInstrument* ins);
	void arbExeTrigger(dataOrder* exec, OKExInstrument* ins);

	void calcFactors(OKExInstrument* ins);
	void updateRings(OKExInstrument* ins);

	double calcMidMA(OKExInstrument* ins);
	double calcBookImbalance(OKExInstrument* ins);
	double calcExecImbalance(OKExInstrument* ins);
	double calcTradeQtyImbalance(OKExInstrument* ins);
	double calcSkewWgtExecImbalance(OKExInstrument* ins);

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
