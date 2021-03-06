#include "GrayCodedBinaryImplementation.h"

GrayCodedBinaryImplementation::GrayCodedBinaryImplementation() {
	setIdentifier(string("GrayCodedBinaryImplementation"));
}

Mat GrayCodedBinaryImplementation::generatePattern() {
	Mat pattern;
	Scalar colour;
	Size cameraResolution = experiment->getInfrastructure()->getCameraResolution();
	int cameraWidth = (int)cameraResolution.width;
	int cameraHeight = (int)cameraResolution.height;


	generateBackground(pattern,colour);

	int columnWidth = cameraWidth / numberColumns;
	int doubleColumnWidth = columnWidth * 2;
	int xPos = 0;

	for (int columnIndex = 0; columnIndex < numberColumns; columnIndex++) {
		if (columnIndex % 4 == 1) {
			rectangle(pattern, Point(xPos, 0), Point((xPos + doubleColumnWidth) - 1, cameraHeight), colour, FILLED);
		}
		xPos += columnWidth;
	}

	return pattern;
}

// Getters and Setters
int GrayCodedBinaryImplementation::getBinaryCode(int x, int y) {
    Rect croppedArea = experiment->getInfrastructure()->getCroppedArea();
    int binCode = this->binaryCode[(y * croppedArea.width) + x];
    if(binCode == -1) return -1;
    return convertGrayCodeToInteger(binCode, numberColumns, getNumberPatterns());
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

int GrayCodedBinaryImplementation::convertGrayCodeToInteger(int grayCodeToConvert, int numberColumns, int numberPatterns) {
	int result = grayCodeToConvert;

	for (int iteration = numberColumns; iteration > 0; iteration /= 2) {
		if (iteration < numberPatterns) {
			result = result ^ (result >> iteration);
		}
	}

	return result;
}
