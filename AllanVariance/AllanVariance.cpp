#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "AllanVariance.h"
using namespace std;
AllanVariance::AllanVariance() {}

AllanVariance::AllanVariance(float* sampleData, unsigned int sampleCount, int frequence){
	mSamplesData = sampleData;
	mSamplesCount = sampleCount;
	mFrequence = frequence;
	mSamplePeriod = (float)1 / frequence;
}

void AllanVariance::init(int maxClusterCount) {
	mMaxClusterCount = maxClusterCount;
	initThetas();
	initStrides();
}

void AllanVariance::logspace(float a, float b, unsigned int n, float* results) {
	double start = pow(10, a);
	double end = pow(10, b);
	double progression = pow(end / start, (float)1 / (n - 1));
	results[0] = start;
	for (int i = 1; i < n; i++) {
		results[i] = results[i - 1] * progression;
	}
}

void AllanVariance::initThetas() {
	if (mSamplesData == NULL || mSamplesCount <= 0) {
		printf("Please input the samples data first!");
		return;
	}

	mThetas = (double*)malloc(mSamplesCount * sizeof(double));
	float sum = 0;
	for (int i = 0; i < mSamplesCount; i++) {
		sum += mSamplesData[i];
		mThetas[i] = (float)sum / mFrequence;
	}
}

void AllanVariance::initStrides() {
	if (mSamplesData == NULL || mSamplesCount <= 0) {
		printf("Please input the samples data first!");
		return;
	}

	int mode = mSamplesCount / 2;
	unsigned int maxStride = 1;
	int shft = 0;
	while (mode) {
		mode = mode >> 1;
		maxStride = 1 << shft;
		shft++;
	}

	float* averageFactors = (float*)malloc(mMaxClusterCount * sizeof(float));
	logspace(0, log10(maxStride), mMaxClusterCount, averageFactors);

	for (int i = 0; i < mMaxClusterCount; i++) {
		averageFactors[i] = ceil(averageFactors[i]);
	}

	mFactors = (int*)malloc(mMaxClusterCount * sizeof(int));

	memset(mFactors, 0, mMaxClusterCount);
	mFactorsCount = 1;
	mFactors[0] = (int)averageFactors[0];

	for (int i = 1; i < mMaxClusterCount; i++) {
		//printf("averageFactors[%d] = %f\n",i,  averageFactors[i]);
		if (averageFactors[i] != averageFactors[i - 1]) {
			mFactors[mFactorsCount] = (int) averageFactors[i];
			mFactorsCount++;
			
		}
	}

	free(averageFactors);
}

double* AllanVariance::variance() {
	mSigma2 = (double*)malloc(mFactorsCount * sizeof(double));
	
	for (int i = 0; i < mFactorsCount; i++) {
		int factor = (int)mFactors[i];
		float clusterPeriod2 = (mSamplePeriod *  factor) * (mSamplePeriod *  factor);
		float devided = 2 * clusterPeriod2 * (mSamplesCount - 2 * factor);
		int averageFactorX2 = 2 * factor;
		int max = mSamplesCount - averageFactorX2;
		mSigma2[i] = 0;
		for (int k = 0; k < max; k++) {
			float temp = (mThetas[k + averageFactorX2] - 2 * mThetas[k + factor] + mThetas[k]);
			mSigma2[i] += (temp * temp);
		}

		mSigma2[i] = mSigma2[i] / devided;
	}
	return mSigma2;
}

double* AllanVariance::deviation() {
	mSigma = (double*)malloc(mFactorsCount * sizeof(double));
	variance();

	for (int i = 0; i < mFactorsCount; i++) {
		mSigma[i] = sqrt(mSigma2[i]);
	}
	return mSigma;
}


AllanVariance::~AllanVariance() {
	if(mThetas != NULL) {
		free(mThetas);
		mThetas = NULL;
	}

	if (mFactors != NULL) {
		free(mFactors);
		mFactors = NULL;
	}

	if (mSigma2 != NULL) {
		free(mSigma2);
		mSigma2 = NULL;
	}
	
	if (mSigma != NULL) {
		free(mSigma);
		mSigma = NULL;
	}

}