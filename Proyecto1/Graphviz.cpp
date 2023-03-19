#include "Graphviz.h"

// Crea las columnas y filas de la tabla para crear el mbr, espacios libres y ocupados
// Recibe la ruta del disco y el mbr
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
            if (sExt.in_use == 's')
                contInUse++;
    }
    int tamExt = ssExt.size();

    for (auto s : ss){
        int pct = getPercentage(s.part_s, m.mbr_tamano);
        if (s.in_use == 'n'){
            graph += "\t\t\t\t<td colspan=\"2\" rowspan=\"12\">Libre<br/><font point-size=\"8\">" + to_string(pct) + "\% del disco</font></td>\n";
        }
        else if (s.in_use == 's'){
            if (s.type == 'p'){
                graph += "\t\t\t\t<td colspan=\"2\" rowspan=\"12\">Primaria<br/><font point-size=\"8\">" + to_string(pct) + "\% del disco</font></td>\n";
            }
            else if (s.type == 'e'){
                graph += "\t\t\t\t<td colspan=\"" + to_string(tamExt + contInUse) + "\">Extendida<br/><font point-size=\"8\">" + to_string(pct) + "\% del disco</font></td>\n";
            }
        }
    }
    graph += "\t\t\t</tr>\n";

    if (ep.part_s > 0 && contInUse > 0){
        graph += "\t\t\t<tr>\n";
        for (auto sExt : ssExt){
            int pct = getPercentage(sExt.part_s, ep.part_s);
            if (sExt.in_use == 'n'){
                graph += "\t\t\t\t<td colspan=\"1\" rowspan=\"11\">Libre<br/><font point-size=\"8\">" + to_string(pct) + "\% del disco</font></td>\n";
            }else if (sExt.in_use == 's'){
                graph += "\t\t\t\t<td colspan=\"1\" rowspan=\"11\">EBR</td>\n";
                graph += "\t\t\t\t<td colspan=\"1\" rowspan=\"11\">Logica<br/><font point-size=\"8\">" + to_string(pct) + "\% del disco</font></td>\n";
            }
        }
        graph += "\t\t\t</tr>\n";
    }
    return graph;
}

// Crea el maquetado del inicio, intermedio, el final de la grafica y la retorna en un string
// Recibe como parametro la ruta del disco
string diskChart(const string path){
    MBR m = getMBR(path);
    string graph = "";
    string start = "digraph {\n\tnode [ shape=none fontname=Arial fontsize=12];\n\n\tn4 [ label = <\n";
    string st_table = "\t\t<table border=\"1\" color=\"dodgerblue3\">\n";
    string end = "\t\t</table>\n\t> ];\n\n\t{rank=same n4};\n}";
    graph += start;
    graph += st_table;
    graph += repDiskTR(path, m);
    graph += end;
    return graph;
}

// Crea el archivo dot y la imagen del reporte de disco
// Recibe como parametro la ruta donde se guardará la imagen y el id del disco
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
                    cout << "ERROR: El archivo dot no pudo ser creado" << endl;
                    exit(1);
                }
                fwrite(text.c_str(), 1, text.size(), myfile);
                fclose(myfile);
                cmd = "dot -T" + getFileExt(path).substr(1) + " " + ruta + " -o " + getPath(ruta) + getFileName(path);
                system(cmd.c_str());
                cout<< "Grafo creado correctamente" <<endl;
            }
            catch (const exception &e){
                cerr << e.what() << '\n';
            }
        }
    }
    else{
        cout << "ERROR: La particion no está montada" << endl;
    }
}

/* --------------------------------------------------------------------------------- */

//Retorna el maquetado de una fila de informacion que se le indique
//Recibe como parametro el nombre del parametro y su valor
string createRowMBRRpt(string param, string valor){
    string graph = "\t\t\t<tr>\n";
    graph += "\t\t\t\t<td border=\"0\" width=\"250\">" + param + "</td>\n";
    graph += "\t\t\t\t<td border=\"0\" width=\"250\">" + valor + "</td>\n";
    graph += "\t\t\t</tr>\n";
    return graph;
}

//Retorna el maquetado de la seccion de informacion del MBR
//Recibe como parametro la ruta del disco
string getTableMBRInfo(const string path){
    string graph = "";
    MBR m = getMBR(path);
    graph += "\t\t\t<tr><td colspan=\"2\" bgcolor=\"slateblue\" border=\"1\" align=\"left\" width=\"500\" color=\"white\"><b><font point-size=\"16\" color=\"white\">REPORTE DE MBR</font></b></td></tr>\n";
    graph += createRowMBRRpt("mbr_tamano", to_string(m.mbr_tamano));
    string fecha = to_string(m.mbr_fecha_creacion.tm_year) + "-" + to_string(m.mbr_fecha_creacion.tm_mon) + "-" + to_string(m.mbr_fecha_creacion.tm_mday);
    graph += createRowMBRRpt("mbr_fecha_creacion", fecha + " " + to_string(m.mbr_fecha_creacion.tm_hour) + ":" + to_string(m.mbr_fecha_creacion.tm_min));
    graph += createRowMBRRpt("mbr_disk_signature", to_string(m.mbr_dsk_signature));
    return graph;
}

//Retorna el maquetado de las secciones de particion logica
//Recibe como parametro la ruta del disco
string getTableExtPartInfo(const string path){
    string graph = "";
    Partition ep = getExtPart(path);
    if (ep.part_s > 0){
        EBR start = getEBR(path, ep.part_start);
        if (start.part_next > 0){
            EBR actual = getEBR(path, start.part_next);
            
            while (actual.part_next != -1){
                graph += "\t\t\t<tr><td colspan=\"2\" bgcolor=\"tomato\" border=\"1\" align=\"left\" width=\"500\" color=\"white\"><b><font point-size=\"16\" color=\"white\">Particion Logica</font></b></td></tr>";
                graph += createRowMBRRpt("part_status", toString(actual.part_status));
                graph += createRowMBRRpt("part_next", to_string(actual.part_next));
                graph += createRowMBRRpt("part_fit", toString(actual.part_fit));
                graph += createRowMBRRpt("part_start", to_string(actual.part_start));
                graph += createRowMBRRpt("part_size", to_string(actual.part_s));
                graph += createRowMBRRpt("part_name", actual.part_name);
                
                actual = getEBR(path, actual.part_next);
            }
            if (actual.part_s > 0){
                graph += "\t\t\t<tr><td colspan=\"2\" bgcolor=\"tomato\" border=\"1\" align=\"left\" width=\"500\" color=\"white\"><b><font point-size=\"16\" color=\"white\">Particion Logica</font></b></td></tr>";
                graph += createRowMBRRpt("part_status", toString(actual.part_status));
                graph += createRowMBRRpt("part_next", to_string(actual.part_next));
                graph += createRowMBRRpt("part_fit", toString(actual.part_fit));
                graph += createRowMBRRpt("part_start", to_string(actual.part_start));
                graph += createRowMBRRpt("part_size", to_string(actual.part_s));
                graph += createRowMBRRpt("part_name", actual.part_name);
            }
        }
    }
    return graph;
}

//Retorna el maquetado de una seccion de Particion del mbr y si es exentedia tambien retorna las logicas
//Recibe como parametro la ruta del dsico y la particion
string getTablePartInfo(const string path, Partition p){
    string graph = "";
    // Partition p = getPartByName(path, name);
    if (p.part_s > 0){
        graph += "\t\t\t<tr><td colspan=\"2\" bgcolor=\"slateblue\" border=\"1\" align=\"left\" width=\"500\" color=\"white\"><b><font point-size=\"16\" color=\"white\">Particion</font></b></td></tr>";
        graph += createRowMBRRpt("part_status", toString(p.part_status));
        graph += createRowMBRRpt("part_type", toString(p.part_type));
        graph += createRowMBRRpt("part_fit", toString(p.part_fit));
        graph += createRowMBRRpt("part_start", to_string(p.part_start));
        graph += createRowMBRRpt("part_size", to_string(p.part_s));
        graph += createRowMBRRpt("part_name", p.part_name);
    }
    if (p.part_type == 'e'){
        graph += getTableExtPartInfo(path);
    }

    return graph;
}

// Crea el maquetado del inicio, intermedio, el final de la tabla y la retorna en un string
// Recibe como parametro la ruta del disco
string getMBRReport(const string path){
    string graph = "";
    string start = "digraph {\n\tnode [ shape=none fontname=Arial fontsize=12];\n\n\tn1 [ label = <\n";
    string st_table = "\t\t<table border=\"2\" cellspacing=\"0\" cellpadding=\"10\">\n";
    string end = "\t\t</table>\n\t> ];\n\n\t{rank=same n1};\n}";
    MBR m = getMBR(path);

    graph += start;
    graph += st_table;
    graph += getTableMBRInfo(path);
    
    graph += getTablePartInfo(path, m.mbr_partition_1);
    graph += getTablePartInfo(path, m.mbr_partition_2);
    graph += getTablePartInfo(path, m.mbr_partition_3);
    graph += getTablePartInfo(path, m.mbr_partition_4);
    
    graph += end;

    return graph;
}

// Crea el archivo dot y la imagen del reporte del MBR
// Recibe como parametro la ruta donde se guardará la imagen y el id del disco
void createMBRReport(const string path, const string id){
    string ruta = getPath(path) + getFileName(path) + ".dot";
    string cmd;
    if (idExists(id)){
        if (createDir(getPath(path))){
            string text = getMBRReport(getDiskMtd(id).path);
            try{
                FILE *myfile;
                myfile = fopen(ruta.c_str(), "w+");
                if (myfile == NULL){
                    cout << "ERROR: El archivo dot no pudo ser creado" << endl;
                    exit(1);
                }
                fwrite(text.c_str(), 1, text.size(), myfile);
                fclose(myfile);
                cmd = "dot -T" + getFileExt(path).substr(1) + " " + ruta + " -o " + getPath(ruta) + getFileName(path);
                system(cmd.c_str());
                cout<< "Grafo creado correctamente" <<endl;
            }
            catch (const exception &e){
                cerr << e.what() << '\n';
            }
        }
    }else{
        cout << "ERROR: La particion no está montada" << endl;
    }
}

/* ---------------------------------------------------------------- */

string getTableSBInfo(const string &path, const string &name){
    string graph = "";
    MBR m = getMBR(path);
    // MountedDisk md = getDisk
    if (isPrimPart(m, name) || isExtPart(m, name)){
        Partition p = getPartByName(path, name);
        SuperBlock sb = getSuperBlock(path, p.part_start);
        //?Aquí va el nombre del disco
        graph += createRowMBRRpt("sb_nombre_hd", p.part_name);
        //?Preguntar que es
        graph += createRowMBRRpt("sb_arbol_virtual_count", "0");
        //?Este tambien
        graph += createRowMBRRpt("sb_detalle_directorio_count", "160");
        graph += createRowMBRRpt("sb_inodos_count", to_string(sb.s_inodes_count));
        graph += createRowMBRRpt("sb_bloques_count", to_string(sb.s_blocks_count));
        //?Tambien preguntar
        graph += createRowMBRRpt("sb_arbol_virtual_free", to_string(sb.s_blocks_count));
        //?Este tambien, pero creo que se refiere al bitmap de bloques igual que el de los inodos
        graph += createRowMBRRpt("sb_detalle_directorio_free", to_string(sb.s_blocks_count));
        graph += createRowMBRRpt("sb_inodos_free", to_string(sb.s_free_inodes_count));
        graph += createRowMBRRpt("sb_bloques_free", to_string(sb.s_free_blocks_count));
        //?Tambien corroborar fechas
        string fecha = to_string(sb.s_mtime.tm_year) + "-" + to_string(sb.s_mtime.tm_mon) + "-" + to_string(sb.s_mtime.tm_mday);
        graph += createRowMBRRpt("sb_date_creacion", fecha + " " + to_string(sb.s_mtime.tm_hour) + ":" + to_string(sb.s_mtime.tm_hour));
        graph += createRowMBRRpt("sb_date_ultimo_montaje", fecha + " " + to_string(sb.s_mtime.tm_hour) + ":" + to_string(sb.s_mtime.tm_hour));
        graph += createRowMBRRpt("sb_montajes_count", to_string(sb.s_mnt_count));
        graph += createRowMBRRpt("sb_montajes_count", to_string(sb.s_mnt_count));

    }else if(isLogPart(path, name)){
        EBR e = getLogPartByName(path, name);
    }

    return graph;
}

string getSBReport(const string &path, const string &name){
    string graph = "";
    string start = "digraph {\n\tnode [ shape=none fontname=Arial fontsize=12];\n\n\tn1 [ label = <\n";
    string st_table = "\t\t<table border=\"2\" cellspacing=\"0\" cellpadding=\"10\">\n";
    string end = "\t\t</table>\n\t> ];\n\n\t{rank=same n1};\n}";

    graph += start;
    graph += st_table;
    graph += "\t\t\t<tr><td colspan=\"2\" bgcolor=\"slateblue\" border=\"1\" align=\"left\" width=\"500\" color=\"white\"><b><font point-size=\"16\" color=\"white\">REPORTE DE SUPERBLOQUE</font></b></td></tr>\n";

    return graph;
}

// Crea el archivo dot y la imagen del reporte del Super Bloque
// Recibe como parametro la ruta donde se guardará la imagen y el id del disco
void createSBReport(const string &path, const string &id){
    string ruta = getPath(path) + getFileName(path) + ".dot";
    string cmd;
    if (idExists(id)){
        if (createDir(getPath(path))){
            //!No está terminado, revisar
            string text = getMBRReport(getDiskMtd(id).path);
            try{
                FILE *myfile;
                myfile = fopen(ruta.c_str(), "w+");
                if (myfile == NULL){
                    cout << "ERROR: El archivo dot no pudo ser creado" << endl;
                    exit(1);
                }
                fwrite(text.c_str(), 1, text.size(), myfile);
                fclose(myfile);
                cmd = "dot -T" + getFileExt(path).substr(1) + " " + ruta + " -o " + getPath(ruta) + getFileName(path);
                system(cmd.c_str());
                cout<< "Grafo creado correctamente" <<endl;
            }
            catch (const exception &e){
                cerr << e.what() << '\n';
            }
        }
    }else{
        cout << "ERROR: La particion no está montada" << endl;
    }
}