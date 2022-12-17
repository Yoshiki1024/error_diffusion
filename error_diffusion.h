#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>

//誤差拡散法による減色処理関数の定義
cv::Mat error_diffusion_gray(cv::Mat srcgray);
//cv::Mat error_diffusion_color(cv::Mat srcImage);
cv::Mat vector_error_diffusion(cv::Mat srcImage);

//誤差の重みづけ変数を格納する配列
 double error_p[4] = {(double)7/16, (double)3/16, (double)5/16, (double)1/16 };
 double error_p_1[3] = { (double)5 / 9, (double)3 / 9, (double)1 / 9 }; //左側の画素の処理時の拡散係数
 double error_p_2[2] = { (double)3 / 4, (double)1 / 4 }; //右側の画素の処理時の拡散係数




//減色処理の処理関数（グレースケール）
cv::Mat error_diffusion_gray(cv::Mat srcgray)
{
	/*
	for (int k = 0; k < 128; k++)
	{
		palette[k] = 2*k+1;
		
	}
	*/


	cv::Mat srcgray2 = srcgray.clone();
	int nRows = srcgray2.rows; //画像の縦幅
	int nCols = srcgray2.cols; //画像の横幅
	std::vector< int > error_array;
	//std::vector <int> sum_array;
	int sum_array_element = 0;

	//std::cout << nRows << std::endl;
	//std::cout << nCols << std::endl;
	//ガンマ値を決める変数を格納
	double gamma = 1/2.2;
	int sum_average = 0;

	//画像の大きさ分　for分で回す
	for (int y = 0; y < nRows; y++)
	{
		for (int x = 0; x < nCols; x++)
		{



			//グレスケ画像の輝度値を取得
			int* brightness_value = srcgray2.ptr<int>(y);
			//unsigned char* under_brightness_value = srcgray2.ptr<unsigned char>(y + 1);
			//sum_array.push_back(int(brightness_value[x]));

			//ガンマ補正処理をかける
			//std::cout << (int)brightness_value[x] << std::endl;
			//brightness_value[x] = std::pow(((double)brightness_value[x] / 255.0), (1.0 / gamma))*255.0;
			//std::cout << int(brightness_value[x]) << std::endl;


			//量子化前の古いピクセルを格納する変数を用意（0～２５５）
			int brightness = brightness_value[x]; //oldpixel
			//std::cout << "oldpixel：" << brightness << std::endl;

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

				//注目画素の誤差を求める
				error_array.push_back(std::abs(brightness - palette[i]));

			}
			int error = *std::min_element(error_array.begin(), error_array.end());
			*/


			//それぞれに拡散させる誤差を格納する変数定義

			//量子化前と量子化後のピクセル値から誤差を算出
			int error = brightness - brightness_value[x]; //oldpixel - newpixel
			//error_array.push_back(error);


			//隣接4ピクセルに拡散させる
			//左側のピクセル時の処理
			if (x == 0 && y < nRows - 1)
			{
				brightness_value[x + 1] = int(brightness_value[x + 1]) + int(error_p_1[0] * error); //右側のピクセル処理
				int* under_brightness_value = srcgray2.ptr<int>(y + 1);
				under_brightness_value[x] = int(under_brightness_value[x]) + int(error_p_1[1] * error); //真下のピクセル処理
				under_brightness_value[x + 1] = int(under_brightness_value[x + 1]) + int(error_p_1[2] * error);	//右下のピクセル処理
			}
			//一番下のピクセル時の処理
			else if (y == nRows - 1 && x < nCols - 1)
			{
				brightness_value[x + 1] = int(brightness_value[x + 1]) + int(error); //右側だけのピクセル処理

			}

			//右側のピクセル時の処理
			else if (x == nCols - 1 && y < nRows - 1)
			{
				int* under_brightness_value = srcgray2.ptr<int>(y + 1);
				under_brightness_value[x - 1] = int(under_brightness_value[x - 1]) + int(error_p_2[1] * error); //左下のピクセル処理
				under_brightness_value[x] = int(under_brightness_value[x]) + int(error_p_2[0] * error); //真下のピクセル処理
			}

			//右下端は処理を行わない
			else if (x == nCols - 1 && y == nRows - 1)
			{
				brightness_value[x] = brightness_value[x];
			}

			else
			{
				int* under_brightness_value = srcgray2.ptr<int>(y + 1);
				brightness_value[x + 1] = int(brightness_value[x + 1]) + int(error_p[0] * error); //右隣のピクセル
				under_brightness_value[x - 1] = int(under_brightness_value[x - 1]) + int(error_p[1] * error); //左下のピクセル
				under_brightness_value[x] = int(under_brightness_value[x]) + int(error_p[2] * error); //真下のピクセル
				under_brightness_value[x + 1] = int(under_brightness_value[x + 1]) + int(error_p[3] * error); //右下のピクセル
			}
		}


	}
	return srcgray2;
}





//ベクトル誤差拡散法アルゴリズム
cv::Mat vector_error_diffusion(cv::Mat srcImage)
{
	cv::Mat frame1 = srcImage.clone();
	int count = 0; //何回処理を行ったかを表示させる変数
	double gamma = 2.2; //ガンマ調整パラメータ
	std::vector <std::vector<int>> P_vec = { {0,0,0}, {0,0,255}, {0,255,0}, {0,255,255}, {255,0,0}, {255,0,255}, {255,255,0}, {255,255,255} }; //減色するためのパレット作成
	std::vector <int> x_err; //現画像の画素値を格納する配列
	std::vector <int> fx; //算出されたユークリッド距離の値を格納する配列
	std::vector <int> x_vec; //ベクトル誤差拡散法適用後の画素値を格納する配列
	std::vector <int> e_i; //誤差を格納する配列
	

	for (int y = 0; y < frame1.rows; y++)
	{
		for (int x = 0; x < frame1.cols; x++)
		{
			//現画像の画素値を取得
			cv::Vec3i* x_err_argment = frame1.ptr<cv::Vec3i>(y);
			/*std::cout << "B値は" + std::to_string(x_err_argment[x][0]) << std::endl;
			std::cout << "G値は" + std::to_string(x_err_argment[x][1]) << std::endl;
			std::cout << "R値は" + std::to_string(x_err_argment[x][2]) << std::endl;*/
			//std::cout << "注目画素値x_now = (" + std::to_string(x_err_argment[x][0]) + "," + std::to_string(x_err_argment[x][1]) + "," + std::to_string(x_err_argment[x][2]) + ")" << std::endl;
			

			////ガンマ補正により，明るさ調節
			////まずはB値から行う
			//std::cout << ((double)x_err_argment[x][0] / 255.0) << std::endl;
			//x_err_argment[x][0] = std::pow(((double)x_err_argment[x][0] / 255.0), (1.0 / gamma)) * 255.0;
			//std::cout << x_err_argment[x][0]<< std::endl;
			////G値の処理を行う
			//x_err_argment[x][1] = std::pow(((double)x_err_argment[x][1] / 255.0), (1.0 / gamma)) * 255.0;
			//std::cout << x_err_argment[x][1] << std::endl;
			////R値の処理を行う
			//x_err_argment[x][2] = std::pow(((double)x_err_argment[x][2] / 255.0), (1.0 / gamma)) * 255.0;
			//std::cout << x_err_argment[x][2] << std::endl;

			//今後誤差を算出するために，配列に画素値を格納
			x_err.push_back(x_err_argment[x][0]);
			x_err.push_back(x_err_argment[x][1]);
			x_err.push_back(x_err_argment[x][2]);
			
			//最小にするための目的関数f(x)を作成
			for (int l = 0; l < 8; l++)
			{
				//ユークリッド距離を算出する
				int fx_argment = std::pow((P_vec[l][0] - int(x_err[0])), 2) + std::pow((P_vec[l][1] - int(x_err[1])), 2) + std::pow((P_vec[l][2] - int(x_err[2])), 2);
				//std::cout << "(" + std::to_string(P_vec[l][0]) + "," + std::to_string(P_vec[l][1]) + "," + std::to_string(P_vec[l][2]) + ")との"  + "ユークリッド距離は" + std::to_string(fx_argment) << std::endl;
				//インデックスを取得するため，配列に算出したユークリッド距離を格納
				fx.push_back(fx_argment);
			}

			//ユークリッド距離が最小となるインデックスを取得
			std::vector <int>::iterator fx_iter = std::min_element(fx.begin(), fx.end());
			size_t fx_index = std::distance(fx.begin(), fx_iter);
			//std::cout << fx_index << std::endl;

			

			//ユークリッド距離が短いインデックスを取得し，変数l_impに格納
			int l_imp = fx_index;
			//量子化後の出力画素値を格納する
			x_vec.push_back(P_vec[fx_index][0]);
			x_vec.push_back(P_vec[fx_index][1]);
			x_vec.push_back(P_vec[fx_index][2]);
			

			//量子化された画素値を原画像に置き換える
			x_err_argment[x][0] = P_vec[fx_index][0];
			x_err_argment[x][1] = P_vec[fx_index][1];
			x_err_argment[x][2] = P_vec[fx_index][2];

			//誤差を配列に格納する
			e_i.push_back(x_err[0] - x_vec[0]);
			e_i.push_back(x_err[1] - x_vec[1]);
			e_i.push_back(x_err[2] - x_vec[2]);
			

			//誤差を隣接４ピクセルん拡散させる処理

			//左側のピクセル時の処理--------------------------------------------------
			if (x == 0 && y < frame1.rows - 1)
			{
				//右側のピクセル処理について処理
				//B値の処理
				x_err_argment[x + 1][0] = int(x_err_argment[x + 1][0]) + int(e_i[0] * error_p_1[0]);
				//G値の処理
				x_err_argment[x + 1][1] = int(x_err_argment[x + 1][1]) + int(e_i[1] * error_p_1[0]);
				//R値の処理
				x_err_argment[x + 1][2] = int(x_err_argment[x + 1][2]) + int(e_i[2] * error_p_1[0]);

				//真下のピクセル処理について
				cv::Vec3i* under_x_err_argment = frame1.ptr<cv::Vec3i>(y + 1); //拡散させる際の下３ピクセルにアクセス
				//B値の処理
				under_x_err_argment[x][0] = int(under_x_err_argment[x][0]) + int(e_i[0] * error_p_1[1]);
				//G値の処理
				under_x_err_argment[x][1] = int(under_x_err_argment[x][1]) + int(e_i[1] * error_p_1[1]);
				//R値の処理
				under_x_err_argment[x][2] = int(under_x_err_argment[x][2]) + int(e_i[2] * error_p_1[1]);

				//右下のピクセル処理について
				//B値の処理
				under_x_err_argment[x + 1][0] = int(under_x_err_argment[x + 1][0]) + int(e_i[0] * error_p_1[2]);
				//G値の処理
				under_x_err_argment[x + 1][1] = int(under_x_err_argment[x + 1][1]) + int(e_i[1] * error_p_1[2]);
				//R値の処理
				under_x_err_argment[x + 1][2] = int(under_x_err_argment[x + 1][2]) + int(e_i[2] * error_p_1[2]);	
			}
			//-------------------------------------------------------------------------------------------------------------------------
			
			//一番下のピクセルの処理----------------------------------------------------------------------------------------------
			else if (y == frame1.rows - 1 && x < frame1.cols - 1)
			{
				//右側のピクセル処理について処理
				//B値の処理
				x_err_argment[x + 1][0] = int(x_err_argment[x + 1][0]) + int(e_i[0]);
				//G値の処理
				x_err_argment[x + 1][1] = int(x_err_argment[x + 1][1]) + int(e_i[1]);
				//R値の処理
				x_err_argment[x + 1][2] = int(x_err_argment[x + 1][2]) + int(e_i[2]);
			}
			//--------------------------------------------------------------------------------------------------------------------
			
			//一番右側のピクセル時の処理
			else if (x == frame1.cols - 1 && y < frame1.rows - 1)
			{
				//左下のピクセル処理
				cv::Vec3i* under_x_err_argment = frame1.ptr<cv::Vec3i>(y + 1); //拡散させる際の下３ピクセルにアクセス
				//B値の処理
				under_x_err_argment[x - 1][0] = int(under_x_err_argment[x - 1][0]) + int(e_i[0] * error_p_2[1]);
				//G値の処理
				under_x_err_argment[x - 1][1] = int(under_x_err_argment[x - 1][1]) + int(e_i[1] * error_p_2[1]);
				//R値の処理
				under_x_err_argment[x - 1][2] = int(under_x_err_argment[x - 1][2]) + int(e_i[2] * error_p_2[1]);

				//真下のピクセル処理
				//B値の処理
				under_x_err_argment[x][0] = int(under_x_err_argment[x][0]) + int(e_i[0] * error_p_2[0]);
				//G値の処理
				under_x_err_argment[x][1] = int(under_x_err_argment[x][1]) + int(e_i[1] * error_p_2[0]);
				//R値の処理
				under_x_err_argment[x][2] = int(under_x_err_argment[x][2]) + int(e_i[2] * error_p_2[0]);
			}

			//右下端は処理を行わない
			else if (x == frame1.cols - 1 && y == frame1.rows - 1)
			{
				//B値の処理
				x_err_argment[x][0] = x_err_argment[x][0];
				//G値の処理
				x_err_argment[x][1] = x_err_argment[x][1];
				//R値の処理
				x_err_argment[x][2] = x_err_argment[x][2];
			}

			else
			{
				//右隣のピクセル処理
				//B値の処理
				x_err_argment[x + 1][0] = int(x_err_argment[x + 1][0]) + int(e_i[0] * error_p[0]);
				//G値の処理
				x_err_argment[x + 1][1] = int(x_err_argment[x + 1][1]) + int(e_i[1] * error_p[0]);
				//R値の処理
				x_err_argment[x + 1][2] = int(x_err_argment[x + 1][2]) + int(e_i[2] * error_p[0]);

				//左下のピクセル処理
				cv::Vec3i* under_x_err_argment = frame1.ptr<cv::Vec3i>(y + 1); //拡散させる際の下３ピクセルにアクセス
				//B値の処理
				under_x_err_argment[x - 1][0] = int(under_x_err_argment[x - 1][0]) + int(e_i[0] * error_p[1]);
				//G値の処理
				under_x_err_argment[x - 1][1] = int(under_x_err_argment[x - 1][1]) + int(e_i[1] * error_p[1]);
				//R値の処理
				under_x_err_argment[x - 1][2] = int(under_x_err_argment[x - 1][2]) + int(e_i[2] * error_p[1]);

				//真下のピクセル処理
				//B値の処理
				under_x_err_argment[x][0] = int(under_x_err_argment[x][0]) + int(e_i[0] * error_p[2]);
				//G値の処理
				under_x_err_argment[x][1] = int(under_x_err_argment[x][1]) + int(e_i[1] * error_p[2]);
				//R値の処理
				under_x_err_argment[x][2] = int(under_x_err_argment[x][2]) + int(e_i[2] * error_p[2]);

				//右下のピクセル処理
				//B値の処理
				under_x_err_argment[x + 1][0] = int(under_x_err_argment[x + 1][0]) + int(e_i[0] * error_p[3]);
				//G値の処理
				under_x_err_argment[x + 1][1] = int(under_x_err_argment[x + 1][1]) + int(e_i[1] * error_p[3]);
				//R値の処理
				under_x_err_argment[x + 1][2] = int(under_x_err_argment[x + 1][2]) + int(e_i[2] * error_p[3]);


			}
			//配列の初期化をする
			x_err.clear();
			fx.clear();
			x_vec.clear();
			e_i.clear();

			count = count + 1;
			//コメント文で出力結果を確認する
			//std::cout << "x_err = " << "{" + std::to_string(x_err_argment[x][0] ) + " , " + std::to_string(x_err_argment[x][1]) + " , " + std::to_string(x_err_argment[x][2]) + "}" << std::endl;
			// //std::cout << "ユークリッド距離が最小となるのは" + std::to_string(fx_index) + "番目である" <<  std::endl;
			//std::cout << "すなわち，l*は" + std::to_string(fx_index) << std::endl;
			//std::cout << "x_vec = {" + std::to_string(x_vec[0]) + " , " + std::to_string(x_vec[1]) + " , " + std::to_string(x_vec[2]) + "}" << std::endl;
			// //std::cout << "誤差 = {" + std::to_string(e_i[0]) + " , " + std::to_string(e_i[1]) + " , " + std::to_string(e_i[2]) + "}";
			//std::cout << std::to_string(count) + "回目の処理が終わりました" << std::endl;
		}
	}
	return frame1;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------


