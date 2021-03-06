#ifndef GRAY_CODED_BINARY_IMPLEMENTATION_H
#define GRAY_CODED_BINARY_IMPLEMENTATION_H

#include "BinaryImplementation.h"

class GrayCodedBinaryImplementation : public BinaryImplementation {
	public:
		GrayCodedBinaryImplementation();
		virtual ~GrayCodedBinaryImplementation() {};
		virtual Mat generatePattern();
		//virtual void iterationProcess();
		//virtual void postIterationsProcess();
		virtual int getBinaryCode(int, int);
		int convertGrayCodeToInteger(int, int, int);
};

#endif //GRAY_CODED_BINARY_IMPLEMENTATION_H
