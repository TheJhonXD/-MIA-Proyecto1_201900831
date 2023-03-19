#include "Tools.h"

const string WHITESPACE = " \n\r\t\f\v";

// Convierte una cadena de caracteres a minisculas y lo retorna
string toLowerCase(string s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c)
              { return std::tolower(c); });
    return s;
}

// Convierte una cadena de caracteres a mayusculas y lo retorna
string toUpperCase(string s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c)
              { return std::toupper(c); });
    return s;
}

// Quita los espacios en blanco que están a la derecha de la cadena
string trimEnd(const string &s) {
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == string::npos) ? "" : s.substr(start);
}

// Quita los espacios en blanco que están a la izquierda de la cadena
string trimStart(const string &s) {
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == string::npos) ? "" : s.substr(0, end + 1);
}

// Quita los espacios al inicio y al final de la cadena
string trim(const string &s) {
    return trimStart(trimEnd(s));
}

// Cambia los espacios dentro de comillas por un simbolo
// Recibe una cadena y un simbolo como entrada
string replaceSpace(string &s, const string &sym) {
    string s_aux = "";
    regex regexRule("(\"[^\"]*)(\\s+)([^\"]*\")");
    smatch match;
    while (regex_search(s, match,regexRule)) {
        //Reemplazo el simbolo
        s.replace(match.position(2), 1, sym);
        //Corto la cadena y la guardo en un auxiliar
        s_aux += s.substr(0, match.position(1) + match.str(0).length());
        //Guardo lo que seigue en la variable para que sea analizado.
        s = match.suffix();
    }
    //Compruebo que se haya hecho match
    if (s_aux.length() > 0) s = s_aux + s;
    return s;
}

// Cambia los ampersand dentro de comillas por un espacio
// Recibe una cadena y un simbolo como entrada
string replaceAmpersand(string &s, const string &sym) {
    regex regexRule("(\"[^\"]*)(&)+([^\"]*\")");
    smatch match;
    while (regex_search(s, match, regexRule))
    {
        s.replace(match.position(2), 1, sym);
    }
    return s;
}

// Divide una cadena en un vector de cadenas, usando un delimitador
// Recibe una cadena y un delimitador
vector<string> split(string s, const string del) {
    vector<string> elems;
    int start, end = -1 * del.size();
    do
    {
        start = end + del.size();
        end = s.find(del, start);
        elems.push_back(s.substr(start, end - start));
    } while (end != -1);
    return elems;
}

//Convierte un caracter a una cadena (char to string)
string toString(char name){
    string s;
    stringstream ss;
    ss << name;
    s = ss.str();                // o use `s = ss.str()`
    return s;
}

// Recibe un string y retorna un string con todos los comentarios eliminados
string deleteComments(string &s) {
    regex regexRule("(#.+)");
    return regex_replace(s, regexRule, "");
}

//Quita las comillas de una cadena
string removeQuotes(string s) {
    return (s.find("\"") == 0) ? s.substr(1, s.length() - 2) : s;
}

//Genera numeros aleatorios en el rango de 1 a 200
int getRandomNumber() {
    return 1 + rand()%(201 - 1);
}

//Remueve el archivo de la cadena ingresada y retorna solo la ruta
string getPath(fs::path p){
    return p.remove_filename().string();
}

//Retorna el nombre del archivo de la ruta
string getFileName(fs::path p){
    return p.stem().string();
}

//Retorna la extension del archivo de una ruta ingresada
string getFileExt(fs::path p){
    return p.extension().string();
}

//Crea los directorios de la ruta ingresada si no existen
bool createDir(string path){
    if (!fs::exists(getPath(path))){
        if (fs::create_directories(path)){
            cout<< "Directorio creado" <<endl;
        }else{
            cout<< "ERROR: No se pudo crear el directorio" <<endl;
            return false;
        }
    }
    return true;
}

int getPercentage(double size, double disk_size){
    return round((size/disk_size) * 100);
}