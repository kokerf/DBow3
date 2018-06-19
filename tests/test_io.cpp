#include <iostream>
#include <vector>

// DBoW3
#include "DBoW3.h"

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
using namespace DBoW3;
using namespace std;

// ----------------------------------------------------------------------------

int main(int argc,char **argv)
{

    if(argc!=6)
    {
        cerr<<"Usage: in_voc.dbow out_db.db image0 image1 image2"<<endl;
        return -1;
    }

    DBoW3::Vocabulary voc;
    voc.load(argv[1]);
    cout<<"loaded"<<endl;
    
    DBoW3::Database db(voc, true, 4);


    cv::Ptr<cv::Feature2D> fdetector;
    fdetector=cv::ORB::create();

    vector<cv::Mat>  features;
    for(size_t i = 0; i < 3; ++i)
    {
        vector<cv::KeyPoint> keypoints;
        cv::Mat descriptors;
        string path_to_image = argv[3+i];
        cv::Mat image = cv::imread(path_to_image, 0);
        if(image.empty())throw std::runtime_error("Could not open image"+path_to_image);
        cout<<"extracting features"<<endl;
        fdetector->detectAndCompute(image, cv::Mat(), keypoints, descriptors);
        features.push_back(descriptors);
        cout<<"done detecting features"<<endl;
    }

    for(size_t i = 0; i < 3; ++i)
    {
        db.add(features[i]);
    }

    for(size_t i = 0; i < 3; ++i)
    {
        QueryResults rets;
        db.query(features[i], rets, 3);
        cout << rets << endl;

    }

    cout << "db: " << db << endl;
    db.save(argv[2]);

    DBoW3::Database db1(argv[2]);
    cout << "db1: " << db1 << endl;

    for(size_t i = 0; i < 3; ++i)
    {
        QueryResults rets;
        db1.query(features[i], rets, 3);
        cout << rets << endl;
    }

 
    return 0;
}
