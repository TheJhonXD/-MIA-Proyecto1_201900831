#ifndef PROYECTO1_STRUCTS_H
#define PROYECTO1_STRUCTS_H

#include <iostream>
#include <string>
#include <math.h>

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

struct SuperBlock{
    int s_filesystem_type;
    int s_inodes_count;
    int s_blocks_count;
    int s_free_blocks_count;
    int s_free_inodes_count;
    Time s_mtime;
    Time s_umtime;
    int s_mnt_count;
    int s_magic;
    int s_inode_s;
    int s_block_s;
    int s_firts_ino;
    int s_first_blo;
    int s_bm_inode_start;
    int s_bm_block_start;
    int s_inode_start;
    int s_block_start;
};

struct Inodo{
    int i_uid;
    int I_gid;
    int i_s;
    Time i_atime;
    Time i_ctime;
    Time i_mtime;
    int i_block;
    char i_type;
    int i_perm;
};

struct content{
    char b_name[12];
    int b_inodo;
};

struct FolderBlock{
    content b_content[4];
};

struct FileBlock{
    char b_content[64];
};

struct PointerBlock{
    int b_pointers[16];
};

struct Journaling{
    char operation[10];
    char type[10];
    char name[25];
    char content;
    Time date;
};

//FUNCIONES
Time getTime();
Partition RPV();
EBR REBRV();
SuperBlock RSBV();
void addMBR(const string path, MBR m);
MBR getMBR(const string path);
void readMBR(const string path);
void addEBR(const string path, int start, EBR e);
EBR getEBR(const string path, int start);
EBR getEBRByName(const string path, Partition ep, const string &name);
void addSuperBlock(const string &path, int start, SuperBlock sb);
SuperBlock getSuperBlock(const string path, int start);
void addInodo(const string &path, int start, Inodo i);
Inodo getInodo(const string path, int start);
void addFolderBlock(const string &path, int start, FolderBlock fb);
FolderBlock getFolderBlock(const string path, int start);
void addFileBlock(const string &path, int start, FileBlock fb);
FileBlock getFileBlock(const string path, int start);
void addPointerBlock(const string &path, int start, PointerBlock pb);
PointerBlock getPointerBlock(const string path, int start);

#endif //PROYECTO1_STRUCTS_H
