#include "classifier.hpp"
#include "data.hpp"
#include <utility>
#include <vector>
#include <algorithm>

Classifier::Classifier(std::vector<Digit> dataSet, const int k)
: c_dataSet(std::move(dataSet))
, c_simplifiedSize(c_dataSet.front().points().size())
, c_k(k)
{
	for (int i = 0; i < KNN_K; i++) {
		knneighbors[i].distance = DBL_MAX;
		knneighbors[i].label = -1;
	}
}

double compare(std::vector<cv::Point2i> points, Digit digit) {
	double distance = 0;
	for (int i = 0; i < digit.points().size(); i++) {
		distance += cv::norm(points[i] - digit.points()[i]);
	}
	
	return distance;
}


void Classifier::updateKNN(Neighbor n) {
	int max_index = KNN_K;
	double max_distance = -1;
	bool shouldSwap = false;

	for (int i = 0; i < KNN_K; i++) {
		if (n.distance < knneighbors[i].distance) {
			shouldSwap = true;
		}
		if (knneighbors[i].distance > max_distance) {
			max_index = i;
			max_distance = knneighbors[i].distance;
		}
	}
	if (shouldSwap) {
		knneighbors[max_index] = n;
	}
}

void Classifier::classify(const std::vector<cv::Point2f>& path)
{
    // equidistant sampling
    simplify(path);
	
	std::vector<cv::Point2i> normalizedPath;
	cv::Rect boundingBox = cv::boundingRect(m_simplifiedPath);
	std::cout << "x:" << boundingBox.x << " y:" << boundingBox.y << " height:" << boundingBox.height << "width:" << boundingBox.width << std::endl;
	float scaling_x = 100.0 / boundingBox.width;
	float scaling_y = 100.0 / boundingBox.height;

	for (int i = 0; i < m_simplifiedPath.size(); i++) {
		normalizedPath.push_back(m_simplifiedPath[i]);
		normalizedPath[i].x = m_simplifiedPath[i].x - boundingBox.x;
		normalizedPath[i].x = m_simplifiedPath[i].x * scaling_x;
		normalizedPath[i].y = m_simplifiedPath[i].y - boundingBox.y;
		normalizedPath[i].y = m_simplifiedPath[i].y * scaling_y;
		normalizedPath[i].y = 100 - normalizedPath[i].y;
	}


	/*for (auto p : m_simplifiedPath) {
		std::cout << "simplifiedPoint x:" << p.x << " y:" << p.y << std::endl;
	}*/

	for (auto p : c_dataSet[0].points()) {
		std::cout << "Learning data set Point x:" << p.x << " y:" << p.y << std::endl;
	}

	for (auto digit : c_dataSet) {
		double distance = compare(normalizedPath, digit);
		Neighbor neighbor = { distance, digit.label() };
		updateKNN(neighbor);
	}

	int labels[10] = { 0 };
	for (int i = 0; i < KNN_K; i++) {
		labels[knneighbors[i].label] += 1;
	}
	int max = -1;
	for (int i = 0; i < 10; i++) {
		if (labels[i] > max) {
			max = labels[i];
			m_result = i;
		}
	}
    // normalize and mirror y

    /*~~~~~~~~~~~*
     * YOUR CODE *
     * GOES HERE *
     *~~~~~~~~~~~*/

    // match using knn

    /*~~~~~~~~~~~*
     * YOUR CODE *
     * GOES HERE *
     *~~~~~~~~~~~*/

    // you should store your result in m_result
    // m_result = ...
}

int Classifier::getResult() const
{
    return m_result;
}

std::vector<cv::Point2f> Classifier::getSimplifiedPath() const
{
    return m_simplifiedPath;
}

void Classifier::simplify(std::vector<cv::Point2f> path)
{
	m_simplifiedPath.clear();
	double pathLength = 0;
	cv::Point2f lastPoint = path.at(0);

	for (cv::Point2f point : path) {
		pathLength += cv::norm(point - lastPoint);
		lastPoint = point;
	}
	std::cout << "pathLength:" << pathLength << std::endl;
	double averageDistance = pathLength / (c_simplifiedSize - 1);
	std::cout << "averageDistance:" << averageDistance << std::endl;

	lastPoint = path.at(0);
	double pathPartLength = 0;
	m_simplifiedPath.push_back(path[0]);
	for (cv::Point2f point : path) {
		pathPartLength += cv::norm(point - lastPoint);
		if (pathPartLength > averageDistance) {
			m_simplifiedPath.push_back(point);
			pathPartLength = pathPartLength - averageDistance;
		}
		lastPoint = point;
	}
	m_simplifiedPath.push_back(path.at(path.size() - 1));
	std::cout << "m_simplifiedPath.size():" << m_simplifiedPath.size() << std::endl;
	
    // sample path with equal spacing and store in m_simplifiedPath
    // you should use c_simplifiedSize as number of points in simplified path

    /*~~~~~~~~~~~*
     * YOUR CODE *
     * GOES HERE *
     *~~~~~~~~~~~*/
}
