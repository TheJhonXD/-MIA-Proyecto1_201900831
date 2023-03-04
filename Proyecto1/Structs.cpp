#include "Structs.h"

//Devuelve la fecha y hora actual, cuando se llame la funcion
Time getTime() {
    Time tim;
    time_t now = time(0);
    tm *t = localtime(&now);
    tim.tm_hour = t->tm_hour;
    tim.tm_min = t->tm_min;
    tim.tm_sec = t->tm_sec;
    tim.tm_mon = t->tm_mon;
    tim.tm_year = t->tm_year + 1900;
    tim.tm_mday = t->tm_mday;
    return tim;
}

//Reset Partition Variable
//Limpia la variable Particion o inicializa en 0
Partition RPV() {
    return {-1, -1, -1, -1, -1, "-1"};
}

//Reset EBR Variable
//Limpia la variable EBR o inicializa en 0
EBR REBRV() {
    return {-1, -1, -1, -1, -1, "-1"};
}

//Reset SuperBlock Variable
//Limpia la variable SuperBlock o inicializa en 0
SuperBlock RSBV() {
    Time tim = {-1, -1, -1, -1, -1, -1};
    return {-1, -1, -1, -1, -1, tim, tim, -1, 0xEF53, -1, -1, -1, -1, -1, -1, -1, -1};
}

//Añade el MBR a un disco especificado
//Recibe la ruta del disco y el MBR
void addMBR(const string path, MBR m) {
    try {
        FILE *myfile;
        myfile = fopen(path.c_str(), "rb+");
        fseek(myfile, 0, SEEK_SET);
        fwrite(&m, sizeof(MBR), 1, myfile);
        fclose(myfile);
    } catch(const exception &e) {
        cerr << e.what() <<endl;
    }
}

//Retorna el MBR del disco especificado
//Recibe la ruta dle disco
MBR getMBR(const string path) {
    MBR m;
    try {
        FILE *myfile;
        myfile = fopen(path.c_str(), "rb");
        fseek(myfile, 0, SEEK_SET);
        fread(&m, sizeof(MBR), 1, myfile);
        fclose(myfile);
    } catch(const exception& e) {
        cerr << e.what() << '\n';
    }
    return m;
}

//Lee el MBR del disco especificado
void readMBR(const string path) {
    try {
        FILE *myfile;
        myfile = fopen(path.c_str(), "rb");
        fseek(myfile, 0, SEEK_SET);
        MBR m;
        fread(&m, sizeof(MBR), 1, myfile);
        cout<< "Size: " << m.mbr_tamano <<endl;
        cout<< "Fecha: " << m.mbr_fecha_creacion.tm_mday << "/" << m.mbr_fecha_creacion.tm_mon << "/" << m.mbr_fecha_creacion.tm_year <<endl;
        cout<< "Hora: " << m.mbr_fecha_creacion.tm_hour << " : " << m.mbr_fecha_creacion.tm_min << " : " << m.mbr_fecha_creacion.tm_sec <<endl;
        cout<< "ID: " << m.mbr_dsk_signature <<endl;
        cout<< "Type: " << m.dsk_fit <<endl;
        cout<< "Part 1: " << m.mbr_partition_1.part_name <<endl;
        cout<< "Part 2: " << m.mbr_partition_2.part_name <<endl;
        cout<< "Part 3: " << m.mbr_partition_3.part_name <<endl;
        cout<< "Part 4: " << m.mbr_partition_4.part_name <<endl;
        fclose(myfile);
    } catch(const exception &e) {
        cerr << e.what() <<endl;
    }
}

//Añade el EBR a una particion extendida
//Recibe la ruta del disco y el EBR
void addEBR(const string path, int start, EBR e){
    try {
        FILE *myfile;
        myfile = fopen(path.c_str(), "rb+");
        fseek(myfile, start, SEEK_SET);
        fwrite(&e, sizeof(EBR), 1, myfile);
        fclose(myfile);
    } catch(const exception &e) {
        cerr << e.what() <<endl;
    }
}

//Retorna el EBR de la particion extendida en la posicion indicada
//Recibe la ruta dle disco y el inicio del EBR
EBR getEBR(const string path, int start){
    EBR e;
    try {
        FILE *myfile;
        myfile = fopen(path.c_str(), "rb");
        fseek(myfile, start, SEEK_SET);
        fread(&e, sizeof(EBR), 1, myfile);
        fclose(myfile);
    } catch(const exception &e) {
        cerr << e.what() <<endl;
    }
    return e;
}

//Retorna el EBR de la particion, la busca por nombre
//Reciba la ruta del disco, la particion extendida y el nombre
EBR getEBRByName(const string path, Partition ep, const string &name){
    EBR start = getEBR(path, ep.part_start);
    if (start.part_next != -1){
        EBR actual = getEBR(path, start.part_next);
        while (actual.part_next != -1){
            if (actual.part_name == name){
                return actual;
            }
            actual = getEBR(path, actual.part_next);
        }
        if (actual.part_name == name) return actual;
    }
    return REBRV(); //!aqui
}

//Añade el Super bloque a una partición especifica
//Recibe la ruta del disco y el super bloque
void addSuperBlock(const string &path, int start, SuperBlock sb){
    try {
        FILE *myfile;
        myfile = fopen(path.c_str(), "rb+");
        fseek(myfile, start, SEEK_SET);
        fwrite(&sb, sizeof(SuperBlock), 1, myfile);
        fclose(myfile);
    } catch(const exception &e) {
        cerr << e.what() <<endl;
    }
}

//Retorna el Super Bloque de la particion indicada
//Recibe la ruta dle disco y el inicio del SuperBloque
SuperBlock getSuperBlock(const string path, int start){
    SuperBlock sb;
    try {
        FILE *myfile;
        myfile = fopen(path.c_str(), "rb");
        fseek(myfile, start, SEEK_SET);
        fread(&sb, sizeof(SuperBlock), 1, myfile);
        fclose(myfile);
    } catch(const exception &e) {
        cerr << e.what() <<endl;
    }
    return sb;
}

//Añade el Inodo a una partición especifica
//Recibe la ruta del disco, la posicion donde se empezara a escribir, y el inodo
void addInodo(const string &path, int start, Inodo i){
    try {
        FILE *myfile;
        myfile = fopen(path.c_str(), "rb+");
        fseek(myfile, start, SEEK_SET);
        fwrite(&i, sizeof(Inodo), 1, myfile);
        fclose(myfile);
    } catch(const exception &e) {
        cerr << e.what() <<endl;
    }
}

//Retorna el Inodo de la particion indicada
//Recibe la ruta del disco y el inicio del inodo
Inodo getInodo(const string path, int start){
    Inodo i;
    try {
        FILE *myfile;
        myfile = fopen(path.c_str(), "rb");
        fseek(myfile, start, SEEK_SET);
        fread(&i, sizeof(Inodo), 1, myfile);
        fclose(myfile);
    } catch(const exception &e) {
        cerr << e.what() <<endl;
    }
    return i;
}

//Añade el bloque de carpeta a una partición especifica
//Recibe la ruta del disco, la posicion donde se empezara a escribir, y el bloque de carpeta
void addFolderBlock(const string &path, int start, FolderBlock fb){
    try {
        FILE *myfile;
        myfile = fopen(path.c_str(), "rb+");
        fseek(myfile, start, SEEK_SET);
        fwrite(&fb, sizeof(Inodo), 1, myfile);
        fclose(myfile);
    } catch(const exception &e) {
        cerr << e.what() <<endl;
    }
}

//Retorna el bloque de carpeta de la particion indicada
//Recibe la ruta del disco y el inicio del bloque de carpeta
FolderBlock getFolderBlock(const string path, int start){
    FolderBlock fb;
    try {
        FILE *myfile;
        myfile = fopen(path.c_str(), "rb");
        fseek(myfile, start, SEEK_SET);
        fread(&fb, sizeof(Inodo), 1, myfile);
        fclose(myfile);
    } catch(const exception &e) {
        cerr << e.what() <<endl;
    }
    return fb;
}

//Añade el bloque de archivo a una partición especifica
//Recibe la ruta del disco, la posicion donde se empezara a escribir, y el bloque de archivo
void addFileBlock(const string &path, int start, FileBlock fb){
    try {
        FILE *myfile;
        myfile = fopen(path.c_str(), "rb+");
        fseek(myfile, start, SEEK_SET);
        fwrite(&fb, sizeof(Inodo), 1, myfile);
        fclose(myfile);
    } catch(const exception &e) {
        cerr << e.what() <<endl;
    }
}

//Retorna el bloque de archivo de la particion indicada
//Recibe la ruta del disco y el inicio del bloque de archivo
FileBlock getFileBlock(const string path, int start){
    FileBlock fb;
    try {
        FILE *myfile;
        myfile = fopen(path.c_str(), "rb");
        fseek(myfile, start, SEEK_SET);
        fread(&fb, sizeof(Inodo), 1, myfile);
        fclose(myfile);
    } catch(const exception &e) {
        cerr << e.what() <<endl;
    }
    return fb;
}

//Añade el bloque de apuntadores a una partición especifica
//Recibe la ruta del disco, la posicion donde se empezara a escribir, y el bloque de apuntadores
void addPointerBlock(const string &path, int start, PointerBlock pb){
    try {
        FILE *myfile;
        myfile = fopen(path.c_str(), "rb+");
        fseek(myfile, start, SEEK_SET);
        fwrite(&pb, sizeof(Inodo), 1, myfile);
        fclose(myfile);
    } catch(const exception &e) {
        cerr << e.what() <<endl;
    }
}

//Retorna el bloque de apuntadores de la particion indicada
//Recibe la ruta del disco y el inicio del bloque de apuntadores
PointerBlock getPointerBlock(const string path, int start){
    PointerBlock pb;
    try {
        FILE *myfile;
        myfile = fopen(path.c_str(), "rb");
        fseek(myfile, start, SEEK_SET);
        fread(&pb, sizeof(Inodo), 1, myfile);
        fclose(myfile);
    } catch(const exception &e) {
        cerr << e.what() <<endl;
    }
    return pb;
}

int getMaxNumStructExt2(const int &tam){
    return floor((tam - sizeof(SuperBlock))/(1 + 3 + sizeof(Inodo) + (3*sizeof(FolderBlock)))); 
}

int getMaxNumStructExt3(const int &tam){
    return floor((tam - sizeof(SuperBlock))/(1 + sizeof(Journaling) + 3 + sizeof(Inodo) + (3*sizeof(FolderBlock)))); 
}