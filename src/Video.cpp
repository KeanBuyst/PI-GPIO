#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>

#include <Video.h>

using namespace cv;

bool running = false;

void Video::Transmit(unsigned int port)
{
    running = true;

    // init camera
    VideoCapture camera(0);
    if (!camera.isOpened())
        std::cout << "Camera couldnt be opened" << std::endl;

    int sock = socket(AF_INET,SOCK_DGRAM, 0);

    // address
    sockaddr_in destAddr;
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(port);  // Set your desired port
    inet_pton(AF_INET, IP, &(destAddr.sin_addr));

    while (running)
    {
        std::vector<uchar> buffer;
        Mat frame;

        camera.grab();
        camera.retrieve(frame);
        if (frame.empty()) {
            continue;
        }
        if (!cv::imencode(".jpg", frame, buffer))
            std::cout << "Failed to compress image to JPEG\n";
        sendto(sock, buffer.data(),buffer.size(),0, (struct sockaddr *)&destAddr, sizeof(destAddr));
    }

    close(sock);
}

void Video::Stop()
{
    running = false;
}