#include "Graphviz.h"

//Crea las columnas y filas de la tabla para crear el mbr, espacios libres y ocupados
//Recibe la ruta del disco y el mbr
string repDiskTR(const string path, MBR m){
    string graph = "";
    string start = "\t\t\t<tr>\n";
    string end = "\t\t\t</tr>\n";
    vector<SpaceSize> ss = BlockSize(path, m);
    vector<SpaceSize> ssExt;
    graph += start;
    graph += "\t\t\t\t<td colspan=\"1\" rowspan=\"12\">MBR</td>\n";

    Partition ep = getExtPart(path);

    int contInUse = 0;
    if (ep.part_s > 0){
        ssExt = ExtBlockSize(path);
        for (auto sExt : ssExt)
            if (sExt.in_use == 's') contInUse++;
    }
    int tamExt = sizeof(ssExt) / sizeof(SpaceSize);

    for (auto s : ss){
        int pct = getPercentage(s.part_s, m.mbr_tamano);
        if (s.in_use == 'n'){
            graph += "\t\t\t\t<td colspan=\"2\" rowspan=\"12\">Libre<br/><font point-size=\"8\">" + to_string(pct) + "\% del disco</font></td>\n";
        }else if(s.in_use == 's'){
            if (s.type == 'p'){
                graph += "\t\t\t\t<td colspan=\"2\" rowspan=\"12\">Primaria<br/><font point-size=\"8\">" + to_string(pct) + "\% del disco</font></td>\n";
            }else if (s.type == 'e'){
                graph += "\t\t\t\t<td colspan=\"" + to_string(tamExt + contInUse) + "\">Extendida<br/><font point-size=\"8\">" + to_string(pct) + "\% del disco</font></td>\n";
            }
        }
    }
    graph += "\t\t\t</tr>\n";
    
    if (ep.part_s > 0){
        graph += "\t\t\t<tr>\n";
        for (auto sExt : ssExt){
            int pct = getPercentage(sExt.part_s,  ep.part_s);
            if (sExt.in_use == 'n'){
                graph += "\t\t\t\t<td colspan=\"1\" rowspan=\"12\">Libre<br/><font point-size=\"8\">" + to_string(pct) + "\% del disco</font></td>\n";
            }else if(sExt.in_use == 's'){
                graph += "\t\t\t\t<td colspan=\"1\" rowspan=\"12\">EBR</td>\n";
                graph += "\t\t\t\t<td colspan=\"1\" rowspan=\"12\">Logica<br/><font point-size=\"8\">" + to_string(pct) + "\% del disco</font></td>\n";
            }
        }
        graph += "\t\t\t</tr>\n";
    }
    return graph;
}

//Crea el maquetado del inicio, intermedio, el final de la grafica y la retorna en un string
//Recibe como parametro la ruta del disco
string diskChart(const string path){
    MBR m = getMBR(path);
    string graph = "";
    string start = "digraph {\n\tnode [ shape=none fontname=Arial fontsize=12];\n\n\tn4 [ label = <\n";
    string st_table = "\t\t<table border=\"\" color=\"dodgerblue3\">\n";
    string end = "\t\t</table>\n\t> ];\n\n\t{rank=same n4};\n}";
    graph += start;
    graph += st_table;
    graph += repDiskTR(path, m);
    graph += end;
    return graph;
}

//Crea el archivo dot y la imagen del reporte de disco
//Recibe como parametro la ruta donde se guardará la imagen y el id del disco
void getDiskGraph(const string path, const string id){
    string ruta = getPath(path) + getFileName(path) + ".dot";
    // string rutaAux = "/home/jhonx/Escritorio/Carpetas/Pruebas/nombreXD";
    string cmd;
    
    if (idExists(id)){
        if (createDir(getPath(path))){
            string text = diskChart(getDiskMtd(id).path);
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
                cmd = "dot -T" + getFileExt(path).substr(1) + " " + ruta + " -o " + getPath(ruta) + getFileName(path);
                system(cmd.c_str());
            } catch(const exception& e){
                cerr << e.what() << '\n';
            }
        }
    }else{
        cout<< "ERROR: La particion no está montada" <<endl;
    }
}

void createMBRReport(const string path, const string id){
    //!Codigo aquí
}