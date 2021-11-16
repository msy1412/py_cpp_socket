#include <stdio.h>
//#include <Winsock2.h>
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs/legacy/constants_c.h"
#include<iostream>
#include<vector>
#include<winsock.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
using namespace cv;
void initialization();
int main() {
	//���峤�ȱ���
	int send_len = 0;
	int recv_len = 0;
	int len = 0;
	//���巢�ͻ������ͽ��ܻ�����
	char send_buf[100];
	char recv_buf[100];
	//����ͼƬ����
	Mat img_decode;
	vector<uchar>img_data;
	char recvBuf_1[1];
	//���������׽��֣����������׽���
	SOCKET s_server;
	SOCKET s_accept;
	//����˵�ַ�ͻ��˵�ַ
	SOCKADDR_IN server_addr;
	SOCKADDR_IN accept_addr;
	initialization();
	//���������Ϣ
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(8004);
	//�����׽���
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	if (::bind(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		cout << "�׽��ְ�ʧ�ܣ�" << endl;
		WSACleanup();
	}
	else {
		cout << "�׽��ְ󶨳ɹ���" << endl;
	}
	//�����׽���Ϊ����״̬
	if (listen(s_server, SOMAXCONN) < 0) {
		cout << "���ü���״̬ʧ�ܣ�" << endl;
		WSACleanup();
	}
	else {
		cout << "���ü���״̬�ɹ���" << endl;
	}
	cout << "��������ڼ������ӣ����Ժ�...." << endl;
	//������������
	len = sizeof(SOCKADDR);
	s_accept = accept(s_server, (SOCKADDR*)&accept_addr, &len);
	if (s_accept == SOCKET_ERROR) {
		cout << "����ʧ�ܣ�" << endl;
		WSACleanup();
		return 0;
	}
	cout << "���ӽ�����׼����������" << endl;
	//���ݰ�
	struct Data {  //20sl64s
		char filename[20];
		long imgStrSize;
		char boxInfo[64];
		//char recvImg_buf[72518];
	}data;

	//��������
	while (1) {
		memset(&data, 0, sizeof(struct Data)); //���
		recv_len = recv(s_accept, (char*)&data, sizeof(struct Data), 0);
		if (recv_len < 0) {
			cout << "����ʧ�ܣ�" << endl;
			break;
		}
		else {
			//cout << "�ͻ�����Ϣ:" << data.filename << endl;
			img_data.resize(data.imgStrSize); //
		}
		for (int i = 0; i < data.imgStrSize; i++)
		{
			recv(s_accept, recvBuf_1, 1, 0);
			img_data[i] = recvBuf_1[0];
		}
		cout << data.filename << "��ͼƬ֡���ճɹ��� boxinfo:" << data.boxInfo << endl;
		img_decode = imdecode(img_data, CV_LOAD_IMAGE_COLOR);
		imshow("server", img_decode);
		send_len = send(s_accept, "Server has recieved!", 29, 0);
		if (send_len < 0) {
			cout << "����ʧ�ܣ�" << endl;
			break;
		}
		if (waitKey(30) == 27) break;
	}
	//�ر��׽���
	closesocket(s_server);
	closesocket(s_accept);
	//�ͷ�DLL��Դ
	WSACleanup();
	return 0;
}

void initialization() {
	//��ʼ���׽��ֿ�
	WORD w_req = MAKEWORD(2, 2);//�汾��
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		cout << "��ʼ���׽��ֿ�ʧ�ܣ�" << endl;
	}
	else {
		cout << "��ʼ���׽��ֿ�ɹ���" << endl;
	}
	//���汾��
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		cout << "�׽��ֿ�汾�Ų�����" << endl;
		WSACleanup();
	}
	else {
		cout << "�׽��ֿ�汾��ȷ��" << endl;
	}
	//������˵�ַ��Ϣ
}