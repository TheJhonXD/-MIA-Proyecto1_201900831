#include "Graphviz.h"

string repDiskTR(const string path, MBR m){
    string graph = "";
    string start = "\t\t\t<tr>\n";
    string end = "\t\t\t</tr>\n";
    vector<SpaceSize> ss = BlockSize(path, m);
    graph += start;
    graph += "\t\t\t\t<td colspan=\"1\" rowspan=\"12\">MBR</td>\n";
    return graph;
}

string diskChart(const string path){
    MBR m = getMBR(path);
    string graph = "";
    string start = "digraph {\n\tnode [ shape=none fontname=Arial ];\n\n\tn4 [ label = <\n";
    string st_table = "\t\t<table border=\"\" color=\"dodgerblue3\">\n";
    string end = "\t\t</table>\n\t> ];\n\n\t{rank=same n4};\n}";
    graph += start;
    graph += st_table;
    graph += repDiskTR(path, m);
    return graph;
}

void getDiskGraph(const string path){
    string ruta = getPath(path) + getFileName(path) + ".dot";
    string rutaAux = "/home/jhonx/Escritorio/Carpetas/Pruebas/nombreXD";
    string cmd;
    if (createDir(path)){
        string text = diskChart(path);
        string ext = "png";
        try{
            FILE *myfile;
            myfile = fopen(ruta.c_str(), "w+");
            if (myfile == NULL){
                cout<< "ERROR: El disco no pudo ser creado" <<endl; 
                exit(1);
            }
            fwrite(text.c_str(), 1, text.size(), myfile);
            fclose(myfile);
            cmd = "dot -T" + ext + " " + ruta + " -o " + getFileName(path);
            system(cmd.c_str());
        } catch(const exception& e){
            cerr << e.what() << '\n';
        }
    }
}