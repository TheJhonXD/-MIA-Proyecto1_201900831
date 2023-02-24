#ifndef PROYECTO1_STRUCTS_H
#define PROYECTO1_STRUCTS_H

#include <iostream>
#include <string>

using namespace std;

struct SpaceSize{
    int part_start;
    int part_s;
    char in_use;
    char type;
};

struct MountedDisk{
    string path;
    string name;
    string id;
};

struct Time{
    int tm_sec;   // seconds of minutes from 0 to 61
    int tm_min;   // minutes of hour from 0 to 59
    int tm_hour;  // hours of day from 0 to 24
    int tm_mday;  // day of month from 1 to 31
    int tm_mon;   // month of year from 0 to 11
    int tm_year;  // year since 1900
};

struct Partition{
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_s;
    char part_name[16];
};

struct MBR{
    int mbr_tamano;
    Time mbr_fecha_creacion;
    int mbr_dsk_signature;
    char dsk_fit;
    Partition mbr_partition_1;
    Partition mbr_partition_2;
    Partition mbr_partition_3;
    Partition mbr_partition_4;
};

struct EBR{
    char part_status;
    char part_fit;
    int part_start;
    int part_s;
    int part_next;
    char part_name[16];
};

//FUNCIONES
Time getTime();
Partition RPV();
EBR REBRV();
void addMBR(const string path, MBR m);
MBR getMBR(const string path);
void readMBR(const string path);
void addEBR(const string path, int start, EBR e);
EBR getEBR(const string path, int start);
EBR getEBRByName(const string path, Partition ep, const string &name);

#endif //PROYECTO1_STRUCTS_H
