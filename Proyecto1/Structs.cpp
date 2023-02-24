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
        myfile = fopen(path.c_str(), "rb");
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
    return {}; //!aqui
}