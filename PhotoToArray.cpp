
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2\imgproc.hpp>
#include<string>
#include <iostream>
#include <fstream>
#include<cmath>;

using namespace std;
using namespace cv;

//double maxValue = 150;  //局部二值化阈值
double Value = 128;     //全局二值化阈值

ofstream os;     //创建一个文件输出流对象
//灰度图函数
Mat Grayscale(Mat img) {
    int width = img.cols;//宽    
    int height = img.rows;//高     

    Mat out = out.zeros(height, width, CV_8UC1); //CV_8UC1 单通道

    //遍历每一个像素
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            out.at<uchar>(y, x) = 0.2126 * (float)img.at<Vec3b>(y, x)[2] \
                + 0.7152 * (float)img.at<Vec3b>(y, x)[1] \
                + 0.0722 * (float)img.at<Vec3b>(y, x)[0];

            // Y = 0.2126\ R + 0.7152\ G + 0.0722\ B        //灰度公式
        }
    }
    return out;
}
//二值化函数
//
Mat Binarize(Mat img, int th) {   
    //th为阈值0-255
    int width = img.cols;//宽
    int height = img.rows;//高

    Mat out = out.zeros(height, width, CV_8UC1);

    //遍历每一个像素
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {

            if (img.at<uchar>(y, x) > th) {
                out.at<uchar>(y, x) = 255;
            }
            else {
                out.at<uchar>(y, x) = 0;
            }
        }
    }
    return out;
}
//十进制转化十六进制
string to16(int data)
{
    //本程序最高十进制数为255  
    //十六进制范围为0X00--0Xff
    int tempData;
    char h[16] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };
    char tempFirst = '0';
    char tempSecond = '0';
    String ReturnDate = "0X";
    
    if (data < 16)
    {
        tempSecond = h[data];
        return ReturnDate + tempFirst + tempSecond;
    }
    else
    {
        tempData = data / 16;
        tempFirst = h[tempData];
        tempData = data % 16;
        tempSecond = h[tempData];
        return ReturnDate + tempFirst + tempSecond;
    }
    
   
}
//二进制转十进制
int to10(int data)
{
    //最大二进制数为 11111111
    int power = 0;
    int tempdata = 0;
    int enddata=0;
    for (int i = 0; i < 8; i++)
    {
        tempdata = data % 10;
        if (tempdata == 1)
        {
            enddata += int(pow(2, power));
        }
        data = data / 10;
        power++;
        if (power == 8)
        {
            power = 0;
        }

    }
    return enddata;
}
//图片转化数组
void toArray(Mat img) {
    String Array = "const unsigned char gImage []= {";      //输出数组的开头
    String end = "};";                                       //输出数组的结尾
    String comma = ",";                                      //分割各个十六进制数
    String temp8bit;                                        //暂存8位的二进制数（字符型）
    int tempint;                                            //暂存8位的二进制数（整形）
    char temp;                                             //暂存一位二进制数
    int i = 0;                      
    os << Array;
    for (int y = 0; y < img.rows; y++)
      //从上到下
    //for (int x = 0; x < img.cols; x++)  //从上到下
    {
        for (int x = 0; x < img.cols; x++)
        //for (int y = 0; y < img.rows; y++)      //z y
        {
            //图片黑色部分是黑色
            
            if (img.at<uchar>(y, x)>Value)      //(img.at<uchar>(rows, cols)
            {
                temp= '0';      //0为灯灭
            }
            else
            {

                temp= '1';       //1位灯亮
            }
            
            /**
            //图片白色部分是白色
            if (img.at<uchar>(y, x) > Value)      //(img.at<uchar>(rows, cols)
            {

                temp = '1';        //1位灯亮
            }
            else
            {
                temp = '0';         //0为灯灭

            }
            */
            temp8bit = temp8bit + temp;
            
            i++; 
            if (y==img.rows)        //行结束清零
            {
                tempint = stoi(temp8bit);
                os << to16(to10(tempint));//先转化成十进制然后转化成十六进制
                i = 0;
                temp8bit = "";
                os << comma;
            }
            if (i == 8)     //8位二进制数得到后清零
            {
                tempint = stoi(temp8bit);
                os << to16(to10(tempint));//先转化成十进制然后转化成十六进制
                i = 0;
                temp8bit ="";
                os << comma;
            }
            
        }
    }   
    os << end;
    os.close();
}



int main()
{
    os.open("array.txt");//将对象与文件关联
   // Mat img = imread("text64_64.jpg");//读取图片
    Mat img = imread("5.jpg");//读取图片
    Mat GrayscaleImg = Grayscale(img);//得到灰度图
    Mat BinarizeImg = Binarize(GrayscaleImg, Value);//二值化
    namedWindow("显示原始图片", 0);//可变大小窗口
    imshow("显示原始图片", img);
    namedWindow("显示灰度图片", 0);
    imshow("显示灰度图片", GrayscaleImg);
    namedWindow("显示二值化图片", 0);
    imshow("显示二值化图片", BinarizeImg);
    cout << "正在生成中请耐心等待" << endl;
    toArray(BinarizeImg);//生成二值化的数组文件
    //toArray(GrayscaleImg);//生成灰度图的数组文件
    cout << "生成完成！";
    waitKey(0);
    return 0;
}
