#include "Commands.h"

struct {
    int s = -1;
    string f = "-1";
    string u = "-1";
    string path = "-1";
    string t = "-1";
    string del = "-1"; //delete
    string name = "-1";
    int add = 0;
    string id = "-1";
    string ruta = "-1";
} PDM; //disk managment parameters

//Reseteo la estructura PDM
void resetPDM(){
    PDM.s = -1; PDM.f = "-1"; PDM.u = "-1"; PDM.path = "-1"; PDM.t = "-1";
    PDM.del = "-1"; PDM.name = "-1"; PDM.add = 0; PDM.id = "-1"; PDM.ruta = "-1";
}

//Comprueba valores validos para el parametro -s
bool check_param_s(int &s) {
    if (s > 0){
        return true;
    }else{
        cout<< "ERROR: Tamanio no valido" <<endl;
    }
    return false;
}

//Comprueba valores validos para el parametro -path
bool check_param_path(string &path) {
    return (path != "-1") ? true : false;
}

//Comprueba valores validos para el parametro -f
//Parametro opcional
bool check_param_f(string &f) {
    if (f == "-1"){
        PDM.f = "ff";
        return true;
    }
    if (f == "bf" || f == "ff" || f == "wf"){
        return true;
    }else{
        cout<< "ERROR: \"" << f << "\" no es un valor valido" <<endl;
    }
    return false;
}

//Comprueba valores validos para el parametro -f
//Parametro opcional
bool check_param_f_fdisk(string &f) {
    if (f == "-1"){
        PDM.f = "wf";
        return true;
    }
    if (f == "bf" || f == "ff" || f == "wf"){
        return true;
    }else{
        cout<< "ERROR: \"" << f << "\" no es un valor valido" <<endl;
    }
    return false;
}

//Comprueba valores validos para el parametro -u
//Parametro opcional
bool check_param_u(string &u) {
    if (u == "-1"){
        PDM.u = "m"; //valor por defecto par mkdisk
        return true;
    }
    if (u == "k" || u == "m"){
        return true;
    }else{
        cout<< "ERROR: \"" << u << "\" no es un valor valido" <<endl;
    }
    return false;
}

//Comprueba valores validos para el parametro -u
//Parametro opcional
bool check_param_u_fdisk(string &u) {
    if (u == "-1"){
        PDM.u = "k";
        return true;
    }
    if (u == "k" || u == "m" || u == "b"){
        return true;
    }else{
        cout<< "ERROR: \"" << u << "\" no es un valor valido" <<endl;
    }
    return false;
}

//Comprueba valores validos para el parametro -t
//Parametro opcional
bool check_param_t(string &t) {
    if (t == "-1"){
        PDM.t = "p"; return true;
    }
    if (t == "p" || t == "e" || t == "l"){
        return true;
    }else{
        cout<< "ERROR: \"" << t << "\" no es un valor valido" <<endl;
    }
    return false;
}

//Comprueba valores validos para el parametro -delete
//Parametro opcional
bool check_param_delete(string &del) {
    if (del == "full"){
        return true;
    }else{
        cout<< "ERROR: \"" << del << "\" no es un valor valido" <<endl;
    }
    return false;
}

//Comprueba valores validos para el parametro -name
bool check_param_name(string &name) {
    return (name != "-1") ? true : false;
}

//Comprueba valores validos para el parametro -name de los reportes
bool check_param_name_rep(string &name){
    if (name == "mbr" || name == "disk" || name == "inode" || name == "journaling"
    || name == "block" || name == "bm_inode" || name == "bm_block" || name == "tree"
    || name == "sb" || name == "file" || name == "ls"){
        return true;
    }else{
        cout<< "ERROR: El nombre del reporte no es valido" <<endl;
    }
    return false;
}

//Comprueba valores validos para el parametro -add
//Parametro opcional
bool check_param_add(int &add) {
    return (add != 0) ? true : false;
}

bool check_param_id(string &id){
    return (id != "-1") ? true : false;
}

//Analiza y realiza las funciones que debe realizar mkdisk (crear un disco)
//Recibe un vector de parametros
void MKDISK(vector<string> params) {
    vector<string> param;
    for (size_t i=1; i<params.size(); i++){
        //Separo el parametro de su valor
        param = split(params[i], "=");
        /* Guardo los valores obtenidos en la estructura */
        if (toLowerCase(param[0]) == ">size"){
            PDM.s = stoi(param[1]);
        }else if (toLowerCase(param[0]) == ">path"){
            PDM.path = removeQuotes(param[1]);
        }else if (toLowerCase(param[0]) == ">unit"){
            PDM.u = toLowerCase(param[1]);
        }else if (toLowerCase(param[0]) == ">fit"){
            PDM.f = toLowerCase(param[1]);
        }else{
            cout<< "ERROR: El parametro " << param[0] << " no es valido" <<endl;
        }
    }
    //Compruebo si son valores validos
    if (check_param_s(PDM.s) && check_param_f(PDM.f) && check_param_u(PDM.u) && check_param_path(PDM.path)){
        (PDM.u == "k") ? PDM.s *= 1024 : PDM.s *= 1024*1024; //Conversión de kilobytes y megabytes.
        //Creo el disco y compruebo su creación.
        if (createDisk(PDM.path, PDM.s)){
            //Creo e inicializo el MBR con los datos obtenidos
            MBR m = {PDM.s, getTime(), getRandomNumber(), *PDM.f.c_str(), RPV(), RPV(), RPV(), RPV()};
            addMBR(PDM.path, m); //Añado el MBR al disco creado
            readMBR(PDM.path);
        }
        //resetPDM(); //Reseto la estructura
    }
    resetPDM(); //Reseteo la estructura
}

//Analiza y realiza las funciones que debe realizar el comando rmdisk (eliminar un disco)
//Recibe un vector de parametros
void RMDISK(vector<string> params) {
    vector<string> param;
    for (size_t i=1; i<params.size(); i++){
        //Separo el parametro de su valor
        param = split(params[i], "=");
        //Guardo los valores obtenidos en la estructura
        if (toLowerCase(param[0]) == ">path"){
            PDM.path = removeQuotes(param[1]);
        }else{
            cout<< "ERROR: El parametro " << param[0] << " no es valido" <<endl;
        }
    }
    //**Si hay un error en los paremetros igual pasa por este if, pasa en todos los comandos
    //Compruebo si son valores validos
    if (check_param_path(PDM.path)){
        //Elimino el disco
        deleteDisk(PDM.path);
        //Reseteo la estructura
        //resetPDM(); //Reseteo la estructura
    }
    resetPDM(); //Reseteo la estructura
}

//Analiza y realiza las funciones que debe realizar el comando fdisk
//Recibe un vector de parametros
void FDISK(vector<string> params) {
    vector<string> param;
    for (size_t i=1; i<params.size(); i++){
        //Separo el parametro de su valor
        param = split(params[i], "=");
        /* Guardo los valores obtenidos en la estructura */
        if (toLowerCase(param[0]) == ">size"){
            PDM.s = stoi(param[1]);
        }else if (toLowerCase(param[0]) == ">unit"){
            PDM.u = toLowerCase(param[1]);
        }else if (toLowerCase(param[0]) == ">path"){
            PDM.path = removeQuotes(param[1]);
        }else if (toLowerCase(param[0]) == ">type"){
            PDM.t = toLowerCase(param[1]);
        }else if (toLowerCase(param[0]) == ">fit"){
            PDM.f = toLowerCase(param[1]);
        }else if (toLowerCase(param[0]) == ">delete"){
            PDM.del = toLowerCase(param[1]);
        }else if (toLowerCase(param[0]) == ">name"){
            PDM.name = removeQuotes(param[1]);
        }else if (toLowerCase(param[0]) == ">add"){
            PDM.add = stoi(param[1]);
        }else{
            cout<< "ERROR: El parametro " << param[0] << " no es valido" <<endl;
        }
    }

    //Compruebo los parametros
    //Parametros para crear una particion
    if (PDM.del == "-1" && PDM.add == 0){
        if (check_param_s(PDM.s) && check_param_path(PDM.path) && check_param_u_fdisk(PDM.u)
            && check_param_name(PDM.name) && check_param_t(PDM.t) && check_param_f_fdisk(PDM.f)) {
            //Conversión de kilobytes y megabytes.
            if (PDM.u == "k")
                PDM.s *= 1024;
            else if (PDM.u == "m")
                PDM.s *= 1024*1024;
            //Creo una variable particion para guardar los datos, excepto el inicio de partcion y el estado
            Partition p = {-1, *PDM.t.c_str(), *PDM.f.c_str(), -1, PDM.s};
            strcpy(p.part_name, PDM.name.c_str());
            if (createPart(PDM.path, p)){ //Creo la particion y compruebo que todo ha salido correctamente
                readMBR(PDM.path);
            } 
        }else{
            cout<< "ERROR: No se pudo realizar la accion" <<endl;
        }
    }else if (PDM.add == 0){
        //Parametros para eliminar una particion
        if (check_param_delete(PDM.del) && check_param_path(PDM.path) && check_param_name(PDM.name)){
            deletePart(PDM.path, PDM.name);
        }else{
            cout<< "ERROR: No se pudo realizar la accion" <<endl;
        }
    }
        //Parametros para añadir o reducir volumen de una particion
    else if (check_param_add(PDM.add) && check_param_u_fdisk(PDM.u) && check_param_path(PDM.path) && check_param_name(PDM.name)){
        (PDM.u == "k") ? PDM.add *= 1024 : PDM.add *= 1024*1024;
        if (PDM.add > 0){
            addVolToPart(PDM.path, PDM.name, PDM.add);
        }else if (PDM.add < 0){
            reducePartSize(PDM.path, PDM.name, PDM.add);
        }
    }else{
        cout<< "ERROR: No se pudo realizar la accion" <<endl;
    }
    resetPDM();
}

//Analiza y realiza las funciones que debe realizar el comando MOUNT (montar una particion)
//Recibe un vector de parametros
void MOUNT(vector<string> params){
    vector<string> param;
    for (size_t i=1; i<params.size(); i++){
        //Separo el parametro de su valor
        param = split(params[i], "=");
        /* Guardo los valores obtenidos en la estructura */
        if (toLowerCase(param[0]) == ">name"){
            PDM.name = removeQuotes(param[1]);
        }else if (toLowerCase(param[0]) == ">path"){
            PDM.path = removeQuotes(param[1]);
        }else{
            cout<< "ERROR: El parametro " << param[0] << " no es valido" <<endl;
        }
    }
    //Compruebo si son valores validos
    if (check_param_path(PDM.path) && check_param_name(PDM.name)){
        mountDisk(PDM.path, PDM.name);
    }
    resetPDM();
}

//Analiza y realiza las funciones que debe realizar el comando UNMOUNT (Desmontar una partcion)
//Recibe un vector de parametros
void UNMOUNT(vector<string> params){
    vector<string> param;
    for (size_t i=1; i<params.size(); i++){
        //Separo el parametro de su valor
        param = split(params[i], "=");
        /* Guardo los valores obtenidos en la estructura */
        if (toLowerCase(param[0]) == ">id"){
            PDM.id = param[1];
        }else{
            cout<< "ERROR: El parametro " << param[0] << " no es valido" <<endl;
        }
    }

    if (check_param_id(PDM.id)){
        unmountDisk(PDM.id);
    }
    resetPDM();
}

void MKFS(vector<string> params){
    
}

//Analiza y realiza las funciones que debe realizar el comando rep (crear reportes)
//Recibe un vector de parametros
void REP(vector<string> params){
    vector<string> param;
    for (size_t i=1; i<params.size(); i++){
        //Separo el parametro de su valor
        param = split(params[i], "=");
        /* Guardo los valores obtenidos en la estructura */
        if (toLowerCase(param[0]) == ">name"){
            PDM.name = toLowerCase(removeQuotes(param[1]));
        }else if (toLowerCase(param[0]) == ">path"){
            PDM.path = removeQuotes(param[1]);
        }else if (toLowerCase(param[0]) == ">id"){
            PDM.id = param[1];
        }else if (toLowerCase(param[0]) == ">ruta"){
            PDM.ruta = removeQuotes(param[1]);
        }else{
            cout<< "ERROR: El parametro " << param[0] << " no es valido" <<endl;
        }
    }
    //?Solo prueba
    if (PDM.name == "mbr" && check_param_name_rep(PDM.name) && check_param_path(PDM.path) && check_param_id(PDM.id)){
        createMBRReport(PDM.path, PDM.id);
    }else if (PDM.name == "disk" && check_param_name_rep(PDM.name) && check_param_path(PDM.path) && check_param_id(PDM.id)){
        getDiskGraph(PDM.path, PDM.id);
    }
    resetPDM();
}