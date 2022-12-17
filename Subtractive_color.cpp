#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include "error_diffusion.h"







int main() 
{
	////------------------- 処理実験プログラム -------------------------------
	//
	//
	////カメラの起動
	//cv::VideoCapture camera(0);

	//if (!camera.isOpened()) {
	//	std::cout << "カメラが起動していません" << std::endl;
	//	return -1;
	//}

	////カメラの元画像と，グレスケ，エッジ検出をした画像を格納
	//cv::Mat frame, gray,canny;
	//




	//while (camera.read(frame)) {
	//	//グレスケ変換
	//	cvtColor(frame, gray, cv::COLOR_RGB2GRAY);
	//	cv::Mat binary = gray.clone();

	//	//単純二値化処理
	//	//cv::threshold(gray, binary, 128, 255, cv::)
	//	for (int y = 0; y < binary.rows; y++)
	//	{
	//		for (int x = 0; x < binary.cols; x++)
	//		{
	//			unsigned char* brightness_value = binary.ptr<unsigned char>(y);

	//			if (brightness_value[x] < 128)
	//			{
	//				brightness_value[x] = 0;
	//			}
	//			else
	//			{
	//				brightness_value[x] = 255;
	//			}
	//		}
	//	}


	//	//cannyによるエッジ検出
	//	//cv::Canny(gray, canny, 50, 200);

	//	//cannyをRGB式空間に変換
	//	//std::cout << "RGB色空間変換の関数作成" << std::endl;
	//	
	//	
	//	
	//	//減色処理を行う
	//	//cv::Mat ed_Image_gray= error_diffusion_gray(gray); //グレースケール画像
	//	//cv::Mat ed_Image_color = error_diffusion_color(frame); //カラー画像
	//
	//	
	//
	//	//減色処理したものと輪郭画像の差分を返す
	//	//std::cout << "差分を返す" << std::endl;

	//	

	//	
	//	
	//	
	//	cv::imshow("srcImage", frame);
	//	cv::imshow("grayscale", gray);
	//	cv::imshow("binary", binary);
	//	//cv::imshow("canny", canny);
	//	//cv::imshow("error_diffusion_gray", ed_Image_gray);
	//	//cv::imshow("error_diffusion_gray_pointer", ed_Image_gray_pointer);
	//    //v::imshow("error_diffusion_color", ed_Image_color);

	//	const int key = cv::waitKey(1);
	//	if (key == 'q')
	//	{
	//		cv::imwrite("tatton.png", frame);
	//		cv::imwrite("tatton_gray.png", gray);
	//		cv::imwrite("tatton_binary.png", binary);
	//		//cv::imwrite("error_diffusion.bmp", ed_Image_gray);
	//		//cv::imwrite("error_diffusion_color.bmp", ed_Image_color);
	//		break;
	//	}
	//	
	//}

	//-------------------------カメラ起動での処理終了-----------------------------------

	//動画で予備実験
	for (int i = 1; i < 351; i++)
	{
		cv::Mat srcImage = cv::imread("C:/yoshiki_Alldata/research_program/movie_subtractive/frame/sample_frame_" + std::to_string(i) + ".png");
		cv::Mat srcImage2;
		srcImage.convertTo(srcImage2, CV_32SC3);
		//cv::Mat srcImage2(srcImage.size(), CV_16S);
		//srcImage2 = srcImage.clone();
		//std::cout << srcImage2.type() << std::endl;

		cv::Mat srcgray;
		cv::Mat srcgray2;
		cv::Mat canny;
		cv::Mat ed_Image_color2; //ガンマ補正語の画像を格納する変数


		//std::cout << srcImage.elemSize() << std::endl;


		if (!srcImage.data)
		{
			std::cout << "データがありません" << std::endl;
			return -1;
		}

		cv::cvtColor(srcImage, srcgray, cv::COLOR_BGR2GRAY);
		//cv::Canny(srcgray, canny, 50, 200);
		//1フレームずつフォルダに書き込み
		//cv::imwrite("C:/yoshiki_Alldata/research_program/movie_subtractive/edge_frame/edge_frame_" + std::to_string(i) + ".png", canny);

		//std::cout << srcgray << std::endl;
		srcgray.convertTo(srcgray2, CV_32S);

		//誤差各線法による減色処理関数の適用



		cv::Mat ed_Image = error_diffusion_gray(srcgray2);
		//std::cout << ed_Image << std::endl;
		//cv::Mat ed_Image_color = error_diffusion_color(srcImage);
		cv::Mat ed_Image_color_vector = vector_error_diffusion(srcImage2);
		//std::cout << ed_Image_color_vector << std::endl;
		cv::Mat ed_Image_color_vector2;
		ed_Image_color_vector.convertTo(ed_Image_color_vector2, CV_8UC3);

		//std::cout << ed_Image_color_vector << std::endl;


		//imshowで画像を表示する
		//cv::imshow("tatton", srcImage);
		//cv::imshow("tatton_gray", srcgray);
		//cv::imshow("tatton_diffusion", ed_Image);
		//cv::imshow("lenna_vector_diffusion", ed_Image_color_vector);
		//画像の保存
		cv::imwrite("C:/yoshiki_Alldata/research_program/movie_subtractive/gray_frame/sample_frame_gray_" + std::to_string(i) + ".png", ed_Image);
		cv::imwrite("C:/yoshiki_Alldata/research_program/movie_subtractive/subtractive_frame/sample_frame_subtractive_" + std::to_string(i) + ".png", ed_Image_color_vector2);
		std::cout << std::to_string(i) + "回目の処理終了です" << std::endl;
	}
	std::cout << "全ての処理が終わりました" << std::endl;
	//cv::imwrite("sample.jpg",ed_Image_color_vector2);
	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}








