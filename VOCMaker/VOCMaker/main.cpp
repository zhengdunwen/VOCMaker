#include <iostream>
#include <opencv2\opencv.hpp>
#include <io.h>
#include <fstream>
#include <string>
#include <direct.h>
#include <conio.h>
#include <Windows.h>
#include "tinyxml2.h"
#include <direct.h> 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
using namespace cv;
using namespace tinyxml2;

//ͼƬ�ṹ����Ϣ
struct ImageInfo
{
	string strPath;
	string strName;
	long  lFileSize;
	long  lFileHight;
	long  lFileWeight;
};


//�Զ��塰С�ڡ�
bool ImageInfoComp(const ImageInfo &a, const ImageInfo &b){
	return a.lFileHight < b.lFileHight;
}


int stringToInt(string s )
{
	string X;
	for (int i = 0; i < s.length(); ++i)
		if (s[i] != ' ' && s[i] != '[' && s[i] != ']')
		{
			if (s[i] == '.')
			{
				break;
			}
			X = X + s[i];
		}
	return atol(X.c_str());
}

class drawPos
{
private:

	// �������ݽṹ
	typedef struct metaData
	{
		vector<int*> pos;
		Mat img;
		vector<string> labels;
		bool isAnnotated;
		string path;
		string name;
		metaData()
		{
			isAnnotated = false;

		}
	};

	typedef struct metaData_no_img
	{
		vector<int*> pos;
		vector<string> labels;
		bool isAnnotated;
		string path;
		string name;
		long lhight;
		long lweight;
		metaData_no_img()
		{
			isAnnotated = false;

		}
		void clear()
		{
			pos.clear();
			labels.clear();
		};
	};



public:
	explicit drawPos(string filePath)
	{
		//getAllFiles(filePath);
		m_filepath = filePath;
		
		//sort(verImageInfo.begin(), verImageInfo.end(), ImageInfoComp);//�����ļ��ĸ߶�����
		//readAllImages2();
		//readAllImages_checkToImage();
		//readMoreImages_checkToOneImage();//���ļ�ѹ����һ���ļ��еķ���

	}

	void outPutXmlFile()
	{
		size_t cnt = 0;
		for (int j = 0; j < data.size(); ++j)
		{
			if (data[j].isAnnotated)
			{
				cnt++;

				tinyxml2::XMLDocument xmlDoc;
				XMLNode * annotation = xmlDoc.NewElement("annotation");
				xmlDoc.InsertFirstChild(annotation);

				XMLElement * pElement = xmlDoc.NewElement("folder");
				pElement->SetText("VOCType");
				annotation->InsertFirstChild(pElement);

				pElement = xmlDoc.NewElement("filename");
				pElement->SetText(data[j].name.c_str());
				annotation->InsertEndChild(pElement);

				pElement = xmlDoc.NewElement("source");
				XMLElement * pElement_sub = xmlDoc.NewElement("database");
				pElement_sub->SetText("VOC");
				pElement->InsertFirstChild(pElement_sub);
				annotation->InsertEndChild(pElement);

				pElement = xmlDoc.NewElement("size");
				pElement_sub = xmlDoc.NewElement("width");
				pElement_sub->SetText(data[j].img.size().width);
				pElement->InsertFirstChild(pElement_sub);
				pElement_sub = xmlDoc.NewElement("height");
				pElement_sub->SetText(data[j].img.size().height);
				pElement->InsertEndChild(pElement_sub);
				pElement_sub = xmlDoc.NewElement("depth");
				pElement_sub->SetText(data[j].img.channels());
				pElement->InsertEndChild(pElement_sub);
				annotation->InsertEndChild(pElement);

				pElement = xmlDoc.NewElement("segmented"); // �Ƿ�ָ�
				pElement->SetText(0);
				annotation->InsertEndChild(pElement);

				for (int k = 0; k < data[j].labels.size(); ++k)
				{
					pElement = xmlDoc.NewElement("object");
					pElement_sub = xmlDoc.NewElement("name"); // ���
					pElement_sub->SetText(data[j].labels[k].c_str());
					pElement->InsertFirstChild(pElement_sub);

					pElement_sub = xmlDoc.NewElement("pose"); // ��̬
					pElement_sub->SetText("Unspecified");
					pElement->InsertEndChild(pElement_sub);

					pElement_sub = xmlDoc.NewElement("truncated");
					pElement_sub->SetText(0);
					pElement->InsertEndChild(pElement_sub);

					pElement_sub = xmlDoc.NewElement("difficult");
					pElement_sub->SetText(0);
					pElement->InsertEndChild(pElement_sub);

					pElement_sub = xmlDoc.NewElement("bndbox");
					XMLElement* pElement_sub_sub = xmlDoc.NewElement("xmin");
					pElement_sub_sub->SetText(data[j].pos[k][0]);
					pElement_sub->InsertFirstChild(pElement_sub_sub);
					pElement_sub_sub = xmlDoc.NewElement("ymin");
					pElement_sub_sub->SetText(data[j].pos[k][1]);
					pElement_sub->InsertEndChild(pElement_sub_sub);
					pElement_sub_sub = xmlDoc.NewElement("xmax");
					pElement_sub_sub->SetText(data[j].pos[k][0] + data[j].pos[k][2]);
					pElement_sub->InsertEndChild(pElement_sub_sub);
					pElement_sub_sub = xmlDoc.NewElement("ymax");
					pElement_sub_sub->SetText(data[j].pos[k][1] + data[j].pos[k][3]);
					pElement_sub->InsertEndChild(pElement_sub_sub);
					pElement->InsertEndChild(pElement_sub);

					annotation->InsertEndChild(pElement);

				}

				string filename = m_filepath + "\\Xml\\";
				for (int x = 0; x < data[j].name.length() - 4; ++x)
				{
					filename += data[j].name[x];
				}
				filename += ".xml";
				xmlDoc.SaveFile(filename.c_str());
			}
		}
		system("cls");
		cout << "���xml�ļ����,��" << cnt << "��" << endl;
		// system("python txt.py"); // �Զ����нű�

	}
	void outPutXmlFile_byOne(metaData date_info)
	{
			if (date_info.isAnnotated)
			{
				tinyxml2::XMLDocument xmlDoc;
				XMLNode * annotation = xmlDoc.NewElement("annotation");
				xmlDoc.InsertFirstChild(annotation);

				XMLElement * pElement = xmlDoc.NewElement("folder");
				pElement->SetText("VOCType");
				annotation->InsertFirstChild(pElement);

				pElement = xmlDoc.NewElement("filename");
				pElement->SetText(date_info.name.c_str());
				annotation->InsertEndChild(pElement);

				pElement = xmlDoc.NewElement("source");
				XMLElement * pElement_sub = xmlDoc.NewElement("database");
				pElement_sub->SetText("VOC");
				pElement->InsertFirstChild(pElement_sub);
				annotation->InsertEndChild(pElement);

				pElement = xmlDoc.NewElement("size");
				pElement_sub = xmlDoc.NewElement("width");
				pElement_sub->SetText(date_info.img.size().width);
				pElement->InsertFirstChild(pElement_sub);
				pElement_sub = xmlDoc.NewElement("height");
				pElement_sub->SetText(date_info.img.size().height);
				pElement->InsertEndChild(pElement_sub);
				pElement_sub = xmlDoc.NewElement("depth");
				pElement_sub->SetText(date_info.img.channels());
				pElement->InsertEndChild(pElement_sub);
				annotation->InsertEndChild(pElement);

				pElement = xmlDoc.NewElement("segmented"); // �Ƿ�ָ�
				pElement->SetText(0);
				annotation->InsertEndChild(pElement);

				for (int k = 0; k < date_info.labels.size(); ++k)
				{
					pElement = xmlDoc.NewElement("object");
					pElement_sub = xmlDoc.NewElement("name"); // ���
					pElement_sub->SetText(date_info.labels[k].c_str());
					pElement->InsertFirstChild(pElement_sub);

					pElement_sub = xmlDoc.NewElement("pose"); // ��̬
					pElement_sub->SetText("Unspecified");
					pElement->InsertEndChild(pElement_sub);

					pElement_sub = xmlDoc.NewElement("truncated");
					pElement_sub->SetText(0);
					pElement->InsertEndChild(pElement_sub);

					pElement_sub = xmlDoc.NewElement("difficult");
					pElement_sub->SetText(0);
					pElement->InsertEndChild(pElement_sub);

					pElement_sub = xmlDoc.NewElement("bndbox");
					XMLElement* pElement_sub_sub = xmlDoc.NewElement("xmin");
					pElement_sub_sub->SetText(date_info.pos[k][0]);
					pElement_sub->InsertFirstChild(pElement_sub_sub);
					pElement_sub_sub = xmlDoc.NewElement("ymin");
					pElement_sub_sub->SetText(date_info.pos[k][1]);
					pElement_sub->InsertEndChild(pElement_sub_sub);
					pElement_sub_sub = xmlDoc.NewElement("xmax");
					pElement_sub_sub->SetText(date_info.pos[k][0] + date_info.pos[k][2]);
					pElement_sub->InsertEndChild(pElement_sub_sub);
					pElement_sub_sub = xmlDoc.NewElement("ymax");
					pElement_sub_sub->SetText(date_info.pos[k][1] + date_info.pos[k][3]);
					pElement_sub->InsertEndChild(pElement_sub_sub);
					pElement->InsertEndChild(pElement_sub);

					annotation->InsertEndChild(pElement);

				}

				string filename = m_filepath + "\\Xml\\";
				for (int x = 0; x < date_info.name.length() - 4; ++x)
				{
					filename += date_info.name[x];
				}
				filename += ".xml";
				xmlDoc.SaveFile(filename.c_str());
			}

	}
	void outPutXmlFile_byOne_no_Image(metaData_no_img date_info)
	{
		if (date_info.isAnnotated)
		{
			tinyxml2::XMLDocument xmlDoc;
			XMLNode * annotation = xmlDoc.NewElement("annotation");
			xmlDoc.InsertFirstChild(annotation);

			XMLElement * pElement = xmlDoc.NewElement("folder");
			pElement->SetText("VOCType");
			annotation->InsertFirstChild(pElement);

			pElement = xmlDoc.NewElement("filename");
			pElement->SetText(date_info.name.c_str());
			annotation->InsertEndChild(pElement);

			pElement = xmlDoc.NewElement("source");
			XMLElement * pElement_sub = xmlDoc.NewElement("database");
			pElement_sub->SetText("VOC");
			pElement->InsertFirstChild(pElement_sub);
			annotation->InsertEndChild(pElement);

			if (date_info.lhight != 0 && date_info.lweight != 0)
			{
		
				pElement = xmlDoc.NewElement("size");
				pElement_sub = xmlDoc.NewElement("width");
				pElement_sub->SetText(date_info.lweight);
				pElement->InsertFirstChild(pElement_sub);
				pElement_sub = xmlDoc.NewElement("height");
				pElement_sub->SetText(date_info.lhight);
				pElement->InsertEndChild(pElement_sub);
				pElement_sub = xmlDoc.NewElement("depth");
				pElement_sub->SetText(3);
				pElement->InsertEndChild(pElement_sub);
				annotation->InsertEndChild(pElement);
			}
			else
			{
				Mat img = imread(date_info.path);
				pElement = xmlDoc.NewElement("size");
				pElement_sub = xmlDoc.NewElement("width");
				pElement_sub->SetText(img.size().width);
				pElement->InsertFirstChild(pElement_sub);
				pElement_sub = xmlDoc.NewElement("height");
				pElement_sub->SetText(img.size().height);
				pElement->InsertEndChild(pElement_sub);
				pElement_sub = xmlDoc.NewElement("depth");
				pElement_sub->SetText(img.channels());
				pElement->InsertEndChild(pElement_sub);
				annotation->InsertEndChild(pElement);
			}
			


			pElement = xmlDoc.NewElement("segmented"); // �Ƿ�ָ�
			pElement->SetText(0);
			annotation->InsertEndChild(pElement);

			for (int k = 0; k < date_info.labels.size(); ++k)
			{
				pElement = xmlDoc.NewElement("object");
				pElement_sub = xmlDoc.NewElement("name"); // ���
				pElement_sub->SetText(date_info.labels[k].c_str());
				pElement->InsertFirstChild(pElement_sub);

				pElement_sub = xmlDoc.NewElement("pose"); // ��̬
				pElement_sub->SetText("Unspecified");
				pElement->InsertEndChild(pElement_sub);

				pElement_sub = xmlDoc.NewElement("truncated");
				pElement_sub->SetText(0);
				pElement->InsertEndChild(pElement_sub);

				pElement_sub = xmlDoc.NewElement("difficult");
				pElement_sub->SetText(0);
				pElement->InsertEndChild(pElement_sub);

				pElement_sub = xmlDoc.NewElement("bndbox");
				XMLElement* pElement_sub_sub = xmlDoc.NewElement("xmin");
				pElement_sub_sub->SetText(date_info.pos[k][0]);
				pElement_sub->InsertFirstChild(pElement_sub_sub);
				pElement_sub_sub = xmlDoc.NewElement("ymin");
				pElement_sub_sub->SetText(date_info.pos[k][1]);
				pElement_sub->InsertEndChild(pElement_sub_sub);
				pElement_sub_sub = xmlDoc.NewElement("xmax");
				pElement_sub_sub->SetText(date_info.pos[k][0] + date_info.pos[k][2]);
				pElement_sub->InsertEndChild(pElement_sub_sub);
				pElement_sub_sub = xmlDoc.NewElement("ymax");
				pElement_sub_sub->SetText(date_info.pos[k][1] + date_info.pos[k][3]);
				pElement_sub->InsertEndChild(pElement_sub_sub);
				pElement->InsertEndChild(pElement_sub);

				annotation->InsertEndChild(pElement);

			}

			string filename = m_filepath + "\\Xml\\";
			int exlen = 4;
			string filetype = "jpeg";
			int findpos = date_info.name.find(filetype);
			if (findpos> 0)
				exlen = 5;
			else
				exlen = 4;
			for (int x = 0; x < date_info.name.length() - exlen; ++x)
			{
				filename += date_info.name[x];
			}
			filename += ".xml";
			xmlDoc.SaveFile(filename.c_str());
		}

	}

	void readInformationText(string Filepath)
	{
		map<string, metaData_no_img> PosInfo;

		ifstream infile;
		infile.open(Filepath);
		if (!infile)
			cout << "error" << endl;

		//x2, y1,x1, y1,x1, y2,x2, y2
		string path,strDnaNum, strType, x2_y1, x1_y1, x1_y2, x2_y2,str;//ÿһ��

		while (getline(infile, path))   //���ж�ȡ,�������з�����
		{
			getline(infile, strDnaNum);
			getline(infile, strType);
			getline(infile, x2_y1);
			if (x2_y1 == "[]")
			{
				continue;
			}
			getline(infile, str);
			getline(infile, x1_y1);
			getline(infile, str);
			getline(infile, x1_y2);
			getline(infile, str);
			getline(infile, x2_y2);

			// [[  656.68359375   849.48797607]]
			// [[  726.10150146  1000.64202881]]]
			int x1 = stringToInt(x1_y1.substr(0, x1_y1.length() / 2));
			int y1 = stringToInt(x1_y1.substr(x1_y1.length() / 2));
			int x2 = stringToInt(x2_y2.substr(0, x2_y2.length() / 2));
			int y2 = stringToInt(x2_y2.substr(x2_y2.length() / 2));
			int *pos = new int[4];
			pos[0] = x1;
			pos[1] = y1;
			pos[2] = x2 - x1;
			pos[3] = y2 - y1;

			//���֮ǰ�Ѿ��洢���ļ�ֻ��Ҫ�����ļ��ĵ�λ�þͿ�����
			map<string, metaData_no_img>::iterator iterPath = PosInfo.find(path);
			if (iterPath != PosInfo.end())
			{

				iterPath->second.pos.push_back(pos);
				iterPath->second.labels.push_back(strDnaNum);
				iterPath->second.isAnnotated = true;

			}
			else
			{
			   //������Ӧ���ļ�����Ӧ��Ŀ¼
				char srcdir[256];
				char desdir[256];
				char cmdstr[1000];
				strcpy(srcdir, path.c_str());
				strcpy(desdir, "D:\\DNA_BACK\\XTEST\\VOC2012\\JPEGImages");
				sprintf(cmdstr,  "cmd /c echo D | xcopy /cherky \"%s\" \"%s\" >NUL", srcdir, desdir);
				WinExec(cmdstr, 0);

				metaData_no_img temp_data;
				temp_data.path = path;
				int pos_ = path.find_last_of('\\');
				temp_data.name = path.substr(pos_+1,path.length() - pos_);
				temp_data.lhight = 0;
				temp_data.lweight = 0;
				temp_data.pos.push_back(pos);
				temp_data.labels.push_back(strDnaNum);
				temp_data.isAnnotated = true;

				PosInfo[path] = temp_data;
			}

		}


		map<string, metaData_no_img>::iterator iterM = PosInfo.begin();
		while (iterM != PosInfo.end())
		{
			outPutXmlFile_byOne_no_Image(iterM->second);

			iterM++;

		}


	}
	//
	// �ֶ�����
	//
	void drawByHand()
	{
		namedWindow("drawPos");
		Mat tempImg1, tempImg2;

		for (int i = 0; i < data.size(); ++i)
		{
			system("cls");
			cout << "��" << i + 1 << "��ͼƬ\n";
			data[i].img.copyTo(tempImg1);
			for (int j = 0; j < data[i].pos.size(); ++j)
			{
				DrawRectangle(tempImg1, Rect(data[i].pos[j][0], data[i].pos[j][1], data[i].pos[j][2], data[i].pos[j][3]));
			}

			setMouseCallback("drawPos", on_MouseHandle, (void*)&tempImg1);

			bool drawFlag = false;

			while (1)
			{
				tempImg1.copyTo(tempImg2);
				switch (waitKey(30))
				{
				case 'c': // �����ͼƬ�����б����Ϣ
				{
							  data[i].pos.clear();
							  data[i].labels.clear();
							  data[i].img.copyTo(tempImg1);
							  system("cls");
							  cout << "��ͼƬ��λ���Ѿ����\n";
							  data[i].isAnnotated = false;

				}break;
				case ' ':goto end; // ��һ��
				case 'a': // ������һ��
				{
							  i--;
							  i--;
							  if (i < 0)
								  i = -1;
							  goto end;

				}
				case 'o': // ����ע��Ϣд�뵽xml
				{
// 							size_t cnt = 0;
// 							for (int j = 0; j < data.size(); ++j)
// 							{
// 								if (data[j].isAnnotated)
// 								{
// 									cnt++;
// 
// 									tinyxml2::XMLDocument xmlDoc;
// 									XMLNode * annotation = xmlDoc.NewElement("annotation");
// 									xmlDoc.InsertFirstChild(annotation);
// 
// 									XMLElement * pElement = xmlDoc.NewElement("folder");
// 									pElement->SetText("VOCType");
// 									annotation->InsertFirstChild(pElement);
// 
// 									pElement = xmlDoc.NewElement("filename");
// 									pElement->SetText(data[j].name.c_str());
// 									annotation->InsertEndChild(pElement);
// 
// 									pElement = xmlDoc.NewElement("source");
// 									XMLElement * pElement_sub = xmlDoc.NewElement("database");
// 									pElement_sub->SetText("VOC");
// 									pElement->InsertFirstChild(pElement_sub);
// 									annotation->InsertEndChild(pElement);
// 
// 									pElement = xmlDoc.NewElement("size");
// 									pElement_sub = xmlDoc.NewElement("width");
// 									pElement_sub->SetText(data[j].img.size().width);
// 									pElement->InsertFirstChild(pElement_sub);
// 									pElement_sub = xmlDoc.NewElement("height");
// 									pElement_sub->SetText(data[j].img.size().height);
// 									pElement->InsertEndChild(pElement_sub);
// 									pElement_sub = xmlDoc.NewElement("depth");
// 									pElement_sub->SetText(data[j].img.channels());
// 									pElement->InsertEndChild(pElement_sub);
// 									annotation->InsertEndChild(pElement);
// 
// 									pElement = xmlDoc.NewElement("segmented"); // �Ƿ�ָ�
// 									pElement->SetText(0);
// 									annotation->InsertEndChild(pElement);
// 
// 									for (int k = 0; k < data[j].labels.size(); ++k)
// 									{
// 										pElement = xmlDoc.NewElement("object");
// 										pElement_sub = xmlDoc.NewElement("name"); // ���
// 										pElement_sub->SetText(data[j].labels[k].c_str());
// 										pElement->InsertFirstChild(pElement_sub);
// 
// 										pElement_sub = xmlDoc.NewElement("pose"); // ��̬
// 										pElement_sub->SetText("Unspecified");
// 										pElement->InsertEndChild(pElement_sub);
// 
// 										pElement_sub = xmlDoc.NewElement("truncated");
// 										pElement_sub->SetText(0);
// 										pElement->InsertEndChild(pElement_sub);
// 
// 										pElement_sub = xmlDoc.NewElement("difficult");
// 										pElement_sub->SetText(0);
// 										pElement->InsertEndChild(pElement_sub);
// 
// 										pElement_sub = xmlDoc.NewElement("bndbox");
// 										XMLElement* pElement_sub_sub = xmlDoc.NewElement("xmin");
// 										pElement_sub_sub->SetText(data[j].pos[k][0]);
// 										pElement_sub->InsertFirstChild(pElement_sub_sub);
// 										pElement_sub_sub = xmlDoc.NewElement("ymin");
// 										pElement_sub_sub->SetText(data[j].pos[k][1]);
// 										pElement_sub->InsertEndChild(pElement_sub_sub);
// 										pElement_sub_sub = xmlDoc.NewElement("xmax");
// 										pElement_sub_sub->SetText(data[j].pos[k][0] + data[j].pos[k][2]);
// 										pElement_sub->InsertEndChild(pElement_sub_sub);
// 										pElement_sub_sub = xmlDoc.NewElement("ymax");
// 										pElement_sub_sub->SetText(data[j].pos[k][1] + data[j].pos[k][3]);
// 										pElement_sub->InsertEndChild(pElement_sub_sub);
// 										pElement->InsertEndChild(pElement_sub);
// 
// 										annotation->InsertEndChild(pElement);
// 
// 									}
// 
// 									string filename = m_filepath + "\\Annotations\\";
// 									for (int x = 0; x < data[j].name.length() - 4; ++x)
// 									{
// 										filename += data[j].name[x];
// 									}
// 									filename += ".xml";
// 									xmlDoc.SaveFile(filename.c_str());
// 								}
// 							}
// 							system("cls");
// 							cout << "���xml�ļ����,��" << cnt << "��" << endl;
// 							// system("python txt.py"); // �Զ����нű�
					outPutXmlFile();

				}break;
				} // end switch
				/*
				grectangle = cv::Rect(0, 0, 200, 200);
				int *pos = new int[4];
				pos[0] = grectangle.x; pos[1] = grectangle.y;
				pos[2] = grectangle.width; pos[3] = grectangle.height;
				data[i].pos.push_back(pos);
				string label = data[i].name.c_str();
				label = label[0];
				cout << "�����ǩ:";
				//cin >> label;
				cout << "OK,����ı�ǩ�ǣ�" << label << endl;
				data[i].labels.push_back(label);
				data[i].isAnnotated = true;
				goto end; // ��һ��
			*/
				if (drawingBox)
				{
					DrawRectangle(tempImg2, grectangle);
					drawFlag = true;

				}
				else
				{
					if (drawFlag&&grectangle.width>5 && grectangle.height>5)
					{
						drawFlag = false;

						int *pos = new int[4];
						pos[0] = grectangle.x; pos[1] = grectangle.y;
						pos[2] = grectangle.width; pos[3] = grectangle.height;
						data[i].pos.push_back(pos);


						system("cls");

						cout << data[i].pos[data[i].pos.size() - 1][0] << "," << data[i].pos[data[i].pos.size() - 1][1] << "," << data[i].pos[data[i].pos.size() - 1][2]
							<< "," << data[i].pos[data[i].pos.size() - 1][3] << endl;
						string label;
						cout << "�����ǩ:";
						cin >> label;
						cout << "OK,����ı�ǩ�ǣ�" << label << endl;
						data[i].labels.push_back(label);
						data[i].isAnnotated = true;

					}
				}

				imshow("drawPos", tempImg2);
			} // end while

		end:;
		}
		outPutXmlFile();
	}

	void drawByHand2()
	{
		namedWindow("drawPos");
		Mat tempImg1, tempImg2;

		for (int i = 0; i < data.size(); ++i)
		{
			system("cls");
			cout << "��" << i + 1 << "��ͼƬ\n";
			data[i].img.copyTo(tempImg1);
			for (int j = 0; j < data[i].pos.size(); ++j)
			{
				DrawRectangle(tempImg1, Rect(data[i].pos[j][0], data[i].pos[j][1], data[i].pos[j][2], data[i].pos[j][3]));
			}

			setMouseCallback("drawPos", on_MouseHandle, (void*)&tempImg1);

			bool drawFlag = false;

			while (1)
			{
				tempImg1.copyTo(tempImg2);
				switch (waitKey(30))
				{
				case 'c': // �����ͼƬ�����б����Ϣ
				{
					data[i].pos.clear();
					data[i].labels.clear();
					data[i].img.copyTo(tempImg1);
					system("cls");
					cout << "��ͼƬ��λ���Ѿ����\n";
					data[i].isAnnotated = false;

				}break;
				case ' ':goto end; // ��һ��
				case 'a': // ������һ��
				{
					i--;
					i--;
					if (i < 0)
						i = -1;
					goto end;

				}
				case 'o': // ����ע��Ϣд�뵽xml
				{
					
					outPutXmlFile();

				}break;
				} // end switch
				/*
				grectangle = cv::Rect(0, 0, 200, 200);
				int *pos = new int[4];
				pos[0] = grectangle.x; pos[1] = grectangle.y;
				pos[2] = grectangle.width; pos[3] = grectangle.height;
				data[i].pos.push_back(pos);
				string label = data[i].name.c_str();
				label = label[0];
				cout << "�����ǩ:";
				//cin >> label;
				cout << "OK,����ı�ǩ�ǣ�" << label << endl;
				data[i].labels.push_back(label);
				data[i].isAnnotated = true;
				goto end; // ��һ��
				*/
				if (drawingBox)
				{
					DrawRectangle(tempImg2, grectangle);
					drawFlag = true;

				}
				else
				{
					if (drawFlag&&grectangle.width>5 && grectangle.height>5)
					{
						drawFlag = false;

						int *pos = new int[4];
						pos[0] = grectangle.x; pos[1] = grectangle.y;
						pos[2] = grectangle.width; pos[3] = grectangle.height;
						data[i].pos.push_back(pos);


						system("cls");

						cout << data[i].pos[data[i].pos.size() - 1][0] << "," << data[i].pos[data[i].pos.size() - 1][1] << "," << data[i].pos[data[i].pos.size() - 1][2]
							<< "," << data[i].pos[data[i].pos.size() - 1][3] << endl;
						string label;
						cout << "�����ǩ:";
						cin >> label;
						cout << "OK,����ı�ǩ�ǣ�" << label << endl;
						data[i].labels.push_back(label);
						data[i].isAnnotated = true;

					}
				}

				imshow("drawPos", tempImg2);
			} // end while

		end:;
		}
		outPutXmlFile();
	}
private:

	//
	// �������ͼƬ�ľ���·��
	//
	void getAllFiles(string path, string fileType = ".jpg")
	{
		// �ļ����
		long hFile = 0;
		// �ļ���Ϣ
		struct _finddata_t fileinfo;

		string p;
		ImageInfo img_info = {};

		if ((hFile = _findfirst(p.assign(path).append("\\*" + fileType).c_str(), &fileinfo)) != -1)
		{
			
			do {
				string namefile = fileinfo.name;
				int ipos = namefile.find("+0.jpg");
				if (ipos < 0)
				{
					continue;
				}
				// �����ļ���ȫ·��
				imgPath.push_back(p.assign(path).append("\\").append(fileinfo.name));
				imgName.push_back(fileinfo.name);

				//��ȡͼ����Ϣ
				img_info.strPath = p;
				img_info.strName = fileinfo.name;
				img_info.lFileSize = fileinfo.size;
				IplImage* b = cvLoadImage(p.c_str());
				img_info.lFileWeight = b->width;
				img_info.lFileHight = b->height;
				cvReleaseImage(&b);//��������ڴ�й©
				verImageInfo.push_back(img_info);

			} while (_findnext(hFile, &fileinfo) == 0);  //Ѱ����һ�����ɹ�����0������-1

			_findclose(hFile);
		}
	}

	//
	// ��ȡ���е�ͼƬ
	//
	size_t readAllImages()
	{
		vector<string>::iterator begin = imgPath.begin();

		for (int i = 0; i < imgPath.size(); ++i, ++begin)
		{
			metaData temp_data;
			Mat tempImg = imread(imgPath[i]);
			if (!tempImg.empty())
			{
				temp_data.img = tempImg.clone();
				temp_data.path = imgPath[i];
				temp_data.name = imgName[i];
				data.push_back(temp_data);
			}
			else
			{
				imgPath.erase(begin);
				begin = imgPath.begin();
				begin += i;
			}
		}
		return data.size();
	}
	size_t readAllImages2()
	{
		vector<string>::iterator begin = imgPath.begin();
		
		for (int i = 0; i < imgPath.size(); ++i, ++begin)
		{
			metaData temp_data;
			Mat tempImg = imread(imgPath[i]);
			if (!tempImg.empty())
			{
				temp_data.img = tempImg.clone();
				temp_data.path = imgPath[i];
				temp_data.name = imgName[i];

				int *pos = new int[4];
				pos[0] = 0;
				pos[1] = 0;
				pos[2] = tempImg.cols;
				pos[3] = tempImg.rows;
				temp_data.pos.push_back(pos);


				string strFilename = imgPath[i].c_str();
				string::size_type Pos = strFilename.find('+');

				if (Pos != string::npos)
				{
					string::size_type Pos2 = strFilename.find('+', Pos+1);
					string label = strFilename.substr(Pos+1, Pos2 - Pos-1);
					temp_data.labels.push_back(label);
					temp_data.isAnnotated = true;
				}
				

				data.push_back(temp_data);
			}
			else
			{
				imgPath.erase(begin);
				begin = imgPath.begin();
				begin += i;
			}
		}
		return data.size();
	}


	void CreateImageAndReturnSiez(string imgPath, int *pos, string imgName)
	{
		IplImage *cvImg; //IplImage��OpenCV�ĵײ�ͼ��ָ��
		CvSize imgSize;

		imgSize.width = 1232;
		imgSize.height = 912;
		cvImg = cvCreateImage(imgSize, IPL_DEPTH_8U, 3); //������һ��640*480��ͼ�����Ϊ8��1ͨ��RGBͼ�� 
		cvSet(cvImg/*ͼƬ*/, CV_RGB(255, 255, 255)/*��ɫ*/, NULL/*���c++����ʡ��*/);
		IplImage* b = cvLoadImage(imgPath.c_str());

		{
			//����ROL���ο飨����Ȥ������������aͼ�ı�ճ������  
			CvRect rect;
			//rect.x = Random(0, imgSize.width - b->width);
			//rect.y = Random(0, imgSize.height - b->height);
			rect.x = 10;
			rect.y = 10;
			//ROI���Ĵ�СΪbͼ�Ĵ�С  
			rect.width = b->width;
			rect.height = b->height;
			//�ڰ�ͼ�м���ROI��  
			cvSetImageROI(cvImg, rect);
			//��bͼճ����aͼ�е�ROI��  
			cvCopy(b, cvImg);
			//�ͷ�ROI����������ʾ����aͼ  
			cvResetImageROI(cvImg);

			pos[0] = rect.x;
			pos[1] = rect.y;
			pos[2] = b->width;
			pos[3] = b->height;

		}
		string filename = m_filepath + "\\MaxImag\\" + imgName;
		cvSaveImage(filename.c_str(), cvImg);
		cvReleaseImage(&cvImg);
		cvReleaseImage(&b);
	}
	

	//��ȡ1Сͼ����ת����һ�Ŵ�ͼ
	size_t readAllImages_checkToImage()
	{
		vector<string>::iterator begin = imgPath.begin();

		for (int i = 0; i < imgPath.size(); ++i, ++begin)
		{
			metaData temp_data;
			Mat tempImg = imread(imgPath[i]);
			if (!tempImg.empty())
			{
				temp_data.img = tempImg.clone();
				temp_data.path = imgPath[i];
				temp_data.name = imgName[i];

				int *pos = new int[4];
				CreateImageAndReturnSiez(imgPath[i], pos, imgName[i]);
				temp_data.pos.push_back(pos);

				string strFilename = imgPath[i].c_str();
				string::size_type Pos = strFilename.find('+');

				if (Pos != string::npos)
				{
					string::size_type Pos2 = strFilename.find('+', Pos + 1);
					string label = strFilename.substr(Pos + 1, Pos2 - Pos - 1);
					temp_data.labels.push_back(label);
					temp_data.isAnnotated = true;
				}

				outPutXmlFile_byOne(temp_data);
				delete[] pos;
				//data.push_back(temp_data);
			}
			else
			{
				imgPath.erase(begin);
				begin = imgPath.begin();
				begin += i;
			}
		}
		return data.size();
	}


	//��ȡ����Сͼ����ת����һ�Ŵ�ͼ
	size_t readMoreImages_checkToOneImage()
	{
		vector<ImageInfo>::iterator begin = verImageInfo.begin();

		metaData_no_img temp_data = {};
		CvSize imgSize;
		imgSize.width = 416;
		imgSize.height = 416;


		IplImage *cvImg = nullptr; //IplImage��OpenCV�ĵײ�ͼ��ָ��
		cvImg = cvCreateImage(imgSize, IPL_DEPTH_8U, 3); //������һ��640*480��ͼ�����Ϊ8��1ͨ��RGBͼ�� 
		cvSet(cvImg/*ͼƬ*/, CV_RGB(255, 255, 255)/*��ɫ*/, NULL/*���c++����ʡ��*/);


		int aline = 5;
		int Xpos = aline, Ypos = aline;
		int iMaxHeight = 0;

		for (int i =0 ; begin != verImageInfo.end(); i++ )
		{
				temp_data.path = begin->strPath;
				temp_data.name = begin->strName;
				temp_data.lhight = begin->lFileHight;
				temp_data.lweight = begin->lFileWeight;


				
				IplImage* b = cvLoadImage(begin->strPath.c_str());

				bool bnew = false;

				if (b->height > iMaxHeight)
				{
					iMaxHeight = b->height;
				}


				if ((Xpos + b->width + aline) >= imgSize.width || (Ypos + iMaxHeight + aline) >= imgSize.height)
				{
					if ((Ypos + iMaxHeight + aline) >= imgSize.height)
					{
						bnew = true;
						//new 
					}
					else
					{
						Xpos = aline;
						Ypos = Ypos + iMaxHeight + aline;
						iMaxHeight = 0;
					}
					if ((Ypos + b->height + aline) >= imgSize.height)
					{
						bnew = true;
						//new 
					}

				}
				
				if (bnew)
				{
					cout << i << endl;
					cout << temp_data.name << endl;
					string filename = m_filepath + "\\MaxImag\\" + temp_data.name;
					cvSaveImage(filename.c_str(), cvImg);
					cvReleaseImage(&cvImg);

					outPutXmlFile_byOne_no_Image(temp_data);

					Xpos = aline, Ypos = aline;
					temp_data.clear();


					cvImg = cvCreateImage(imgSize, IPL_DEPTH_8U, 3); //������һ��640*480��ͼ�����Ϊ8��1ͨ��RGBͼ�� 
					cvSet(cvImg/*ͼƬ*/, CV_RGB(255, 255, 255)/*��ɫ*/, NULL/*���c++����ʡ��*/);
				}

				int *pos = new int[4]; 
				{
					//����ROL���ο飨����Ȥ������������aͼ�ı�ճ������  
					CvRect rect;
					//rect.x = Random(0, imgSize.width - b->width);
					//rect.y = Random(0, imgSize.height - b->height);
					rect.x = Xpos;
					rect.y = Ypos;
					//ROI���Ĵ�СΪbͼ�Ĵ�С  
					rect.width = b->width;
					rect.height = b->height;


					Xpos = Xpos + b->width + aline;
					
					
					//�ڰ�ͼ�м���ROI��  
					cvSetImageROI(cvImg, rect);
					//��bͼճ����aͼ�е�ROI��  
					cvCopy(b, cvImg);
					//�ͷ�ROI����������ʾ����aͼ  
					cvResetImageROI(cvImg);

					pos[0] = rect.x;
					pos[1] = rect.y;
					pos[2] = b->width;
					pos[3] = b->height;

				}
				cvReleaseImage(&b);





				temp_data.pos.push_back(pos);

				string strFilename = begin->strName.c_str();
				string::size_type Pos = strFilename.find('+');

				if (Pos != string::npos)
				{
					string::size_type Pos2 = strFilename.find('+', Pos + 1);
					string label = strFilename.substr(Pos + 1, Pos2 - Pos - 1);
					temp_data.labels.push_back(label);
					temp_data.isAnnotated = true;
				}

				begin++;

		}

		{
			cout << temp_data.name << endl;
			string filename = m_filepath + "\\MaxImag\\" + temp_data.name;
			cvSaveImage(filename.c_str(), cvImg);
			cvReleaseImage(&cvImg);

			outPutXmlFile_byOne_no_Image(temp_data);

			Xpos = aline, Ypos = aline;
			temp_data.clear();

		}

		return data.size();
	}

	
public:

	//����
	static void DrawRectangle(cv::Mat& img, cv::Rect box)
	{
		cv::rectangle(img, Rect(box.x, box.y, box.width, box.height), Scalar(255, 100, 100), 2, 1);
	}

	//�����¼���Ӧ
	static void on_MouseHandle(int event, int x, int y, int flags, void* param)
	{
		Mat& image = *(cv::Mat*)param;

		switch (event)
		{
		case EVENT_MOUSEMOVE:
		{

								if (drawingBox)
								{
									grectangle.width = x - grectangle.x;
									grectangle.height = y - grectangle.y;
								}

		}break;
		case EVENT_LBUTTONDOWN:
		{

								  drawingBox = true;
								  grectangle = Rect(x, y, 0, 0);

		}break;
		case EVENT_LBUTTONUP:
		{

								drawingBox = false;
								if (grectangle.width < 0)
								{
									grectangle.x += grectangle.width;
									grectangle.width *= -1;
								}
								if (grectangle.height < 0)
								{
									grectangle.y += grectangle.height;
									grectangle.height *= -1;
								}


								DrawRectangle(image, grectangle);

		}break;
		default:break;

		}
	}

	int Random(int m, int n)
	{
		int pos, dis;
		if (m == n)
		{
			return m;
		}
		else if (m > n)
		{
			pos = n;
			dis = m - n + 1;
			return rand() % dis + pos;
		}
		else
		{
			pos = m;
			dis = n - m + 1;
			return rand() % dis + pos;
		}
	}
private:

	// ���е�ͼƬ����·��
	vector<string> imgPath;
	// ����ͼƬ������
	vector<string> imgName;

	vector<ImageInfo>  verImageInfo;//ͼ���ȫ����Ϣ

	string m_filepath;
	//�ṹ������
	vector<metaData> data;

	static cv::Rect grectangle;

	static bool drawingBox;
};

bool drawPos::drawingBox = false;
Rect drawPos::grectangle = Rect(-1, -1, 0, 0);





int main(int argc, char** argv)
{
	/*

	// system("python rename.py"); // �Զ����нű�
	string p = argv[1];
	//"D:\\tensorflow_code\\DNA_TEST\\Date_set_128\\VOC2012";
	string path1 = p + "\\MaxImag";
	mkdir(path1.c_str());
	string path2 = p + "\\Xml";
	mkdir(path2.c_str());

	drawPos t(p);
	//t.outPutXmlFile();
	//t.drawByHand();
	*/

	string p = argv[1];
	drawPos t(p);
	t.readInformationText("D:\\DNA_BACK\\3000DNA\\posInfo.txt");
	return 0;
}