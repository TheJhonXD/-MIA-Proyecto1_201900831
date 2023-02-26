#ifndef PROYECTO1_DISKS_H
#define PROYECTO1_DISKS_H

#include "Tools.h"
#include "Structs.h"

//vector<MountedDisk> mds;

bool createDisk(string path, int tam);
bool deleteDisk(string path);
bool hasExtPart(const string path);
Partition getExtPart(const string &path);
vector<SpaceSize> BlockSize(const string path, MBR m);
vector<SpaceSize> ExtBlockSize(const string path);
bool createPart(string path, Partition &p);
bool deletePart(const string path, const string name);
bool addVolToPart(const string path, const string name, int tam);
bool reducePartSize(const string path, const string name, int tam);
bool mountDisk(const string path, const string name);
bool idExists(const string id);
bool unmountDisk(const string id);
vector<MountedDisk> getDisksMounted();
MountedDisk getDiskMtd(const string id);

#endif //PROYECTO1_DISKS_H
