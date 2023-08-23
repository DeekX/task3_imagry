#include "threads.h"
Threads::Threads() {
    capturingThread = std::jthread(&Threads::startCapturing,this);
    operationsThread = std::jthread(&Threads::processFrame, this);
}

void Threads::startCapturing() {
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return;
    }

    while (true) {
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "Error: Could not read frame." << std::endl;
            break;
        }

        else{
            std::lock_guard<std::mutex>lock(mtx);
            ++frameIndex;
            framesQueue.push({frame,frameIndex});
        }
        if (cv::waitKey(1) == 27) {
            break;
        }
    }
    cap.release();
    cv::destroyAllWindows();
}


void Threads::processFrame() {
    static auto pKNN = cv::createBackgroundSubtractorKNN();
    while (true) {
        if (!framesQueue.empty()) {
            //std::cout<<frameIndex<<'\n';
            cv::Mat fgMask,result;
            std::pair<cv::Mat,int> currentFrame;
            {
                std::lock_guard<std::mutex>lock(mtx);
                if(!framesQueue.front().first.empty())
                    currentFrame= framesQueue.front();
                framesQueue.pop();
            }
            //currentFrame = framesQueue.front();
            std::cout<<currentFrame.second<<'\n';
            pKNN->apply(currentFrame.first, fgMask);
            cv::bitwise_and(currentFrame.first,currentFrame.first,result,fgMask);
            cv::imshow("Frame", currentFrame.first);
            cv::imshow("FG Mask1", fgMask);
            cv::imshow("FG Mask", result);
        }
    }
}

