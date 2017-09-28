#pragma once
#ifndef ALLAN_VARIANCE_H
#define ALLAN_VARIANCE_H
class AllanVariance {
public:
	AllanVariance();
	AllanVariance(float* sampleData, unsigned int sampleCount, int frequence);

	void init(int);

	void setSampleData(float*, unsigned int);
	
	void initStrides();

	void initThetas();

	void logspace(float a, float b, unsigned int n, float* results);


	unsigned int getFactorsCount() {
		return mFactorsCount;
	}

	double* variance();
	double* deviation();
	void getAllanVarice();

	~AllanVariance();

private:
	int mFrequence;
	unsigned int mSamplesCount;
	float* mSamplesData;
	float mSamplePeriod;

	double* mThetas;

	unsigned int mMaxClusterCount;
	unsigned int mFactorsCount;
	int* mFactors;

	double* mSigma2;
	double* mSigma;

};

#endif