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
} PDM; //disk managment parameters

//Reseteo la estructura PDM
void resetPDM(){
    PDM.s = -1; PDM.f = "-1"; PDM.u = "-1"; PDM.path = "-1"; PDM.t = "-1";
    PDM.del = "-1"; PDM.name = "-1"; PDM.add = 0;
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

//Comprueba valores validos para el parametro -add
//Parametro opcional
bool check_param_add(int &add) {
    return (add > 0 || add < 0) ? true : false;
}

//Analiza y realiza las funciones que debe realizar mkdisk
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
        }
        //resetPDM(); //Reseto la estructura
    }
    resetPDM(); //Reseteo la estructura
}

//Analiza y realiza las funciones que debe realizar el comando rmdisk
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
            PDM.name = param[1];
        }else if (toLowerCase(param[0]) == ">add"){
            PDM.add = stoi(param[1]);
        }
    }

    //Compruebo los parametros
    //Parametros para crear una particion
    if (check_param_s(PDM.s) && check_param_path(PDM.path) && check_param_u(PDM.u)
        && check_param_name(PDM.name) && check_param_t(PDM.t)) {
        //Creo una variable particion para guardar los datos, excepto el inicio de partcion y el estado
        Partition p = {-1, *PDM.t.c_str(), *PDM.f.c_str(), -1, PDM.s, *PDM.name.c_str()};
        createPart(PDM.path, p); //Creo la particion y compruebo que todo ha salido correctamente
    }
        //Parametros para eliminar una particion
    else if (check_param_delete(PDM.del) && check_param_path(PDM.path) && check_param_name(PDM.name)){
        deletePart(PDM.path, PDM.name);
    }
        //Parametros para añadir o reducir volumen de una particion
    else if (check_param_add(PDM.add) && check_param_u(PDM.u) && check_param_path(PDM.path) && check_param_name(PDM.name)){
        (PDM.u == "k") ? PDM.add *= 1024 : PDM.add *= 1024*1024;
        if (PDM.add > 0){
            //?Codigo para añadir volumen
            
        }else if (PDM.add < 0){
            //!Codigo para reducir volumen
        }
    }else{
        cout<< "ERROR: No se pudo realizar la accion" <<endl;
    }

}