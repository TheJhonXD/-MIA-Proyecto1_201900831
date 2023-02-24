#include "Disks.h"

vector<MountedDisk> mds;

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

//Retorna la particion extendida del disco
Partition getExtPart(const string &path){
    MBR m = getMBR(path);
    if (m.mbr_partition_1.part_type == 'e'){
        return m.mbr_partition_1;
    }else if (m.mbr_partition_2.part_type == 'e'){
        return m.mbr_partition_2;
    }else if (m.mbr_partition_3.part_type == 'e'){
        return m.mbr_partition_3;
    }else if (m.mbr_partition_4.part_type == 'e'){
        return m.mbr_partition_4;
    }
    return {};
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

bool isLogPart(const string path, const string &name){
    Partition ep = getExtPart(path);
    EBR start = getEBR(path, ep.part_start);
    if (start.part_next != -1){
        EBR actual = getEBR(path, start.part_next);
        while (actual.part_next != -1){
            if (actual.part_name == name){
                return true;
            }
            actual = getEBR(path, actual.part_next);
        }
        if (actual.part_name == name) return true;
    }
    return false;
}

Partition getPartByName(const string path, const string name){
    MBR m = getMBR(path);
    if (m.mbr_partition_1.part_name == name){
        return m.mbr_partition_1;
    }else if (m.mbr_partition_2.part_name == name){
        return m.mbr_partition_2;
    }else if (m.mbr_partition_3.part_name == name){
        return m.mbr_partition_3;
    }else if (m.mbr_partition_4.part_name == name){
        return m.mbr_partition_4;
    }
    return {}; //!aqui
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
    }else if (isLogPart(path, name)){
        return true;
    }
    return false;
}

//Retorna el numero de particiones primarias
int numPrimPart(MBR m){
    int cont = 0;
    if (m.mbr_partition_1.part_type == 'p') cont++;
    if (m.mbr_partition_2.part_type == 'p') cont++;
    if (m.mbr_partition_3.part_type == 'p') cont++;
    if (m.mbr_partition_4.part_type == 'p') cont++;
    return cont;
}

//Retorna el numero de particiones extendidas
int numExtPart(MBR m){
    int cont = 0;
    if (m.mbr_partition_1.part_type == 'e') cont++;
    if (m.mbr_partition_2.part_type == 'e') cont++;
    if (m.mbr_partition_3.part_type == 'e') cont++;
    if (m.mbr_partition_4.part_type == 'e') cont++;
    return cont;
}

//Comprueba si el disco está vacío
//Recibe la ruta del disco como parametro
bool isDiskEmpty(const string path){
    MBR m = getMBR(path);
    //Compruebo si algun slot de particion no está vacío
    if (m.mbr_partition_1.part_s > 0 || m.mbr_partition_2.part_s > 0 || m.mbr_partition_3.part_s > 0 || m.mbr_partition_4.part_s > 0){
        return false;
    }
    return true;
}

void addPartition(MBR &m, Partition p){
    if (m.mbr_partition_1.part_s < 0){
        m.mbr_partition_1 = p;
    }else if (m.mbr_partition_2.part_s < 0){
        m.mbr_partition_2 = p;
    }else if (m.mbr_partition_3.part_s < 0){
        m.mbr_partition_3 = p;
    }else if (m.mbr_partition_4.part_s < 0){
        m.mbr_partition_4 = p;
    }
}

void addLogPartition(const string path, int prevSpace, EBR &nuevo){
    Partition ep = getExtPart(path);
    EBR start = getEBR(path, ep.part_start);
    EBR prev = REBRV();
    
    if (prevSpace != -1){ //Compruebo si hay un slot de espacio anterior al del nuevo
        prev = getEBR(path, prevSpace);
        if (prev.part_next != -1){ //Compruebo si existe una particion siguiente
            EBR nxt = getEBR(path, prev.part_next);
            prev.part_next = nuevo.part_start;
            nuevo.part_next = nxt.part_start;
            addEBR(path, prev.part_start, prev);
            addEBR(path, nuevo.part_start, nuevo);
        }else{ //Si no existe un siguiente quiere decir que está en el ultimo slot de espacio
            prev.part_next = nuevo.part_start;
            addEBR(path, prev.part_start, prev);
            addEBR(path, nuevo.part_start, nuevo);
        }
    }else{
        nuevo.part_next = getEBR(path, start.part_next).part_start;
        start.part_next = nuevo.part_start;
        addEBR(path, start.part_start, start);
        addEBR(path, nuevo.part_start, nuevo);
    }
}

void sortPartitions(MBR &m) {
    vector<int> sizes;
    vector<Partition> parts;
    vector<Partition> nuevo;

    sizes.push_back(m.mbr_partition_1.part_start);
    sizes.push_back(m.mbr_partition_2.part_start);
    sizes.push_back(m.mbr_partition_3.part_start);
    sizes.push_back(m.mbr_partition_4.part_start);

    parts.push_back(m.mbr_partition_1);
    parts.push_back(m.mbr_partition_2);
    parts.push_back(m.mbr_partition_3);
    parts.push_back(m.mbr_partition_4);

    std::sort(sizes.begin(), sizes.end());

    for (size_t i=0; i<sizes.size(); i++){
        for (size_t j=0; j<parts.size(); j++){
            if (parts[j].part_start == sizes[i]){
                nuevo.push_back(parts[i]);
            }
        }
    }

    m.mbr_partition_1 = nuevo[0];
    m.mbr_partition_2 = nuevo[1];
    m.mbr_partition_3 = nuevo[2];
    m.mbr_partition_4 = nuevo[3];
}

//Retorna un vector de structs que contienen el inicio y el final de cada espacio libre y ocupado en el disco
//Recibe la ruta del disco y el mbr del mismo
vector<SpaceSize> BlockSize(const string path, MBR m){
    //*Si ocurre un error corroborar el absoluto
    vector<SpaceSize> v;
    int inicio = sizeof(MBR) + 1;
    int prevSize = inicio - 1;
    int prevStart = 0;
    int endSpace = prevSize + prevStart;

    if (!isDiskEmpty(path)){ //Compruebo si no está vacío el disco

        if (m.mbr_partition_1.part_s > 0){ //Compruebo si el espacio para particion está vacío
            //Compruebo si hay espacio libre entre el mbr y la particion
            if (abs(inicio - m.mbr_partition_1.part_start) > 0){
                /* Agrego los datos del espacio a la lista de tamaño de espacio */
                // v.push_back({inicio, inicio + (m.mbr_partition_1.part_start - inicio), 'n', '0'});
                v.push_back({inicio, abs(m.mbr_partition_1.part_start - inicio), 'n', '0'});
                v.push_back({m.mbr_partition_1.part_start, m.mbr_partition_1.part_s, 's', m.mbr_partition_1.part_type});
                prevSize = m.mbr_partition_1.part_s; //Guardo el tamaño de la partición actual
                prevStart = m.mbr_partition_1.part_start; //Guardo la posicion de inicio de la partion actual
                endSpace = prevSize + prevStart; //Guardo el calculo de posicion final de la particion actual
            }else if ((inicio - m.mbr_partition_1.part_start) == 0){ //Compruebo si no hay espacio entre particiones
                //Agrego los datos del espacio a la lista de tamaño de espacio
                v.push_back({m.mbr_partition_1.part_start, m.mbr_partition_1.part_s, 's', m.mbr_partition_1.part_type});
                prevSize = m.mbr_partition_1.part_s; //Guardo el tamaño de la partición actual
                prevStart = m.mbr_partition_1.part_start; //Guardo la posicion de inicio de la partion actual
                endSpace = prevSize + prevStart; //Guardo el calculo de posicion final de la particion actual
            }
        }

        /* MISMO PROCEDIMIENTO PARA LAS DEMÁS PARTICIONES */
        if (m.mbr_partition_2.part_s > 0){
            if (abs((endSpace + 1) - m.mbr_partition_2.part_start) > 0){
                //*se verificó calculo de espacio libre
                // v.push_back({endSpace + 1, endSpace + (m.mbr_partition_2.part_start - endSpace - 1), 'n', '0'});
                v.push_back({endSpace + 1, abs(m.mbr_partition_2.part_start - endSpace - 1), 'n', '0'});
                v.push_back({m.mbr_partition_2.part_start, m.mbr_partition_2.part_s, 's', m.mbr_partition_2.part_type});
                prevSize = m.mbr_partition_2.part_s;
                prevStart = m.mbr_partition_2.part_start;
                endSpace = prevSize + prevStart;
            }else if ((inicio - m.mbr_partition_2.part_start) == 0){
                v.push_back({m.mbr_partition_2.part_start, m.mbr_partition_2.part_s, 's', m.mbr_partition_2.part_type});
                prevSize = m.mbr_partition_2.part_s;
                prevStart = m.mbr_partition_2.part_start;
                endSpace = prevSize + prevStart;
            }
        }
        if (m.mbr_partition_3.part_s > 0){
            if (abs((endSpace + 1) - m.mbr_partition_3.part_start) > 0){
                // v.push_back({endSpace + 1, endSpace + (m.mbr_partition_3.part_start - endSpace - 1), 'n', '0'});
                v.push_back({endSpace + 1, abs(m.mbr_partition_3.part_start - endSpace - 1), 'n', '0'});
                v.push_back({m.mbr_partition_3.part_start, m.mbr_partition_3.part_s, 's', m.mbr_partition_3.part_type});
                prevSize = m.mbr_partition_3.part_s;
                prevStart = m.mbr_partition_3.part_start;
                endSpace = prevSize + prevStart;
            }else if ((inicio - m.mbr_partition_3.part_start) == 0){
                v.push_back({m.mbr_partition_3.part_start, m.mbr_partition_3.part_s, 's', m.mbr_partition_3.part_type});
                prevSize = m.mbr_partition_3.part_s;
                prevStart = m.mbr_partition_3.part_start;
                endSpace = prevSize + prevStart;
            }
        }

        if (m.mbr_partition_4.part_s > 0){
            if (abs((endSpace + 1) - m.mbr_partition_4.part_start) > 0){
                // v.push_back({endSpace + 1, endSpace + (m.mbr_partition_4.part_start - endSpace - 1), 'n', '0'});
                v.push_back({endSpace + 1, abs(m.mbr_partition_4.part_start - endSpace - 1), 'n', '0'});
                v.push_back({m.mbr_partition_4.part_start, m.mbr_partition_4.part_s, 's', m.mbr_partition_4.part_type});
                prevSize = m.mbr_partition_4.part_s;
                prevStart = m.mbr_partition_4.part_start;
                endSpace = prevSize + prevStart;
            }else if ((inicio - m.mbr_partition_4.part_start) == 0){
                v.push_back({m.mbr_partition_4.part_start, m.mbr_partition_4.part_s, 's', m.mbr_partition_4.part_type});
                prevSize = m.mbr_partition_4.part_s;
                prevStart = m.mbr_partition_4.part_start;
                endSpace = prevSize + prevStart;
            }
        }

        //Compruebo si hay espacio libre entre la ultima particion y el final del disco
        if ((endSpace + 1) < m.mbr_tamano){
            //Agrego los datos del espacio a la lista de tamaño de espacio
            v.push_back({endSpace + 1, abs(m.mbr_tamano - (endSpace + 1)), 'n', '0'});
        }
    }else{
        v.push_back({inicio, m.mbr_tamano - inicio, 'n', '0'});
    }

    return v;
}

vector<SpaceSize> ExtBlockSize(const string path){
    //todo: trabajar como particion estuviera en el inicio si no jala
    vector<SpaceSize> v;
    int inicio = sizeof(EBR) + 1;
    Partition ep = getExtPart(path); //Partición extendida (extended partition)
    EBR e = getEBR(path, ep.part_start); //EBR inicial
    int endSpace = 0;

    if (e.part_next != -1){ //Compruebo si está vacío el disco
        EBR actual = getEBR(path, e.part_next);
        EBR siguiente;

        //Primera parte
        if (abs(inicio - actual.part_start) > 0){
            v.push_back({inicio, actual.part_start - inicio, 'n', '0'});
        }

        //Segunda parte
        endSpace = actual.part_start + actual.part_s;
        while (actual.part_next != -1){
            v.push_back({actual.part_start, actual.part_s, 's', 'l'});
            siguiente = getEBR(path, actual.part_next);
            if (abs((endSpace + 1) - siguiente.part_start) > 0){
                v.push_back({endSpace + 1, abs(siguiente.part_start - (endSpace + 1)), 'n', '0'});
            }
            actual = siguiente;
        }
        
        //Tercera parte
        if ((endSpace + 1) < ep.part_s){
            v.push_back({endSpace + 1, ep.part_s - (endSpace + 1), 'n', '0'});
        }
        
    }else{
        v.push_back({inicio, ep.part_s - inicio, 'n', 'l'});
    }

    return v;
}

//Asigna la particion en la memoria a bloques según el algoritmo de primer ajuste
//Recibe la ruta del disco y la particion
bool firstFit(const string path, Partition &p){
    MBR m = getMBR(path); //Obtengo el mbr del disco
    vector<SpaceSize> ss = BlockSize(path, m); //Obtengo el bloque de tamaños
    for (SpaceSize s : ss){
        //Compruebo si el tamaño es mayor que la particion y si no está en uso
        if ((p.part_s <= s.part_s) && (s.in_use != 's')){
            p.part_start = s.part_start;
            addPartition(m, p);
            sortPartitions(m);
            addMBR(path, m);
            return true;
        }
    }
    return false;
}

//Asigna la particion en la memoria a bloques según el algoritmo de mejor ajuste
//Recibe la ruta del disco y la particion
bool bestFit(const string path, Partition &p){
    MBR m = getMBR(path); //Obtengo el mbr del disco
    vector<SpaceSize> ss = BlockSize(path, m); //Obtengo el bloque de tamaños
    int bestFitIdx = -1;
    for (int i=0; i<sizeof(ss)/sizeof(SpaceSize); i++){
        if ((ss[i].part_s >= p.part_s) && (ss[i].in_use != 's')){
            if (bestFitIdx == -1){
                bestFitIdx = i;
            }else if (ss[bestFitIdx].part_s > ss[i].part_s){
                bestFitIdx = i;
            }
        }
    }
    if (bestFitIdx != -1){
        p.part_start = ss[bestFitIdx].part_start;
        addPartition(m, p);
        sortPartitions(m);
        addMBR(path, m);
        return true;
    }
    return false;
}

//Asigna la particion en la memoria a bloques según el algoritmo de peor ajuste
//Recibe la ruta del disco y la particion
bool worstFit(const string path, Partition &p){
    MBR m = getMBR(path); //Obtengo el mbr del disco
    vector<SpaceSize> ss = BlockSize(path, m); //Obtengo el bloque de tamaños
    int worstFitIdx = -1;
    for (int i=0; i<sizeof(ss)/sizeof(SpaceSize); i++){
        if ((ss[i].part_s >= p.part_s) && (ss[i].in_use != 's')){
            if (worstFitIdx == -1){
                worstFitIdx = i;
            }else if (ss[worstFitIdx].part_s < ss[i].part_s){
                worstFitIdx = i;
            }
        }
    }
    if (worstFitIdx != -1){
        p.part_start = ss[worstFitIdx].part_start;
        addPartition(m, p);
        sortPartitions(m);
        addMBR(path, m);
        return true;
    }
    return false;
}

//Asigna la particion en la memoria a bloques según el algoritmo de primer ajuste para la particion extendida
//Recibe la ruta del disco y la particion
bool extFirstFit(const string path, EBR &e){
    int prevSpace = -1;
    vector<SpaceSize> ss = ExtBlockSize(path); //Obtengo el bloque de tamaños
    for (SpaceSize s : ss){
        //Compruebo si el tamaño es mayor que la particion y si no está en uso
        if ((e.part_s <= s.part_s) && (s.in_use != 's')){
            e.part_start = s.part_start;
            addLogPartition(path, prevSpace, e);
            return true;
        }
        prevSpace = s.part_start; //Guardo el espacio anterior
    }
    return false;
}

//Asigna la particion en la memoria a bloques según el algoritmo de mejor ajuste para la particion extendida
//Recibe la ruta del disco y la particion
bool extBestFit(const string path, EBR &e){
    MBR m = getMBR(path); //Obtengo el mbr del disco
    vector<SpaceSize> ss = ExtBlockSize(path); //Obtengo el bloque de tamaños
    int bestFitIdx = -1;
    int prevSpace = -1;
    for (int i=0; i<sizeof(ss)/sizeof(SpaceSize); i++){
        if ((ss[i].part_s >= e.part_s) && (ss[i].in_use != 's')){
            if (bestFitIdx == -1){
                bestFitIdx = i;
            }else if (ss[bestFitIdx].part_s > ss[i].part_s){
                bestFitIdx = i;
            }
        }
    }
    if (bestFitIdx != -1){
        e.part_start = ss[bestFitIdx].part_start;
        if (bestFitIdx > 0) prevSpace = ss[bestFitIdx - 1].part_start;
        addLogPartition(path, prevSpace, e);
        return true;
    }
    return false;
}

//Asigna la particion en la memoria a bloques según el algoritmo de peor ajuste para la particion extendida
//Recibe la ruta del disco y la particion
bool extWorstFit(const string path, EBR &e){
    // MBR m = getMBR(path); //Obtengo el mbr del disco
    vector<SpaceSize> ss = ExtBlockSize(path); //Obtengo el bloque de tamaños
    int worstFitIdx = -1;
    int prevSpace = -1;
    for (int i=0; i<sizeof(ss)/sizeof(SpaceSize); i++){
        if ((ss[i].part_s >= e.part_s) && (ss[i].in_use != 's')){
            if (worstFitIdx == -1){
                worstFitIdx = i;
            }else if (ss[worstFitIdx].part_s < ss[i].part_s){
                worstFitIdx = i;
            }
        }
    }
    if (worstFitIdx != -1){
        e.part_start = ss[worstFitIdx].part_start;
        if (worstFitIdx > 0) prevSpace = ss[worstFitIdx - 1].part_start;
        addLogPartition(path, prevSpace, e);
        return true;
    }
    return false;
}

bool chooseFit(const string &path, char fit,  Partition &p){
    if (fit == 'f'){
        return firstFit(path, p);
    }else if (fit == 'b'){
        return bestFit(path, p);
    }else if (fit == 'w'){
        return worstFit(path, p);
    }
    return false;
}

bool chooseExtFit(const string &path, char fit,  EBR &e){
    if (fit == 'f'){
        return extFirstFit(path, e);
    }else if (fit == 'b'){
        return extBestFit(path, e);
    }else if (fit == 'w'){
        return extWorstFit(path, e);
    }
    return false;
}

//Crea una particion en el disco especificado
//Recibe la ruta del disco y la variable particion
bool createPart(string path, Partition &p){
    if (fs::exists(path)){ //Compruebo si existe el disco
        if (!partExists(path, p.part_name)){ //Compruebo si existe la particion
            MBR m = getMBR(path);
            if (p.part_type == 'p'){
                if (numPrimPart(m) < 4){
                    if (chooseFit(path, m.dsk_fit, p)){
                        cout<< "Particion primaria creada correctamente" <<endl;
                        return true;
                    }else{
                        cout<< "ERROR: No se pudo asignar la particion" <<endl;
                    }
                }else{
                    cout<< "ERROR: Se exedió el numero de particiones permitidas" <<endl;
                }
            }else if (p.part_type == 'e'){
                if (numExtPart(m) == 0){
                    if (numPrimPart(m) < 4){
                        if (chooseFit(path, m.dsk_fit, p)){
                            cout<< "Particion extendida creada correctamente" <<endl;
                            return true;
                        }else{
                            cout<< "ERROR: No se pudo asignar la particion" <<endl;
                        }
                    }else{
                        cout<< "ERROR: Se exedió el numero de particiones permitidas" <<endl;
                    }
                }else{
                    cout<< "ERROR: Se exedió el numero de particiones extendidas permitidas" <<endl;
                }
            }else if (p.part_type == 'l'){
                if (numExtPart(m) > 0){
                    EBR e = {-1, p.part_fit, p.part_start, p.part_s, -1, *p.part_name};
                    if (chooseExtFit(path, p.part_fit, e)){
                        cout<< "Particion logica creada correctamente" <<endl;
                    }else{
                        cout<< "ERROR: No se pudo asignar la particion logica" <<endl;
                    }
                }else{
                    cout<< "ERROR: No se puede crear una particion logica si no existe una extendida" <<endl;
                }
            }

        }else{
            cout<< "ERROR: La particion ya existe" <<endl;
        }
    }else{
        cout<< "ERROR: El disco \"" << getFileName(path) << "\" no existe" <<endl;
    }
    return false;
}

bool fillSpaceDeleted(const string path, int start, int end){
    try {
        FILE *myfile;
        myfile = fopen(path.c_str(), "rb+");
        fseek(myfile, start, SEEK_SET);
        char c = '\0';
        //*Si falla comprobar (end - start)
        for(int i = 0; i < end; i++){
            fwrite(&c, sizeof(c), 1, myfile);
        }
        return true;
    } catch(const exception &e) {
        cerr << e.what() <<endl;
    }
    return false;
}

bool removePrimPart(const string path, const string name, MBR &m){
    if (m.mbr_partition_1.part_name == name){
        fillSpaceDeleted(path, m.mbr_partition_1.part_start, m.mbr_partition_1.part_s);
        m.mbr_partition_1 = RPV();
        return true;
    }else if (m.mbr_partition_2.part_name == name){
        fillSpaceDeleted(path, m.mbr_partition_2.part_start, m.mbr_partition_2.part_s);
        m.mbr_partition_2 = RPV();
        return true;
    }else if (m.mbr_partition_3.part_name == name){
        fillSpaceDeleted(path, m.mbr_partition_3.part_start, m.mbr_partition_3.part_s);
        m.mbr_partition_3 = RPV();
        return true;
    }else if (m.mbr_partition_4.part_name == name){
        fillSpaceDeleted(path, m.mbr_partition_4.part_start, m.mbr_partition_4.part_s);
        m.mbr_partition_4 = RPV();
        return true;
    }
    return false;
}

bool removeExtPart(const string path, MBR &m){
    Partition ep = getExtPart(path);
    fillSpaceDeleted(path, ep.part_start, ep.part_s);
    if (m.mbr_partition_1.part_type == 'e'){
        m.mbr_partition_1 = RPV();
        return true;
    }else if (m.mbr_partition_2.part_type == 'e'){
        m.mbr_partition_2 = RPV();
        return true;
    }else if (m.mbr_partition_3.part_type == 'e'){
        m.mbr_partition_3 = RPV();
        return true;
    }else if (m.mbr_partition_4.part_type == 'e'){
        m.mbr_partition_4 = RPV();
        return true;
    }
    return false;
}

bool removeLogPart(const string path, const string &name){
    //!Recordar editar el EBR en el disco para el anterior y actual, tambien en addLogPart
    Partition ep = getExtPart(path);
    EBR start = getEBR(path, ep.part_start);
    EBR prev = start;
    if (start.part_next != -1){
        EBR actual = getEBR(path, start.part_next);
        while (actual.part_next != -1){
            if (actual.part_name == name){
                if (actual.part_next != -1){
                    EBR nxt = getEBR(path, actual.part_next);
                    prev.part_next = nxt.part_start;
                    addEBR(path, prev.part_start, prev);
                }else{
                    prev.part_next = -1;
                    addEBR(path, prev.part_start, prev);
                }
                fillSpaceDeleted(path, actual.part_start, actual.part_s);
                return true;
            }
            prev = actual;
            actual = getEBR(path, actual.part_next);
        }
        if (actual.part_name == name){
            if (prev.part_next != -1){
                prev.part_start = -1;
                addEBR(path, prev.part_start, prev);
            }
            return true;
        }
    }
    return false;
}

//Elimina una particion en el disco especificado
//Recibe la ruta del disco y el nombre de la particion
bool deletePart(const string path, const string name){
    if (fs::exists(path)){
        if (partExists(path, name)){
            MBR m = getMBR(path);
            if (isPrimPart(m, name)){
                if (removePrimPart(path, name, m)){
                    addMBR(path, m);
                    cout<< "Particion primaria eliminada correctamente" <<endl;
                    return true;
                }
            }else if (isExtPart(m, name)){
                if (removeExtPart(path, m)){
                    addMBR(path, m);
                    cout<< "Particion extendida eliminada correctamente" <<endl;
                    return true;
                }
            }else if (isLogPart(path, name)){
                if (removeLogPart(path, name)){
                    cout<< "Particion logica eliminada correctamente" <<endl;
                    return true;
                }
            }
        }else{
            cout<< "ERROR: La particion no existe" <<endl;
        }
    }else{
        cout<< "ERROR: El disco \"" << getFileName(path) << "\" no existe" <<endl;
    }
    return false;
}

int availSpacePart(MBR m, const string &name){
    int availSpace = -1;

    if (m.mbr_partition_1.part_name == name){
        availSpace = m.mbr_partition_2.part_start - (m.mbr_partition_1.part_start + m.mbr_partition_1.part_s);
    }else if (m.mbr_partition_2.part_name == name){
        availSpace = m.mbr_partition_3.part_start - (m.mbr_partition_2.part_start + m.mbr_partition_2.part_s);
    }else if (m.mbr_partition_3.part_name == name){
        availSpace = m.mbr_partition_4.part_start - (m.mbr_partition_3.part_start + m.mbr_partition_3.part_s);
    }else if (m.mbr_partition_4.part_name == name){
        availSpace = m.mbr_tamano - (m.mbr_partition_4.part_start + m.mbr_partition_4.part_s);
    }
    return availSpace;
}

int availSpaceLogPart(const string path, const string &name){
    Partition ep = getExtPart(path);
    EBR start = getEBR(path, ep.part_start);
    int availSpace = -1;
    if (start.part_next != -1){
        EBR actual = getEBR(path, start.part_next);
        while (actual.part_next != -1){
            if (actual.part_name == name){
                availSpace = getEBR(path, actual.part_next).part_start - (actual.part_start + actual.part_s);
                return availSpace;
            }
            actual = getEBR(path, actual.part_next);
        }
        if (actual.part_name == name){
            availSpace = ep.part_s - (actual.part_start + actual.part_s);
        }
    }
    return availSpace;
}

bool choosePartToAdd(MBR &m, const string &name, int tam){
    int availSpace = availSpacePart(m, name);
    if (m.mbr_partition_1.part_name == name){
        if (tam < availSpace){
            m.mbr_partition_1.part_s = m.mbr_partition_1.part_s + tam;
            return true;
        }
    }else if (m.mbr_partition_2.part_name == name){
        if (tam < availSpace){
            m.mbr_partition_2.part_s = m.mbr_partition_2.part_s + tam;
            return true;
        }
    }else if (m.mbr_partition_3.part_name == name){
        if (tam < availSpace){
            m.mbr_partition_3.part_s = m.mbr_partition_3.part_s + tam;
            return true;
        }
    }else if (m.mbr_partition_4.part_name == name){
        if (tam < availSpace){
            m.mbr_partition_4.part_s = m.mbr_partition_4.part_s + tam;
            return true;
        }
    }
    return false;
}

bool chooseLogPartToAdd(const string path, const string &name, int tam){
    Partition ep = getExtPart(path);
    EBR e = getEBRByName(path, ep, name);
    int availSpace = availSpaceLogPart(path, name);
    if (tam < availSpace){
        e.part_s = e.part_s + tam;
        addEBR(path, e.part_start, e);
        return true;
    }
    return false;
}

bool addVolToPart(const string path, const string name, int tam){
    if (fs::exists(path)){
        if (partExists(path, name)){
            MBR m = getMBR(path);
            if (isPrimPart(m, name) || isExtPart(m, name)){
                if (choosePartToAdd(m, name, tam)){
                    addMBR(path, m);
                    cout<< "Espacio de particion extendido" <<endl;
                    return true;
                }
            }else if (isLogPart(path, name)){
                if (chooseLogPartToAdd(path, name, tam)){
                    cout<< "Espacio de particion logica exntendido" <<endl;
                    return true;
                }
            }
        }else{
            cout<< "ERROR: La particion no existe" <<endl;
        }
    }else{
        cout<< "ERROR: El disco \"" << getFileName(path) << "\" no existe" <<endl;
    }
    return false;
}

bool shrinkPartition(MBR &m, const string &name, int tam){
    if(m.mbr_partition_1.part_name == name){
        if (m.mbr_partition_1.part_s > abs(tam)){
            m.mbr_partition_1.part_s = m.mbr_partition_1.part_s - tam;
            return true;
        }
    }else if(m.mbr_partition_2.part_name == name){
        if (m.mbr_partition_2.part_s > abs(tam)){
            m.mbr_partition_2.part_s = m.mbr_partition_2.part_s - tam;
            return true;
        }
    }else if(m.mbr_partition_3.part_name == name){
        if (m.mbr_partition_3.part_s > abs(tam)){
            m.mbr_partition_3.part_s = m.mbr_partition_3.part_s - tam;
            return true;
        }
    }else if(m.mbr_partition_4.part_name == name){
        if (m.mbr_partition_4.part_s > abs(tam)){
            m.mbr_partition_4.part_s = m.mbr_partition_4.part_s - tam;
            return true;
        }
    }
    return false;
}

bool reducePartSize(const string path, const string name, int tam){
    if (fs::exists(path)){
        if (partExists(path, name)){
            MBR m = getMBR(path);
            if (isPrimPart(m, name) || isExtPart(m, name)){
                if (shrinkPartition(m, name, tam)){
                    addMBR(path, m);
                    cout<< "Espacio de particion reducido" <<endl;
                    return true;
                }
            }else if (isLogPart(path, name)){
                Partition ep = getExtPart(path);
                EBR e = getEBRByName(path, ep, name);
                if (e.part_s > abs(tam)){
                    e.part_s = e.part_s - tam;
                    addEBR(path, e.part_start, e);
                    cout<< "Espacio de particion logica reducido" <<endl;
                    return true;
                }
            }
        }else{
            cout<< "ERROR: La particion no existe" <<endl;
        }
    }else{
        cout<< "ERROR: El disco \"" << getFileName(path) << "\" no existe" <<endl;
    }
    return false;
}

//Number Partition Mounted Same Disk
//Devuelve el numero de particiones montadas del mismo disco
int numPartMtdSameDisk(const string path, const string name){
    int cont = 0;
    for(auto md : mds){
        if (md.path == path && md.name == name) cont++;
    }
    return cont;
}

string getIdMtdDisk(const string path, const string name){
    string lastNum = "31";
    return (lastNum + to_string(numPartMtdSameDisk(path, name)) + getFileName(path));
}

bool mountDisk(const string path, const string name){
    if (fs::exists(path)){
        if (partExists(path, name)){
            MountedDisk md = {path, name, getIdMtdDisk(path, name)};
            mds.push_back(md);
            cout<< "Disco montado correctamente" <<endl;
            return true;
        }else{
            cout<< "ERROR: La particion no existe" <<endl;
        }
    }else{
        cout<< "ERROR: El disco \"" << getFileName(path) << "\" no existe" <<endl;
    }
    return false;
}

bool idExists(const string id){
    for (auto md : mds){
        if (md.id == id) return true;
    }
    return false;
}

int getPartIdPos(const string id){
    int pos = -1;
    for (auto md : mds){
        pos++;
        if (md.id == id) return pos; 
    }
    return pos;
}

bool deleteID(const string id){
    try{
        auto elem_to_remove = mds.begin() + getPartIdPos(id);
        if (elem_to_remove != mds.end()) mds.erase(elem_to_remove);
        return true;
    }catch (const exception& e){
        cerr << e.what() << '\n';
    }
    return false;
}

bool unmountDisk(const string id){
    if (idExists(id)){
        if (deleteID(id)){
            cout<< "ID eliminado satifactoriamente" <<endl;
            return true;
        }else{
            cout<< "ERROR: El ID no pudo ser eliminado" <<endl;
        }
    }else{
        cout<< "ERROR: El ID de particion no existe" <<endl;
    }
    return false;
}