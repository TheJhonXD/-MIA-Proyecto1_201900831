#include <iostream>
#include <filesystem>
#include <fstream>
#include "Tools.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <bits/stdc++.h>

using namespace std;
namespace fs = filesystem;

//Analiza la entrada
void analyzer(string s) {
    //Reemplazo los espacios dentro de las comillas por dolar
    replaceSpace(s, "&");
    //Compruebo si no es un comentario
    if (s.front() != '#') {
        deleteComments(s);
        s = trim(s);
    }
    //Divido la cadena en un vector
    vector<string> cmds = split(s, " ");
    //Reemplazo todos los signos de dolar por espacios
    for(string &cmd : cmds)
        replaceAmpersand(cmd, " ");
    //Devuelvo a su estado anterior la entrada
    replaceAmpersand(s, " ");

    if (toLowerCase(cmds[0]) == "mkdisk"){
        cout<< "»» " << s <<endl;
    }else if (toLowerCase(cmds[0]) == "rmdisk"){
        cout<< "»» " << s <<endl;
    }else if (toLowerCase(cmds[0]) == "fdisk"){
        cout<< "»» " << s <<endl;
    }else if (toLowerCase(cmds[0]) == "mount"){
        cout<< "»» " << s <<endl;
    }else if (toLowerCase(cmds[0]) == "unmount"){
        cout<< "»» " << s <<endl;
    }else if (toLowerCase(cmds[0]) == "mkfs"){
        cout<< "»» " << s <<endl;
    }else if (toLowerCase(cmds[0]) == "login"){
        cout<< "»» " << s <<endl;
    }else if (toLowerCase(cmds[0]) == "logout"){
        cout<< "»» " << s <<endl;
    }else if (toLowerCase(cmds[0]) == "mkgrp"){
        cout<< "»» " << s <<endl;
    }else if (toLowerCase(cmds[0]) == "rmgrp"){
        cout<< "»» " << s <<endl;
    }else if (toLowerCase(cmds[0]) == "mkusr"){
        cout<< "»» " << s <<endl;
    }else if (toLowerCase(cmds[0]) == "rmusr"){
        cout<< "»» " << s <<endl;
    }else if (toLowerCase(cmds[0]) == "rep"){
        cout<< "»» " << s <<endl;
    }else if (toLowerCase(cmds[0]) == "pause"){
        cout<< "Press any key to continue...";
        cin.get(); //Espero un caracter
    }else if (s.front() == '#'){
        cout<< s <<endl;
    }else if (s.length() == 0){
        cout<< "" <<endl;
    }else{
        cout<< "ERROR: el comando \"" << cmds[0] << "\" no es valido" <<endl;
    }
}

//Lee un archivo
void readFile(const string &path) {
    //Compruebo si existe el archivo
    if (fs::exists(path)) {
        try {
            //Preparo mi archivo para abrir
            ifstream myfile(path.c_str());
            string line;
            //Compruebo si está abierto
            if(myfile.is_open()){
                while (myfile) {
                    //Leo el contenido del archivo
                    getline(myfile, line);
                    if (line.length() != 0) {
                        line = trim(line);
                        analyzer(line);
                    }
                }
            }
        }
        catch(const exception& e) {
            cerr << e.what() << '\n';
        }
    }else{
        cout<< "El archivo no existe" <<endl;
    }
}

int main() {
    string input;
    srand(time(NULL));
    //fs::create_directories("../../Pruebas/prueba2/disco.dsk");
    cout<< "************* ⍟ Consola ⍟ *************" <<endl;
    do {
        //cin.ignore(numeric_limits<size_t>::max());
        cout<< "»» ";
        getline(cin, input);

        //limpia la entrada por teclado
        input = trim(input);
        //Reemplazo los espacios dentro de las comillas por dolar
        replaceSpace(input, "&");
        //Realizo un split de la entrada
        vector<string> cmds = split(input, " ");
        //Reemplazo todos los signos de dolar por espacios
        for (string &cmd: cmds)
            replaceAmpersand(cmd, " ");
        //Devuelvo a su estado anterior la entrada
        replaceAmpersand(input, " ");
        //Compruebo si es el comando para ejecutar un archivo
        if (toLowerCase(cmds[0]) == "execute") {
            //cout<<"FFFFFF"<<endl;
            //Remuevo las comillas y leo el archivo
            //cout<<split(cmds[1], "=")[1]<<endl;
            readFile(removeQuotes(split(cmds[1], "=")[1]));
        }else{
            if (input != "exit")
                analyzer(trim(input));
        }

    } while (input != "exit");

    return 0;
}


/*Compilar en vscode
    g++ -Wall -o main *.cpp
    ./main
*/