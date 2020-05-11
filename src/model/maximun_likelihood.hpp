#include <opencv2/opencv.hpp>
#include <math.h>
float pi = 3.1415926;
struct vector3f
{
    float x;
    float y;
    float z;
};

/*
Usage:
Dim2NormDistributionMLModel model;
model.training(...);
model.inference(...);
*/

class Dim2NormDistributionMLModel
{
private:
    static const uint32_t dim = 3;
    struct modelParam
    {
        // likelihood Model
        cv::Mat mean = cv::Mat::ones(dim, 1, CV_32FC1);
        cv::Mat squre = cv::Mat::eye(dim, dim, CV_32FC1);
        // prior Model
        float lamda{};
    };
    modelParam likelihoodModelZero;
    modelParam likelihoodModelOne;
    float inference(const cv::Mat &inputX, const modelParam &model)
    {
        const cv::Mat &mean = model.mean;
        const cv::Mat &squre = model.squre;
        float expParam = -1 * cv::determinant((inputX - mean).t() * squre * (inputX - mean));
        float ratio = 1 / cv::sqrt(cv::pow(2 * pi, dim) * determinant(mean));
        return ratio * cv::exp(expParam);
    }

    bool trainning(const cv::Mat &inputX, const cv::Mat &inputY)
    {
        cv::Mat oneX;
        cv::Mat zeroX;
        if (inputY.cols != 1 || inputX.cols != dim)
        {
            std::runtime_error("inputY/X column is not 1");
        }
        if (inputX.rows != inputY.rows)
        {
            std::runtime_error("inputX/Y sample number is not equel with model");
        }

        int countOne  = 0;
        int countZero = 0;
        for (int i = 0; i < inputY.rows; ++i)
        {
            if (inputY.at<uint8_t>(i, 0) == 1)
            {
                cv::vconcat(oneX, inputX.row(i), oneX);
                countOne++;
            }
            else if (inputY.at<uint8_t>(i, 0) == 0)
            {
                cv::vconcat(zeroX, inputX.row(i), zeroX);
                countZero++;
            }
            else
            {
                std::cout << "ERROR, Unknown Y value." << std::endl;
            }
        }
        likelihoodModelOne.lamda = countOne / inputY.rows;
        likelihoodModelZero.lamda = (inputY.rows - countOne) / inputY.rows;

        //caculate likelihood (mean and squre)
        cv::reduce(inputX, likelihoodModelOne.mean, 0, CV_REDUCE_AVG);
        likelihoodModelZero.mean = zeroX;
        likelihoodModelOne.mean = oneX;

        for (int i = 0; i < countZero+countOne;)
        {
            cv::Mat divItemX;

            if (inputY.at<uint8_t>(i, 0) == 0)
            {
                divItemX = inputX.row(i) - likelihoodModelZero.mean;
                likelihoodModelZero.squre += divItemX.t() * divItemX;
                i--;
            }
            else if (inputY.at<uint8_t>(i, 0) == 1)
            {
                divItemX = inputX.row(i) - likelihoodModelOne.mean;
                likelihoodModelOne.squre += divItemX.t() * divItemX;
                i--;
            }
            else
            {
                std::cout << "ERROR, Unknown Y value." << std::endl;
            }
        }
        likelihoodModelOne.squre = likelihoodModelOne.squre / countOne;
        likelihoodModelZero.squre = likelihoodModelZero.squre / countZero;
    }

};