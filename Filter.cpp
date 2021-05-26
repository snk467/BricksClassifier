#include "Filter.h"
#include "Converter.h"

cv::Mat Filter::rankFilter(cv::Mat sourceImage, int N, int index)
{
    // Sprawdzanie danych wej�ciowych 
    if (N % 2 == 0 || index < 0 || index >= N * N)
    {
        throw std::exception();
    }

    cv::Mat_<cv::Vec3b> image = sourceImage.clone();
    Converter converter = Converter(image);
    cv::Mat_<uchar> grayImage = converter.toGray();
    cv::Mat_<cv::Vec3b> outImage = image.clone();
    for (int x = N / 2; x < image.cols - N / 2; x++)
    {
        for (int y = N / 2; y < image.rows - N / 2; y++)
        {
            // Lista do przechowywania przetwarzanych pikseli
            std::vector<std::pair<cv::Vec3b, uchar>> pixelsList = std::vector<std::pair<cv::Vec3b, uchar>>();
            // Gromadzenie przetwarzanych pikseli
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    pixelsList.push_back(std::pair<cv::Vec3b, uchar>(image(y - N / 2 + j, x - N / 2 + i), grayImage(y - N / 2 + j, x - N / 2 + i)));
                }
            }
            std::sort(pixelsList.begin(), pixelsList.end(),
                [](const std::pair<cv::Vec3b, uchar>& a, const std::pair<cv::Vec3b, uchar>& b)
                {
                    return a.second < b.second;
                });

            // Ustawianie piksela wynikowego na odpowiedni� warto��
            outImage(y, x) = pixelsList[index].first;
        }
    }

    return outImage;
}
