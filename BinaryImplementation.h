#ifndef BINARY_IMPLEMENTATION_H
#define BINARY_IMPLEMENTATION_H

#include "slBenchmark.h"

#define BINARY_NUM_PATTERNS 6

#define BINARY_BLACK_VAL 0
#define BINARY_WHITE_VAL 195

#define BINARY_WHITE_THRESHOLD 50
#define BINARY_BLACK_THRESHOLD -50

#define BINARY_Z_SCALE 10000

using namespace cv;

class BinaryImplementation : public slImplementation {
	public:
		BinaryImplementation();
		virtual ~BinaryImplementation() {};
		void preExperimentRun();
		void postExperimentRun();
		bool hasMoreIterations();
		virtual Mat generatePattern();
		virtual void iterationProcess();
		virtual void postIterationsProcess();

	protected:
		int numberColumns;
		int *binaryCode;
};

#endif //BINARY_IMPLEMENTATION_H
