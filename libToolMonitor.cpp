#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <queue>

#include "libToolMonitor.h"
#include "json/json.h"

string strFilePath = "";
string strConfigPath = "";
int avgNum = 1;
int configAvgTime = 0;
int countPointNum = 0;
int fileLineCount = 0;
int configMaxLoopMark = 0;
int configMaxLineNo = 0;
int configTotalNum = 0;

ifstream fFile;

string last_line = "";
string current_line = "";
string next_line = "";

vector<string> split(string strContent,string mark) {
    string::size_type pos;
    vector<string> result;
    strContent += mark;
    int size = strContent.size();
    for(int i=0; i<size; i++)
    {
        pos = strContent.find(mark,i);
        if(pos<size)
        {
            string s = strContent.substr(i,pos-i);
            result.push_back(s);
            i = pos+mark.size()-1;
        }
    }
    return result;
}

int readConfigFile(string strConfigPath) {
	ifstream fConfigFile;
	if (strConfigPath != "") {
		fConfigFile.open(strConfigPath);
		if (fConfigFile.is_open()) {
			stringstream ssBuffer;
			ssBuffer<<fConfigFile.rdbuf();
			string strConfigData(ssBuffer.str());
			fConfigFile.close();
			if (strConfigData != "") {
				Json::Value root;
				Json::Reader configReader;
				if (configReader.parse(strConfigData,root)) {
					configAvgTime = root["avgTime"].asInt();
					configMaxLoopMark = root["maxLoopMark"].asInt();
					configMaxLineNo = root["maxLineNo"].asInt();
					configTotalNum = root["total"].asInt();
				}
			}
		} else {
			//file open error
			return -2;
		}
	} else {
		//file path is null
		return -1;
	}
	return 0;
}

int ifRead(int pointNum,int lineNo) {
	if (pointNum == 0) {
		if (lineNo <= 1) {
			return 1;
		} else {

		}
	} else if (pointNum == 1) {

	}
	return 0;
}

int readDataFile(string strFilePath) {
	if (strFilePath != "") {
		fFile.open(strFilePath,ios::in);
		if (fFile.is_open()) {
			string strLineContent = "";
			while(getline(fFile,strLineContent)) {
				fileLineCount++;
				if (strLineContent != "") {
					while(ifRead(countPointNum,fileLineCount)) {
						if (current_line == "") {
							current_line = strLineContent;
						}
						this_thread::sleep_for(chrono::milliseconds(10));
					}
				}
			}

		} else {
			return -2;
		}
	} else {
		return -1;
	}
	return 0;
}


int initial(char *filePath,
	char *configPath,
	vector<vector<double>> &limit,
	vector<vector<double>> &countnum,
	vector<vector<double>> &load_ago) 
{
	if (fFile.is_open())
	{
		fFile.close();
	}

	strFilePath = filePath;
	strConfigPath = strConfigPath;

	readConfigFile(strConfigPath);

	readDataFile(strFilePath);

	return 0;
}


char *calculate(int loopMark,int sequence, int load, double timedata,int toolNum,
	vector<vector<double>> &limit,
	vector<vector<double>> &countnum,
	vector<vector<double>> &load_ago) 
{
	int retLoad;
	int retToolNum;
	double retMax;
	double retMin;
	countPointNum++;
	if (configTotalNum != 0) {
		if (countPointNum == 1) {

		} else if (countPointNum > 1) {
		} else if(countPointNum >= configTotalNum) {

		} else {

		}
	}

	Json::Value root;
	Json::Value arrayObj; 

	arrayObj["Load"] = 1;
	arrayObj["Upper limit"] = 3.5;
	arrayObj["Lower limit"] = 1.5;
	arrayObj["Toolnum"] = 2;

	root["calculate"].append(arrayObj);

	string strRet = root.toStyledString();

	return "";
}
