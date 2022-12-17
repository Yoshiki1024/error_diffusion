#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>

//�O���[�X�P�[���摜�ɏ�����K���}�␳���s��
void Gamma_gray(cv::Mat ed_Image, cv::Mat ed_Image2, double gamma)
{
	uchar LUT[256];
	ed_Image2 = ed_Image.clone();
	

	//�K���}�␳�e�[�u���̍쐬
	for (int i = 0; i < 256; i++)
	{
		LUT[i] = (int)(pow((double)i / 255.0, 1.0 / gamma) * 255.0);

	}

	//CvMat�֕ϊ�
	cv::Mat lut_mat = cv::Mat(1, 256, CV_8UC1, LUT);

	cv::LUT(ed_Image, lut_mat, ed_Image2);
}
	

//�O���[�X�P�[���摜�ɏ�����K���}�␳���s��
void Gamma_color(cv::Mat ed_Image_color,  double gamma)
{
	uchar LUT[256];
	//�K���}�␳�e�[�u���̍쐬
	for (int i = 0; i < 256; i++)
	{
		LUT[i] = (int)(pow((double)i / 255.0, 1.0 / gamma) * 255.0);

	}

	//CvMat�֕ϊ�
	cv::Mat lut_mat = cv::Mat(1, 256, CV_8UC1, LUT);

	cv::LUT(ed_Image_color, lut_mat, ed_Image_color);
}