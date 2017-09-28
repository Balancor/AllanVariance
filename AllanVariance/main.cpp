#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <iostream>
#include <vector>

#include "AllanVariance.h"

#define RAW_DATA_FILE_PATH "samples.txt"

using namespace std;

unsigned int frequence = 50;


void readDataFromFile(const char* fileName, vector<float>* rawData) {

	FILE* fp = fopen(fileName, "r+");
	if (fp == NULL)return;

	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	printf("fileSize: %d\n", fileSize);
	
	char* fileData = (char*)malloc(fileSize * sizeof(char));

	int readCount = 0;
	while (fileSize - readCount > 0) {
		if (fileSize - readCount > 1024) {
			int count = fread(fileData + readCount, 1, 1024, fp);
			if (count != 1024) {
				printf("Cannot read  1024 bytes data from %s\n", fileName);
				return;
			}
			readCount += 1024;
		} else {
			int reservedCount = fileSize - readCount;
			int count = fread(fileData + readCount, 1, reservedCount, fp);
			printf("count: %d, reservedCount: %d\n", count, reservedCount);
			if (count != reservedCount) {
				printf("Cannot read %d bytes data from %s\n", reservedCount, fileName);
				return;
			}
			readCount += count;
		}
	}
	


	float value = 0.0f;
	char* floatStr  = fileData;
	for (int i = 0; i < fileSize - 1; i++)	{
		char temp = fileData[i];
		if (temp == '\n' || temp == ' ')continue;
		if (temp == ',') {
			value = atof(floatStr);
			rawData->push_back(value);
			floatStr = &fileData[i + 1];
		}
	}
	printf("rawData size: %d\n", rawData->size());
	free(fileData);
	fclose(fp);

	
}

int main() 
{
	vector<float> rawData;
	readDataFromFile(RAW_DATA_FILE_PATH, &rawData);
	if (rawData.empty()) {
		printf("Cannot read the data from the file: %s\n", RAW_DATA_FILE_PATH);
		return -1;
	}

	AllanVariance* allanVariance = new AllanVariance(&rawData[0],  rawData.size(), frequence);

	allanVariance->init(10000);

	int count = allanVariance->getFactorsCount();
	double* variances = allanVariance->variance();

	for (int i = 0; i < count; i++) {
		printf("Variances[%d] = %.08f\n", i, variances[i]);
	}

	delete allanVariance;
	rawData.clear();
	rawData.shrink_to_fit();
	return 0;
}