#ifndef PROYECTO1_TOOLS_H
#define PROYECTO1_TOOLS_H

#include <iostream>
#include <string>
#include <filesystem>
#include <algorithm>
#include <sstream>
#include <regex>
#include <math.h>

using namespace std;
namespace fs = filesystem;

string toLowerCase(string s);
string toUpperCase(string s);
string trimEnd(const string &s);
string trimStart(const string &s);
string trim(const string &s);
string replaceSpace(string &s, const string &sym);
string replaceAmpersand(string &s, const string &sym);
vector<string> split(string s, const string del);
string toString(char *name);
string deleteComments(string &s);
string removeQuotes(string s);
int getRandomNumber();
string getPath(fs::path p);
string getFileName(fs::path p);
string getFileExt(fs::path p);
bool createDir(string path);
int getPercentage(double size, double disk_size);

#endif //PROYECTO1_TOOLS_H
