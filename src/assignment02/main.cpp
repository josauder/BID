#include "../framework/kinect.hpp"
#include "../assignment01/touchRecognizer.hpp"
#include "classifier.hpp"
#include "digitVisualizer.hpp"

#include <opencv2/opencv.hpp>
#include "data.hpp"

#include <unordered_set>




void addElementIfNew(std::list<std::unordered_set<int>> &equivalenceTable, int newElement) {
	for (auto set : equivalenceTable) {
		if (set.count(newElement)) {
			return;
		}
	}
	std::unordered_set<int> newSet;
	newSet.insert(newElement);
	equivalenceTable.push_back(newSet);
}

void merge(std::list<std::unordered_set<int>> &equivalenceTable, int a, int b) {
	auto firstSet = equivalenceTable.begin();
	auto lastSet = equivalenceTable.begin();
	for (auto iterator = equivalenceTable.begin(); iterator != equivalenceTable.end(); iterator++) {
		if (iterator->count(a)) {
			firstSet = iterator;
			//std::cout << "a found";
		}
		if (iterator->count(b)) {
			lastSet = iterator;
			//std::cout << " b found\n";
		}
	}
	if (firstSet == lastSet) {
		//std::cout << "Same set, no merging needed" << std::endl;
		return;
	}
	std::cout << "merging " << a << "with" << b << std::endl;
	for (auto element : (*lastSet)) {
		firstSet->insert(element);
	}
	equivalenceTable.erase(lastSet);
}


int playground() {
	cv::Mat img = cv::imread("test.png", CV_LOAD_IMAGE_GRAYSCALE);
	if (!img.data)                              // Check for invalid input
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	std::vector<char> mask = std::vector<char>((img.rows + 1) * (img.cols + 1), 0);
	std::list<std::unordered_set<int>> equTable;

	int blobCounter = 1;
	for (int y = 0; y < img.rows; y++) {
		for (int x = 0; x < img.cols; x++) {
			int value = img.at<std::uint8_t>(y, x);
			if (value) {
				std::vector<int> connectedValues;
				int temp = 0;
				temp = mask[img.cols * y + x];
				if(temp) {
					connectedValues.push_back(temp);
				}
				temp = mask[img.cols * y + x + 1];
				if (temp) {
					connectedValues.push_back(temp);
				}
				temp = mask[img.cols * (y + 1) + x];
				if (temp) {
					connectedValues.push_back(temp);
				}
				if (connectedValues.size()) {
					for (int i = 1; i < connectedValues.size(); i++) {
						merge(equTable, connectedValues[0], connectedValues[i]);
					}
					mask[img.cols * (y + 1) + x + 1] = connectedValues[0];
				}
				else {
					addElementIfNew(equTable, blobCounter);
					mask[img.cols * (y + 1) + x + 1] = blobCounter;
					blobCounter++;
				}
			}
		}
		std::cout << "Y:" << y << std::endl;
	}

	for (int y = 0; y < img.rows; y++) {
		for (int x = 0; x < img.cols; x++) {
			int i = 1;
			bool flag = false;
			for (auto set : equTable) {
				int temp = mask[img.cols * (y + 1) + x + 1];
				if (set.count(temp)) {
					flag = true;
					break;
				}
				i++;
			}
			if (flag)
				img.at<std::uint8_t>(y, x) = i * 30;
			else
				img.at<std::uint8_t>(y, x) = 0;
		}
	}

	cv::namedWindow("Playground",CV_WINDOW_AUTOSIZE);
	cv::imshow("Playground", img);

	return 0;
}

int main()
{
    // initialize framework
    Kinect::initialize();
    Kinect::listDevices();

    // set up device
    Kinect kinect;
    if(!kinect.ready())
    {
        std::cout << "Could not set up Kinect. Exiting." << std::endl;
        return -1;
    }

    // prepare touch pad
    TouchRecognizer touchRecognizer;

    // load data set
    const auto data = DataSet::read("pendigits.tra");

    // prepare variables
    cv::Mat depthFrame;
    cv::Mat colorFrame;
    cv::Mat outputFrame;
    auto running = true;
    std::vector<cv::Point2f> path;

    // calibrate
    kinect.getDepthFrame(depthFrame);
    touchRecognizer.calibrate(depthFrame);

    // prepare classifier and viewer
    Classifier classifier(data, 25);
    DigitVisualizer digitVisualizer;

    // prepare windows - this isn't necessary, but it allows to assign useful positions
    cv::namedWindow("color");
    cv::namedWindow("depth");
    cv::namedWindow("output");

    // move windows
    kinect.getColorFrame(colorFrame);
    const auto xOffset = colorFrame.cols;
    const auto yOffset = colorFrame.rows;
    cv::moveWindow("color", 0 * xOffset, 0 * yOffset);
    cv::moveWindow("depth", 1 * xOffset, 0 * yOffset);
    cv::moveWindow("output", 0 * xOffset, 1 * yOffset);

    while(running)
    {
		playground();
        // update frames
        kinect.getDepthFrame(depthFrame);
        kinect.getColorFrame(colorFrame);

        // run touch recognizer
        auto positions = touchRecognizer.recognize(depthFrame);
        if(!positions.empty())
		{
            // track user's drawing using the path variable
			for (cv::RotatedRect rect : positions) {
				path.push_back(rect.center);
			}
            /*~~~~~~~~~~~*
             * YOUR CODE *
             * GOES HERE *
             *~~~~~~~~~~~*/
		} 
        else
		{
			if (!path.empty()) {
				std::cout << "classifying" << std::endl;
				// done drawing -> classify

				/*~~~~~~~~~~~*
				 * YOUR CODE *
				 * GOES HERE *
				 *~~~~~~~~~~~*/

				 // after classifying, you should update the visualizer like this:
				classifier.classify(path);
				path.clear();
				digitVisualizer.setSimplifiedPath(classifier.getSimplifiedPath());
			    digitVisualizer.setResult(classifier.getResult());
			}
		}

        // visualize results
        digitVisualizer.draw(colorFrame, path, outputFrame);

        // show frames
        auto depthFrameUnscaled = depthFrame.clone();
        //cv::imshow("depth", depthFrame);
        cv::imshow("color", colorFrame);
        cv::imshow("output", outputFrame);

        // check for keypresses
        const auto key = cv::waitKey(10);
        switch (key)
        {
		case 0x1b: // esc
            running = false;
            break;
		case 0x0d: // enter
            touchRecognizer.calibrate(depthFrame);
            break;
		case 0x30: // number keys - you can use this to take a look at the data set
		case 0x31:
		case 0x32:
		case 0x33:
		case 0x34:
		case 0x35:
		case 0x36:
		case 0x37:
		case 0x38:
		case 0x39: 
            DataVisualizer::showNumber(data, key - 0x30);
            break;
        default:
            break;
        }
    }
}
