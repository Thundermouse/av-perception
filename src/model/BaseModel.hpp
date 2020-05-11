#include <opencv2/opencv.hpp>
#include <string>
class BaseModel
{
private:
    std::string modelName{};
    uint32_t numParam{};
    cv::Mat params{};
public:
    virtual bool inference() = 0;
    virtual bool training() = 0;
};