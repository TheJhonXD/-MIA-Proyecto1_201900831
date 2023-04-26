#ifndef PROYECTO1_GRAPHVIZ_H
#define PROYECTO1_GRAPHVIZ_H

#include "Disks.h" //Tal vez comentar porque ya está incluido en el filesystem.h
#include "FileSystem.h"
#include <math.h>
#include <fstream>

void getDiskGraph(string path, string id);
void createMBRReport(string path, string id);
void createSBReport(const string &path, const string &id);
void createBmIReport(const string &path, const string &id);
void createBmBlockReport(const string &path, const string &id);

#endif //PROYECTO1_GRAPHVIZ_H
