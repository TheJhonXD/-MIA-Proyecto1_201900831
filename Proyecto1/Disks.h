#ifndef PROYECTO1_DISKS_H
#define PROYECTO1_DISKS_H

#include "Tools.h"
#include "Structs.h"

bool createDisk(string path, int tam);
bool deleteDisk(string path);
bool createPart(string path, Partition &p);
bool deletePart(const string path, const string name);

#endif //PROYECTO1_DISKS_H
