#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <queue>
#include <stdlib.h>
#include <cmath>

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

int loopMarkFlag = 0;
int readFileFlag = 0;

int lineNumMark = 0;

// int bIfRead = 0;
int bCalculate = 0;

double originTimePoint = 0;
double originStudyTimePoint = 0;

ifstream fFile;

string last_line = "";
string current_line = "";
string next_line = "";

deque<string> qLineList;
deque<int> qLoadOriginList;

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

// 是否继续读行号
int ifRead(int pointNum,int lineNo) {
	// if (pointNum == 0) {
	// 	if (lineNo <= 1) {
	// 		return 1;
	// 	} else {
	// 		return 0;
	// 	}
	// } else if (pointNum == 1) {
	// 	if (lineNo <= 3) {
	// 		return 1;
	// 	}
	// } else {
	// 	if (lineNo <= (pointNum+1)) {
	// 		return 1;
	// 	} else {
	// 		return 0;
	// 	}

	// }
	// return 0;
	if (bCalculate == 0) {
		if (pointNum == 0) {
			if (qLineList.size() >= 1) {
				return 0;
			} else {
				return 1;
			}
		}
 	// } else  if (bCalculate == 2) {
	} else {
 		if (lineNo != configMaxLineNo) {
 			if (qLineList.size() >= 2) {
 				return 0;
 			} else {
 				return 1;
 			}
 		} else {
 			return 0;
 		}
 	} 
 	// else if (bCalculate == 1) {
 	// 	return 0;
 	// }

	// if (pointNum == 0) {
	// 	if (qLineList.size() >= 1) {
	// 		return 0;
	// 	} else {
	// 		return 1;
	// 	}
	// } else if (pointNum != configMaxLineNo) {
	// 	if (qLineList.size() >= 2) {
	// 		return 0;
	// 	} else {
	// 		return 1;
	// 	}
	// } else {

	// }

}

int initialFirstLineData(string lineContent) {
	if (lineContent != "") {
		vector<string> vcLine = split(lineContent,",");
		int iLoopMark;
		stringstream ssLoopMark;
		stringstream ssTimePoint;
		ssLoopMark<<vcLine[0];
		ssLoopMark>>iLoopMark;
		if((iLoopMark!= configMaxLoopMark)&&(iLoopMark != -1)) {
			loopMarkFlag = 1;
		} else {
			loopMarkFlag = 0;
		}
		ssTimePoint<<vcLine[2];
		ssTimePoint>>originStudyTimePoint;
	}
}

int readDataFile(string strFilePath) {
	if (strFilePath != "") {
		fFile.open(strFilePath,ios::in);
		if (fFile.is_open()) {
			string strLineContent = "";
			while(getline(fFile,strLineContent)) {
				if (strLineContent != "") {
					while(ifRead(countPointNum,fileLineCount)) {
						if (current_line == "") {
							current_line = strLineContent;
							qLineList.push_back(strLineContent);
							// if (fileLineCount == 0) {
							// 	initialFirstLineData(current_line);
							// }
						}
						fileLineCount++;
						this_thread::sleep_for(chrono::milliseconds(2));
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

	avgNum = 1;
	configAvgTime = 0;
	countPointNum = 0;
	fileLineCount = 0;
	configMaxLoopMark = 0;
	configMaxLineNo = 0;
	configTotalNum = 0;

	loopMarkFlag = 0;

	originTimePoint = 0;

	bCalculate = 0;

	qLineList.clear();

	strFilePath = filePath;
	strConfigPath = strConfigPath;

	readConfigFile(strConfigPath);

	readDataFile(strFilePath);

	return 0;
}

int firstPointData(int loopMark,int sequence, int load, double timedata,int toolNum) {
	originTimePoint = timedata;
	if (loopMark != -1) {
		loopMarkFlag = 1;
	} else {
		loopMarkFlag = 0;
	}
	return 0;	
}

int secondPointData(int loopMark,int sequence, int load, double timedata,int toolNum) {
	double timeSpace; 
	if (originTimePoint != 0) {
		timeSpace = timedata - originTimePoint;
		avgNum = configAvgTime*1000/timeSpace;
	}
	return 0;
}

int getAvgLoad(int load) {
	int retVal = 0;
	int tmpAvgNum = qLoadOriginList.size();
	if (tmpAvgNum )
	if (tmpAvgNum != 0) {
		deque<int>::iterator it;
		qLoadOriginList.begin();
		int tmpSum = 0;
		for (it = qLoadOriginList.begin(); it != qLoadOriginList.end(); ++it) {
			tmpSum = tmpSum + *it;
		}
		
		if (avgNum == 0) {
			retVal = (tmpSum + load)/(tmpAvgNum + 1);
		} else {
			if (tmpAvgNum < avgNum) {
				retVal = (tmpSum + load)/(tmpAvgNum + 1);
			} else {
				retVal = (tmpSum + load)/avgNum;
			}
		}

	} else {
		return load;
	}

	return retVal;
}

int judgeLineNo(int lineNo,int pointNum) {
	if ((pointNum < (configTotalNum)/5)&&(lineNo == configMaxLineNo)) {
		return 0;
	} else {
		return 1;
	}
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
	bCalculate = 1;

	countPointNum++;
	if (configTotalNum != 0) {
		if (countPointNum == 1) {
			//处理第一个输入点
			firstPointData(loopMark,sequence,load,timedata,toolNum);
			retLoad = getAvgLoad(load);
			qLoadOriginList.push_back(load);
			vector<string> tmpLineElement;
			if (qLineList.size() >= 1) {
				tmpLineElement = split(qLineList.front(),",");
				// qLineList.pop_front();
				stringstream ssMax;
				stringstream ssMin;
				ssMax<<tmpLineElement[3];
				ssMax>>retMax;
				ssMin<<tmpLineElement[4];
				ssMin>>retMin;
				retToolNum = toolNum;

				qLineList.pop_front();
				// 将行号标记为第一行已用
				lineNumMark = 1;
			} else {
				retToolNum = -1;
			}
		} else if (countPointNum > 1) {

			if (countPointNum == 2) {
				secondPointData(loopMark,sequence,load,timedata,toolNum);
				// retLoad = getAvgLoad(load);
				// qLoadOriginList.push_back(load);
			}

			retLoad = getAvgLoad(load);
			qLoadOriginList.push_back(load);
			if (qLoadOriginList.size() > avgNum) {
				qLoadOriginList.pop_front();
			}

			if (qLineList.size() >= 2) {
				int markLoopMarkEffect = 0;
				int markLineNoEffect = 0;
				int markTimeStampEffect = 0;

				vector<string> firstLine = split(qLineList.front(),",");
				vector<string> secondLine = split(qLineList.at(1),",");
				int absFirstLoopMark;
				int absFirstLineNo;
				double absFirstTimeStamp;

				int absSecondLoopMark;
				int absSecondLineNo;
				double absSecondTimeStamp;

				stringstream ssFirstLoopMark;
				stringstream ssFirstLineNo;
				stringstream ssFirstTimeStamp;

				stringstream ssSecondLoopMark;
				stringstream ssSecondLineNo;
				stringstream ssSecondTimeStamp;

				ssFirstLoopMark<<firstLine[0];
				ssFirstLoopMark>>absFirstLoopMark;
				ssFirstLineNo<<firstLine[1];
				ssFirstLineNo>>absFirstLineNo;
				ssFirstTimeStamp<<firstLine[2];
				ssFirstTimeStamp>>absFirstTimeStamp;

				ssSecondLoopMark<<secondLine[0];
				ssSecondLoopMark>>absSecondLoopMark;
				ssSecondLineNo<<secondLine[1];
				ssSecondLineNo>>absSecondLineNo;
				ssSecondTimeStamp<<secondLine[2];
				ssSecondTimeStamp>>absSecondTimeStamp;

				// absFirstLoopMark = firstLine[0];
				// absFirstLineNo = firstLine[1];
				// absFirstTimeStamp = firstLine[2];

				// absSecondLoopMark = secondLine[0];
				// absSecondLineNo = secondLine[1];
				// absSecondTimeStamp = secondLine[2];

				// int timegap = abs(absSecondTimeStamp - absFirstTimeStamp);
				// avgNum = configAvgTime/timegap;
				if (loopMarkFlag) {
					if ((abs(absFirstLoopMark - loopMark)) < (abs(absSecondLoopMark - loopMark))) {
						markLoopMarkEffect = 1;
					} else if ((abs(absFirstLoopMark - loopMark)) > (abs(absSecondLoopMark - loopMark))) {
						markLoopMarkEffect = 2;
					} else {
						markLoopMarkEffect = 0;
					}
				} else {
					markLoopMarkEffect = 0;
				}

				// if (loopMarkFlag) {
				// 	if (absFirstLoopMark < absSecondLoopMark) {

				// 	} else if (absFirstLoopMark > absSecondLoopMark) {

				// 	} else {
				// 		markLoopMarkEffect = 0;
				// 	}
				// }

				//判断行号 是否有效
				if (judgeLineNo(sequence,countPointNum)) {
					if ((abs(absFirstLineNo - sequence)) < (abs(absSecondLineNo - sequence))) {
						markLineNoEffect = 1;
					} else if ((abs(absFirstLineNo - sequence)) > (abs(absSecondLineNo - sequence))) {
						markLineNoEffect = 2;
					} else {
						markLineNoEffect = 0;
					}
				} else {
					markLineNoEffect = 0;
				}

				//对比时间戳 有效性
				if((abs(absFirstTimeStamp - timedata)) < (abs(absSecondTimeStamp - timedata))) {
					markTimeStampEffect = 1;
				} else if ((abs(absFirstTimeStamp - timedata)) > (abs(absSecondTimeStamp - timedata))) {
					markTimeStampEffect = 2;
				} else {
					markTimeStampEffect = 0;
				}

				// 根据权值判断 所选择使用行
				if (markLoopMarkEffect == 0) {
					if (markLineNoEffect == 0) {
						// lineNo 无效
					} else {
						if (markTimeStampEffect == 1) {
							stringstream ssMax;
							stringstream ssMin;
							ssMax<<firstLine[3];
							ssMax>>retMax;
							ssMin<<firstLine[4];
							ssMin>>retMin;
							retToolNum = toolNum;


						} else if (markTimeStampEffect == 2) {
							stringstream ssMax;
							stringstream ssMin;
							ssMax<<secondLine[3];
							ssMax>>retMax;
							ssMin<<secondLine[4];
							ssMin>>retMin;
							retToolNum = toolNum;
						} else {
							// 使用第一行数据
							stringstream ssMax;
							stringstream ssMin;
							ssMax<<firstLine[3];
							ssMax>>retMax;
							ssMin<<firstLine[4];
							ssMin>>retMin;
							retToolNum = toolNum;
						}
					}

				} else {
					if (markLoopMarkEffect == 1) {
						//firstLine effect
						stringstream ssMax;
						stringstream ssMin;
						ssMax<<firstLine[3];
						ssMax>>retMax;
						ssMin<<firstLine[4];
						ssMin>>retMin;
						retToolNum = toolNum;
					} else if (markLoopMarkEffect == 2) {
						// secondLine effect
						stringstream ssMax;
						stringstream ssMin;
						ssMax<<secondLine[3];
						ssMax>>retMax;
						ssMin<<secondLine[4];
						ssMin>>retMin;
						retToolNum = toolNum;
					} else {

					}
				}


			} else {
				retToolNum = -1;
			}

		} else if(lineNumMark == configMaxLineNo) {
			if (qLineList.size() >= 1) {
				vector<string> tmpLineElement = split(qLineList.front(),",");
				tmpLineElement = split(qLineList.front(),",");
				// qLineList.pop_front();
				stringstream ssMax;
				stringstream ssMin;
				ssMax<<tmpLineElement[3];
				ssMax>>retMax;
				ssMin<<tmpLineElement[4];
				ssMin>>retMin;
				retToolNum = toolNum;
			} else {
				retToolNum = -1;
			}
			
		} else {

		}
	}

	Json::Value root;
	Json::Value arrayObj; 

	arrayObj["Load"] = retLoad;
	arrayObj["Upper limit"] = retMax;
	arrayObj["Lower limit"] = retMin;
	arrayObj["Toolnum"] = retToolNum;

	root["calculate"].append(arrayObj);

	string strRet = root.toStyledString();

	bCalculate = 2;
	return (char *)strRet.c_str();
}
