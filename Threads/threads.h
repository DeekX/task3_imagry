#pragma once
#include <thread>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <bits/stdc++.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <time.h>
#include <mutex>
#include <opencv2/video/background_segm.hpp>

class Threads {
    std::jthread capturingThread;
    std::jthread operationsThread;
    cv::Mat frame;
    std::mutex mtx;
    long long frameIndex=0;
    bool frameReady = false;
    std::queue<std::pair<cv::Mat,int>>framesQueue;
public:
    Threads();

private:
    void startCapturing();
    void processFrame();
};