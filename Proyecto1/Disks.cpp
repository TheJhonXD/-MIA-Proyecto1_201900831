#include "Disks.h"

//Crea un archivo que simula un disco en la ruta y tamaño especificados
bool createDisk(string path, int tam){
    //Compruebo si no existe el archivo
    if (!fs::exists(path)){
        //creo los directorios si no existen
        if (createDir(path)){
            FILE *_file;
            _file = fopen(path.c_str(), "wb");
            //Compruebo si se creo el archivo
            if (_file == NULL){
                cout<< "ERROR: El disco no pudo ser creado" <<endl; return false;
            }
            /* Lleno el archivo con caracteres nulos para simular el tamaño */
            //*cambié esto
            char c[1024];
            memset(c, 0, 1024); //Limpio la variable
            for (int i=0; i<tam/1024; i++){
                fwrite(&c, sizeof(c), 1, _file);//Escribo en el archivo el contenido nulo del buffer
            }
            fclose(_file); //Cierro el archivo
            cout<< "Disco creado exitosamente" <<endl;
            return true;
        }
    }else{
        cout<< "El disco \"" << getFileName(path) << "\" ya existe" << endl;
    }
    return false;
}

//Elimina un disco en la ruta especificada
bool deleteDisk(string path){
    //Compruebo si existe el archivo
    if (fs::exists(path)){
        //Remuevo el archivo y compruebo si fué eliminado
        if (fs::remove(path)){
            cout<< "Disco eliminado satifactoriamente" <<endl; return true;
        }else{
            cout<< "ERROR: El disco no pudo se eliminado" <<endl;
        }
    }else{
        cout<< "El disco \"" << getFileName(path) << "\" no existe" << endl;
    }
    return false;
}

//Comprueba si una particion dada es primaria
bool isPrimPart(MBR m, const string &name) {
    if (m.mbr_partition_1.part_name == name && m.mbr_partition_1.part_type == 'p'){
        return true;
    }else if (m.mbr_partition_2.part_name == name && m.mbr_partition_2.part_type == 'p'){
        return true;
    }else if (m.mbr_partition_3.part_name == name && m.mbr_partition_3.part_type == 'p'){
        return true;
    }else if (m.mbr_partition_4.part_name == name && m.mbr_partition_4.part_type == 'p'){
        return true;
    }
    return false;
}

//Comprueba si una particion dada es extendida
bool isExtPart(MBR m, const string &name) {
    if (m.mbr_partition_1.part_name == name && m.mbr_partition_1.part_type == 'e'){
        return true;
    }else if (m.mbr_partition_2.part_name == name && m.mbr_partition_2.part_type == 'e'){
        return true;
    }else if (m.mbr_partition_3.part_name == name && m.mbr_partition_3.part_type == 'e'){
        return true;
    }else if (m.mbr_partition_4.part_name == name && m.mbr_partition_4.part_type == 'e'){
        return true;
    }
    return false;
}

//Comprueba si existe una particion
//Recibe la ruta del disco y el nombre de la particion
bool partExists(const string path, const string name){
    MBR m = getMBR(path);
    //Busco si existe una particion primaria con el nombre dado
    if (isPrimPart(m, name)){
        return true;
    }else if (isExtPart(m, name)){ // Busco si existe una particion extendida con el nombre dado
        return true;
    }
    return false;
}

bool isDiskEmpty(const string path){
    MBR m = getMBR(path);
    if (m.mbr_partition_1.part_s > 0 || m.mbr_partition_2.part_s > 0 || m.mbr_partition_3.part_s > 0 || m.mbr_partition_4.part_s > 0){
        return false;
    }
    return true;
}

vector<SpaceSize> BlockSize(const string path, MBR m){
    vector<SpaceSize> v;
    SpaceSize s;
    int inicio = sizeof(MBR) + 1;
    int prevSize = inicio - 1;
    int prevStart = 0;
    int endSpace = prevSize + prevStart;

    if (!isDiskEmpty(path)){ //Compruebo si no está vacío el disco
        if (m.mbr_partition_1.part_s > 0){
            //Compruebo si hay espacio entre el mbr y la particion 1
            if ((inicio - m.mbr_partition_1.part_start) > 0){
                v.push_back({inicio, inicio + (m.mbr_partition_1.part_start - inicio), 'n'});
                v.push_back({m.mbr_partition_1.part_start, m.mbr_partition_1.part_s, 's'});
                prevSize = m.mbr_partition_1.part_s;
                prevStart = m.mbr_partition_1.part_start;
                endSpace = prevSize + prevStart;
            }else if ((inicio - m.mbr_partition_1.part_start) == 0){
                v.push_back({m.mbr_partition_1.part_start, m.mbr_partition_1.part_s, 's'});
                prevSize = m.mbr_partition_1.part_s;
                prevStart = m.mbr_partition_1.part_start;
                endSpace = prevSize + prevStart;
            }
        }
        if (m.mbr_partition_2.part_s > 0){
            //Compruebo si hay espacio entre el mbr y la particion 1
            if (((endSpace + 1) - m.mbr_partition_2.part_start) > 0){
                v.push_back({endSpace + 1, endSpace + (m.mbr_partition_2.part_start - endSpace - 1), 'n'});
                v.push_back({m.mbr_partition_2.part_start, m.mbr_partition_2.part_s, 's'});
                prevSize = m.mbr_partition_2.part_s;
                prevStart = m.mbr_partition_2.part_start;
                endSpace = prevSize + prevStart;
            }else if ((inicio - m.mbr_partition_2.part_start) == 0){
                v.push_back({m.mbr_partition_2.part_start, m.mbr_partition_2.part_s, 's'});
                prevSize = m.mbr_partition_2.part_s;
                prevStart = m.mbr_partition_2.part_start;
                endSpace = prevSize + prevStart;
            }
        }
        if (m.mbr_partition_3.part_s > 0){
            //Compruebo si hay espacio entre el mbr y la particion 1
            if (((endSpace + 1) - m.mbr_partition_3.part_start) > 0){
                v.push_back({endSpace + 1, endSpace + (m.mbr_partition_3.part_start - endSpace - 1), 'n'});
                v.push_back({m.mbr_partition_3.part_start, m.mbr_partition_3.part_s, 's'});
                prevSize = m.mbr_partition_3.part_s;
                prevStart = m.mbr_partition_3.part_start;
                endSpace = prevSize + prevStart;
            }else if ((inicio - m.mbr_partition_3.part_start) == 0){
                v.push_back({m.mbr_partition_3.part_start, m.mbr_partition_3.part_s, 's'});
                prevSize = m.mbr_partition_3.part_s;
                prevStart = m.mbr_partition_3.part_start;
                endSpace = prevSize + prevStart;
            }
        }

        if (m.mbr_partition_4.part_s > 0){
            //Compruebo si hay espacio entre el mbr y la particion 1
            if (((endSpace + 1) - m.mbr_partition_4.part_start) > 0){
                v.push_back({endSpace + 1, endSpace + (m.mbr_partition_4.part_start - endSpace - 1), 'n'});
                v.push_back({m.mbr_partition_4.part_start, m.mbr_partition_4.part_s, 's'});
                prevSize = m.mbr_partition_4.part_s;
                prevStart = m.mbr_partition_4.part_start;
                endSpace = prevSize + prevStart;
            }else if ((inicio - m.mbr_partition_4.part_start) == 0){
                v.push_back({m.mbr_partition_4.part_start, m.mbr_partition_4.part_s, 's'});
                prevSize = m.mbr_partition_4.part_s;
                prevStart = m.mbr_partition_4.part_start;
                endSpace = prevSize + prevStart;
            }
        }

        if ((endSpace + 1) < m.mbr_tamano){
            v.push_back({endSpace + 1, m.mbr_tamano - (endSpace + 1), 'n'});
        }
    }else{
        v.push_back({inicio, m.mbr_tamano - inicio, 'n'});
    }

    return v;
}



void firstFit(const string path){
    MBR m = getMBR(path);
}

//Crea una particion en el disco especificado
//Recibe la ruta del disco, nombre, tipo y tamaño de la particion
bool createPart(string path, string name, char typek, int tam){
    if (fs::exists(path)){ //Compruebo si existe el disco
        if (!partExists(path, name)){ //Compruebo si existe la particion
            cout<< "Si entre" <<endl;
            return true;
        }else{
            cout<< "ERROR: La particion ya existe" <<endl;
        }
    }else{
        cout<< "ERROR: El disco \"" << getFileName(path) << "\" no existe" <<endl;
    }
    return false;
}