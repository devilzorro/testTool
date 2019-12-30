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
int averNum = 1;
int averTime = 0;
int countPointNum = 0;
int fileLineCount = 0;
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
					averTime = root["averTime"].asInt();
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

int readDataFile(string strFilePath) {
	if (strFilePath != "") {
		fFile.open(strFilePath,ios:in);
		if (fFile.is_open()) {
			while() {

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


char *calculate(int loopMark,int sequence, int load, double timedata,int toolnum,
	vector<vector<double>> &limit,
	vector<vector<double>> &countnum,
	vector<vector<double>> &load_ago) 
{
	countPointNum++;
	if (countPointNum == 1) {

	} else if (countPointNum == 2) {

	}

	Json::Value root;

	return "";
}
