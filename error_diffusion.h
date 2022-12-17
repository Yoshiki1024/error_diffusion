#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>

//�덷�g�U�@�ɂ�錸�F�����֐��̒�`
cv::Mat error_diffusion_gray(cv::Mat srcgray);
//cv::Mat error_diffusion_color(cv::Mat srcImage);
cv::Mat vector_error_diffusion(cv::Mat srcImage);

//�덷�̏d�݂Â��ϐ����i�[����z��
 double error_p[4] = {(double)7/16, (double)3/16, (double)5/16, (double)1/16 };
 double error_p_1[3] = { (double)5 / 9, (double)3 / 9, (double)1 / 9 }; //�����̉�f�̏������̊g�U�W��
 double error_p_2[2] = { (double)3 / 4, (double)1 / 4 }; //�E���̉�f�̏������̊g�U�W��




//���F�����̏����֐��i�O���[�X�P�[���j
cv::Mat error_diffusion_gray(cv::Mat srcgray)
{
	/*
	for (int k = 0; k < 128; k++)
	{
		palette[k] = 2*k+1;
		
	}
	*/


	cv::Mat srcgray2 = srcgray.clone();
	int nRows = srcgray2.rows; //�摜�̏c��
	int nCols = srcgray2.cols; //�摜�̉���
	std::vector< int > error_array;
	//std::vector <int> sum_array;
	int sum_array_element = 0;

	//std::cout << nRows << std::endl;
	//std::cout << nCols << std::endl;
	//�K���}�l�����߂�ϐ����i�[
	double gamma = 1/2.2;
	int sum_average = 0;

	//�摜�̑傫�����@for���ŉ�
	for (int y = 0; y < nRows; y++)
	{
		for (int x = 0; x < nCols; x++)
		{



			//�O���X�P�摜�̋P�x�l���擾
			int* brightness_value = srcgray2.ptr<int>(y);
			//unsigned char* under_brightness_value = srcgray2.ptr<unsigned char>(y + 1);
			//sum_array.push_back(int(brightness_value[x]));

			//�K���}�␳������������
			//std::cout << (int)brightness_value[x] << std::endl;
			//brightness_value[x] = std::pow(((double)brightness_value[x] / 255.0), (1.0 / gamma))*255.0;
			//std::cout << int(brightness_value[x]) << std::endl;


			//�ʎq���O�̌Â��s�N�Z�����i�[����ϐ���p�Ӂi0�`�Q�T�T�j
			int brightness = brightness_value[x]; //oldpixel
			//std::cout << "oldpixel�F" << brightness << std::endl;

			if (brightness < 128)
			{
				brightness_value[x] = 0; //newpixel
			}

			else
			{
				brightness_value[x] = 255; //newpixel
			}


			/*
			for (int i = 0; i < 128; i++)
			{

				//���ډ�f�̌덷�����߂�
				error_array.push_back(std::abs(brightness - palette[i]));

			}
			int error = *std::min_element(error_array.begin(), error_array.end());
			*/


			//���ꂼ��Ɋg�U������덷���i�[����ϐ���`

			//�ʎq���O�Ɨʎq����̃s�N�Z���l����덷���Z�o
			int error = brightness - brightness_value[x]; //oldpixel - newpixel
			//error_array.push_back(error);


			//�א�4�s�N�Z���Ɋg�U������
			//�����̃s�N�Z�����̏���
			if (x == 0 && y < nRows - 1)
			{
				brightness_value[x + 1] = int(brightness_value[x + 1]) + int(error_p_1[0] * error); //�E���̃s�N�Z������
				int* under_brightness_value = srcgray2.ptr<int>(y + 1);
				under_brightness_value[x] = int(under_brightness_value[x]) + int(error_p_1[1] * error); //�^���̃s�N�Z������
				under_brightness_value[x + 1] = int(under_brightness_value[x + 1]) + int(error_p_1[2] * error);	//�E���̃s�N�Z������
			}
			//��ԉ��̃s�N�Z�����̏���
			else if (y == nRows - 1 && x < nCols - 1)
			{
				brightness_value[x + 1] = int(brightness_value[x + 1]) + int(error); //�E�������̃s�N�Z������

			}

			//�E���̃s�N�Z�����̏���
			else if (x == nCols - 1 && y < nRows - 1)
			{
				int* under_brightness_value = srcgray2.ptr<int>(y + 1);
				under_brightness_value[x - 1] = int(under_brightness_value[x - 1]) + int(error_p_2[1] * error); //�����̃s�N�Z������
				under_brightness_value[x] = int(under_brightness_value[x]) + int(error_p_2[0] * error); //�^���̃s�N�Z������
			}

			//�E���[�͏������s��Ȃ�
			else if (x == nCols - 1 && y == nRows - 1)
			{
				brightness_value[x] = brightness_value[x];
			}

			else
			{
				int* under_brightness_value = srcgray2.ptr<int>(y + 1);
				brightness_value[x + 1] = int(brightness_value[x + 1]) + int(error_p[0] * error); //�E�ׂ̃s�N�Z��
				under_brightness_value[x - 1] = int(under_brightness_value[x - 1]) + int(error_p[1] * error); //�����̃s�N�Z��
				under_brightness_value[x] = int(under_brightness_value[x]) + int(error_p[2] * error); //�^���̃s�N�Z��
				under_brightness_value[x + 1] = int(under_brightness_value[x + 1]) + int(error_p[3] * error); //�E���̃s�N�Z��
			}
		}


	}
	return srcgray2;
}





//�x�N�g���덷�g�U�@�A���S���Y��
cv::Mat vector_error_diffusion(cv::Mat srcImage)
{
	cv::Mat frame1 = srcImage.clone();
	int count = 0; //���񏈗����s��������\��������ϐ�
	double gamma = 2.2; //�K���}�����p�����[�^
	std::vector <std::vector<int>> P_vec = { {0,0,0}, {0,0,255}, {0,255,0}, {0,255,255}, {255,0,0}, {255,0,255}, {255,255,0}, {255,255,255} }; //���F���邽�߂̃p���b�g�쐬
	std::vector <int> x_err; //���摜�̉�f�l���i�[����z��
	std::vector <int> fx; //�Z�o���ꂽ���[�N���b�h�����̒l���i�[����z��
	std::vector <int> x_vec; //�x�N�g���덷�g�U�@�K�p��̉�f�l���i�[����z��
	std::vector <int> e_i; //�덷���i�[����z��
	

	for (int y = 0; y < frame1.rows; y++)
	{
		for (int x = 0; x < frame1.cols; x++)
		{
			//���摜�̉�f�l���擾
			cv::Vec3i* x_err_argment = frame1.ptr<cv::Vec3i>(y);
			/*std::cout << "B�l��" + std::to_string(x_err_argment[x][0]) << std::endl;
			std::cout << "G�l��" + std::to_string(x_err_argment[x][1]) << std::endl;
			std::cout << "R�l��" + std::to_string(x_err_argment[x][2]) << std::endl;*/
			//std::cout << "���ډ�f�lx_now = (" + std::to_string(x_err_argment[x][0]) + "," + std::to_string(x_err_argment[x][1]) + "," + std::to_string(x_err_argment[x][2]) + ")" << std::endl;
			

			////�K���}�␳�ɂ��C���邳����
			////�܂���B�l����s��
			//std::cout << ((double)x_err_argment[x][0] / 255.0) << std::endl;
			//x_err_argment[x][0] = std::pow(((double)x_err_argment[x][0] / 255.0), (1.0 / gamma)) * 255.0;
			//std::cout << x_err_argment[x][0]<< std::endl;
			////G�l�̏������s��
			//x_err_argment[x][1] = std::pow(((double)x_err_argment[x][1] / 255.0), (1.0 / gamma)) * 255.0;
			//std::cout << x_err_argment[x][1] << std::endl;
			////R�l�̏������s��
			//x_err_argment[x][2] = std::pow(((double)x_err_argment[x][2] / 255.0), (1.0 / gamma)) * 255.0;
			//std::cout << x_err_argment[x][2] << std::endl;

			//����덷���Z�o���邽�߂ɁC�z��ɉ�f�l���i�[
			x_err.push_back(x_err_argment[x][0]);
			x_err.push_back(x_err_argment[x][1]);
			x_err.push_back(x_err_argment[x][2]);
			
			//�ŏ��ɂ��邽�߂̖ړI�֐�f(x)���쐬
			for (int l = 0; l < 8; l++)
			{
				//���[�N���b�h�������Z�o����
				int fx_argment = std::pow((P_vec[l][0] - int(x_err[0])), 2) + std::pow((P_vec[l][1] - int(x_err[1])), 2) + std::pow((P_vec[l][2] - int(x_err[2])), 2);
				//std::cout << "(" + std::to_string(P_vec[l][0]) + "," + std::to_string(P_vec[l][1]) + "," + std::to_string(P_vec[l][2]) + ")�Ƃ�"  + "���[�N���b�h������" + std::to_string(fx_argment) << std::endl;
				//�C���f�b�N�X���擾���邽�߁C�z��ɎZ�o�������[�N���b�h�������i�[
				fx.push_back(fx_argment);
			}

			//���[�N���b�h�������ŏ��ƂȂ�C���f�b�N�X���擾
			std::vector <int>::iterator fx_iter = std::min_element(fx.begin(), fx.end());
			size_t fx_index = std::distance(fx.begin(), fx_iter);
			//std::cout << fx_index << std::endl;

			

			//���[�N���b�h�������Z���C���f�b�N�X���擾���C�ϐ�l_imp�Ɋi�[
			int l_imp = fx_index;
			//�ʎq����̏o�͉�f�l���i�[����
			x_vec.push_back(P_vec[fx_index][0]);
			x_vec.push_back(P_vec[fx_index][1]);
			x_vec.push_back(P_vec[fx_index][2]);
			

			//�ʎq�����ꂽ��f�l�����摜�ɒu��������
			x_err_argment[x][0] = P_vec[fx_index][0];
			x_err_argment[x][1] = P_vec[fx_index][1];
			x_err_argment[x][2] = P_vec[fx_index][2];

			//�덷��z��Ɋi�[����
			e_i.push_back(x_err[0] - x_vec[0]);
			e_i.push_back(x_err[1] - x_vec[1]);
			e_i.push_back(x_err[2] - x_vec[2]);
			

			//�덷��אڂS�s�N�Z����g�U�����鏈��

			//�����̃s�N�Z�����̏���--------------------------------------------------
			if (x == 0 && y < frame1.rows - 1)
			{
				//�E���̃s�N�Z�������ɂ��ď���
				//B�l�̏���
				x_err_argment[x + 1][0] = int(x_err_argment[x + 1][0]) + int(e_i[0] * error_p_1[0]);
				//G�l�̏���
				x_err_argment[x + 1][1] = int(x_err_argment[x + 1][1]) + int(e_i[1] * error_p_1[0]);
				//R�l�̏���
				x_err_argment[x + 1][2] = int(x_err_argment[x + 1][2]) + int(e_i[2] * error_p_1[0]);

				//�^���̃s�N�Z�������ɂ���
				cv::Vec3i* under_x_err_argment = frame1.ptr<cv::Vec3i>(y + 1); //�g�U������ۂ̉��R�s�N�Z���ɃA�N�Z�X
				//B�l�̏���
				under_x_err_argment[x][0] = int(under_x_err_argment[x][0]) + int(e_i[0] * error_p_1[1]);
				//G�l�̏���
				under_x_err_argment[x][1] = int(under_x_err_argment[x][1]) + int(e_i[1] * error_p_1[1]);
				//R�l�̏���
				under_x_err_argment[x][2] = int(under_x_err_argment[x][2]) + int(e_i[2] * error_p_1[1]);

				//�E���̃s�N�Z�������ɂ���
				//B�l�̏���
				under_x_err_argment[x + 1][0] = int(under_x_err_argment[x + 1][0]) + int(e_i[0] * error_p_1[2]);
				//G�l�̏���
				under_x_err_argment[x + 1][1] = int(under_x_err_argment[x + 1][1]) + int(e_i[1] * error_p_1[2]);
				//R�l�̏���
				under_x_err_argment[x + 1][2] = int(under_x_err_argment[x + 1][2]) + int(e_i[2] * error_p_1[2]);	
			}
			//-------------------------------------------------------------------------------------------------------------------------
			
			//��ԉ��̃s�N�Z���̏���----------------------------------------------------------------------------------------------
			else if (y == frame1.rows - 1 && x < frame1.cols - 1)
			{
				//�E���̃s�N�Z�������ɂ��ď���
				//B�l�̏���
				x_err_argment[x + 1][0] = int(x_err_argment[x + 1][0]) + int(e_i[0]);
				//G�l�̏���
				x_err_argment[x + 1][1] = int(x_err_argment[x + 1][1]) + int(e_i[1]);
				//R�l�̏���
				x_err_argment[x + 1][2] = int(x_err_argment[x + 1][2]) + int(e_i[2]);
			}
			//--------------------------------------------------------------------------------------------------------------------
			
			//��ԉE���̃s�N�Z�����̏���
			else if (x == frame1.cols - 1 && y < frame1.rows - 1)
			{
				//�����̃s�N�Z������
				cv::Vec3i* under_x_err_argment = frame1.ptr<cv::Vec3i>(y + 1); //�g�U������ۂ̉��R�s�N�Z���ɃA�N�Z�X
				//B�l�̏���
				under_x_err_argment[x - 1][0] = int(under_x_err_argment[x - 1][0]) + int(e_i[0] * error_p_2[1]);
				//G�l�̏���
				under_x_err_argment[x - 1][1] = int(under_x_err_argment[x - 1][1]) + int(e_i[1] * error_p_2[1]);
				//R�l�̏���
				under_x_err_argment[x - 1][2] = int(under_x_err_argment[x - 1][2]) + int(e_i[2] * error_p_2[1]);

				//�^���̃s�N�Z������
				//B�l�̏���
				under_x_err_argment[x][0] = int(under_x_err_argment[x][0]) + int(e_i[0] * error_p_2[0]);
				//G�l�̏���
				under_x_err_argment[x][1] = int(under_x_err_argment[x][1]) + int(e_i[1] * error_p_2[0]);
				//R�l�̏���
				under_x_err_argment[x][2] = int(under_x_err_argment[x][2]) + int(e_i[2] * error_p_2[0]);
			}

			//�E���[�͏������s��Ȃ�
			else if (x == frame1.cols - 1 && y == frame1.rows - 1)
			{
				//B�l�̏���
				x_err_argment[x][0] = x_err_argment[x][0];
				//G�l�̏���
				x_err_argment[x][1] = x_err_argment[x][1];
				//R�l�̏���
				x_err_argment[x][2] = x_err_argment[x][2];
			}

			else
			{
				//�E�ׂ̃s�N�Z������
				//B�l�̏���
				x_err_argment[x + 1][0] = int(x_err_argment[x + 1][0]) + int(e_i[0] * error_p[0]);
				//G�l�̏���
				x_err_argment[x + 1][1] = int(x_err_argment[x + 1][1]) + int(e_i[1] * error_p[0]);
				//R�l�̏���
				x_err_argment[x + 1][2] = int(x_err_argment[x + 1][2]) + int(e_i[2] * error_p[0]);

				//�����̃s�N�Z������
				cv::Vec3i* under_x_err_argment = frame1.ptr<cv::Vec3i>(y + 1); //�g�U������ۂ̉��R�s�N�Z���ɃA�N�Z�X
				//B�l�̏���
				under_x_err_argment[x - 1][0] = int(under_x_err_argment[x - 1][0]) + int(e_i[0] * error_p[1]);
				//G�l�̏���
				under_x_err_argment[x - 1][1] = int(under_x_err_argment[x - 1][1]) + int(e_i[1] * error_p[1]);
				//R�l�̏���
				under_x_err_argment[x - 1][2] = int(under_x_err_argment[x - 1][2]) + int(e_i[2] * error_p[1]);

				//�^���̃s�N�Z������
				//B�l�̏���
				under_x_err_argment[x][0] = int(under_x_err_argment[x][0]) + int(e_i[0] * error_p[2]);
				//G�l�̏���
				under_x_err_argment[x][1] = int(under_x_err_argment[x][1]) + int(e_i[1] * error_p[2]);
				//R�l�̏���
				under_x_err_argment[x][2] = int(under_x_err_argment[x][2]) + int(e_i[2] * error_p[2]);

				//�E���̃s�N�Z������
				//B�l�̏���
				under_x_err_argment[x + 1][0] = int(under_x_err_argment[x + 1][0]) + int(e_i[0] * error_p[3]);
				//G�l�̏���
				under_x_err_argment[x + 1][1] = int(under_x_err_argment[x + 1][1]) + int(e_i[1] * error_p[3]);
				//R�l�̏���
				under_x_err_argment[x + 1][2] = int(under_x_err_argment[x + 1][2]) + int(e_i[2] * error_p[3]);


			}
			//�z��̏�����������
			x_err.clear();
			fx.clear();
			x_vec.clear();
			e_i.clear();

			count = count + 1;
			//�R�����g���ŏo�͌��ʂ��m�F����
			//std::cout << "x_err = " << "{" + std::to_string(x_err_argment[x][0] ) + " , " + std::to_string(x_err_argment[x][1]) + " , " + std::to_string(x_err_argment[x][2]) + "}" << std::endl;
			// //std::cout << "���[�N���b�h�������ŏ��ƂȂ�̂�" + std::to_string(fx_index) + "�Ԗڂł���" <<  std::endl;
			//std::cout << "���Ȃ킿�Cl*��" + std::to_string(fx_index) << std::endl;
			//std::cout << "x_vec = {" + std::to_string(x_vec[0]) + " , " + std::to_string(x_vec[1]) + " , " + std::to_string(x_vec[2]) + "}" << std::endl;
			// //std::cout << "�덷 = {" + std::to_string(e_i[0]) + " , " + std::to_string(e_i[1]) + " , " + std::to_string(e_i[2]) + "}";
			//std::cout << std::to_string(count) + "��ڂ̏������I���܂���" << std::endl;
		}
	}
	return frame1;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------


