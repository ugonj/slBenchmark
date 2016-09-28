#include "GrayCodedBinaryImplementation.h"

GrayCodedBinaryImplementation::GrayCodedBinaryImplementation() {
	setIdentifier(string("GrayCodedBinaryImplementation"));
}

Mat GrayCodedBinaryImplementation::generatePattern() {
	Size cameraResolution = experiment->getInfrastructure()->getCameraResolution();

	int iterationIndex = experiment->getIterationIndex();

	int screenWidth = (int)cameraResolution.width;
	int screenHeight = (int)cameraResolution.height;

	Mat pattern(screenHeight, screenWidth, CV_8UC3, Scalar(BINARY_WHITE_VAL, BINARY_WHITE_VAL, BINARY_WHITE_VAL));
	Scalar colour(BINARY_BLACK_VAL, BINARY_BLACK_VAL, BINARY_BLACK_VAL);

	if (iterationIndex % 2 == 0) {
		numberColumns *= 2;
	} else {
		pattern.setTo(Scalar(BINARY_BLACK_VAL, BINARY_BLACK_VAL, BINARY_BLACK_VAL));
		colour = Scalar(BINARY_WHITE_VAL, BINARY_WHITE_VAL, BINARY_WHITE_VAL);
	}

	int columnWidth = screenWidth / numberColumns;
	int doubleColumnWidth = columnWidth * 2;
	int xPos = 0;

	for (int columnIndex = 0; columnIndex < numberColumns; columnIndex++) {
		if (columnIndex % 4 == 1) {
			rectangle(pattern, Point(xPos, 0), Point((xPos + doubleColumnWidth) - 1, screenHeight), colour, FILLED);
		}
		xPos += columnWidth;
	}

	return pattern;
}
/* FOR ADAPTIVE!!!!
void GrayCodedBinaryImplementation::iterationProcess() {
	if (benchmark->getIterationIndex() % 2 != 0) {

		Mat positiveMat = getCaptureAt(getNumberCaptures() - 2);
		Mat negativeMat = getLastCapture();

		for (int y = 0; y < benchmark->getCameraHeight(); y++) {
			int previousColumnEdgeIndex = -1;

			for (int x = 0; x < benchmark->getCameraWidth(); x++) {
				Vec3b positivePixelBGR = positiveMat.at<Vec3b>(y, x);
				Vec3b negativePixelBGR = negativeMat.at<Vec3b>(y, x);

				int positiveColourTotal = (int)positivePixelBGR[0] + (int)positivePixelBGR[1] + (int)positivePixelBGR[2];
				int negativeColourTotal = (int)negativePixelBGR[0] + (int)negativePixelBGR[1] + (int)negativePixelBGR[2];

				int colourDifference = positiveColourTotal - negativeColourTotal;

				int arrayOffset = (y * benchmark->getCameraWidth()) + x;

				binaryCode[arrayOffset] <<= 1;

				if (colourDifference < BLACK_THRESHOLD) {
					binaryCode[arrayOffset] += 1;
				} else if (colourDifference > WHITE_THRESHOLD) {
					binaryCode[arrayOffset] += 0;
                                } else {
					binaryCode[arrayOffset] = -1;
                                }			

				if (binaryCode[arrayOffset] != -1) {	
					int columnEdgeIndex = convertBinaryToInteger(binaryCode[arrayOffset], numberColumns, NUM_PATTERNS);

					if (columnEdgeIndex == previousColumnEdgeIndex + 1) {
					}

					previousColumnEgdeIndex = columnEdgeIndex;
				}

			}			
		}	
	}
}
*/

void GrayCodedBinaryImplementation::postIterationsProcess() {
	Rect croppedArea = experiment->getInfrastructure()->getCroppedArea();

	for (int y = 0; y < croppedArea.height; y++) {
		int lastBinaryCode = -1;

		for (int x = 0; x < croppedArea.width; x++) {
			double xCamera = (double)x / (double)croppedArea.width;
			int currentBinaryCode = binaryCode[(y * croppedArea.width) + x];

			if (currentBinaryCode != -1 && currentBinaryCode != lastBinaryCode) {
				
				double xPattern = ((double)convertGrayCodeToInteger(currentBinaryCode, numberColumns, BINARY_NUM_PATTERNS) / (double)numberColumns);
				double displacement = xCamera - xPattern;
				double z = displacement * BINARY_Z_SCALE;

				slDepthExperimentResult result(x, y, displacement * BINARY_Z_SCALE);
				experiment->storeResult(&result);

				lastBinaryCode = currentBinaryCode;
			}
		}
	}
}

int GrayCodedBinaryImplementation::convertGrayCodeToInteger(int grayCodeToConvert, int numberColumns, int numberPatterns) {
	int result = grayCodeToConvert;

	for (int iteration = numberColumns; iteration > 0; iteration /= 2) {
		if (iteration < numberPatterns) {
			result = result ^ (result >> iteration);
		}
	}

	return result;
}
