#include "FileSystem.h"
Inodo createInode(const string &path, int gid, int size, char type){
    Inodo i = RIV();
    i.i_uid = 201900831;
    i.i_gid = gid;
    i.i_s = size;
    i.i_atime = getTime();
    i.i_ctime = getTime();
    i.i_mtime = getTime();
    i.i_type = type;

    return i;
}

FolderBlock createFB(const string &path, string parent, int iparent, string child, int ichild){
    FolderBlock fb = RFBV();
    strcpy(fb.b_content[0].b_name, child.c_str());
    fb.b_content[0].b_inodo = ichild;
    strcpy(fb.b_content[1].b_name, parent.c_str());
    fb.b_content[1].b_inodo = iparent;

    return fb;
}

/* bool bindInodeNBlock(){

} */

//Retorna el inicio de la particion indicada
//Recibe la ruta del disco y el nombre de la partición
int getPartStart(const string &path, const string &name){
    MBR m = getMBR(path);
    if (isPrimPart(m, name) || isExtPart(m, name)){
        return getPartByName(path, name).part_start;
    }else if (isLogPart(path, name)){
        return getLogPartByName(path, name).part_start;
    }
    return -1;
}

int getNextFreeSpaceInodeBmp(const string &path, int start, int tam){
    try {
        FILE *myfile;
        myfile = fopen(path.c_str(), "rb");
        fseek(myfile, tam - 1, SEEK_SET);
        //fread(&m, sizeof(MBR), 1, myfile);
        int c;
        int cPrev = 0;
        for (int i=(tam - 1); i>=start; i--){
            c = fgetc(myfile);
            if (c == 1 && cPrev == 0){
                return i;
            }
        }
        return start;
        fclose(myfile);
    } catch(const exception& e) {
        cerr << e.what() << '\n';
    }
    return -1;
}

int getNextFreeSpaceBlockBmp(const string &path, int start, int tam){
    try {
        FILE *myfile;
        myfile = fopen(path.c_str(), "rb");
        fseek(myfile, tam - 1, SEEK_SET);
        //fread(&m, sizeof(MBR), 1, myfile);
        int c;
        int cPrev = 0;
        for (int i=(tam - 1); i>=start; i--){
            c = fgetc(myfile);
            if (c == 1 && cPrev == 0){
                return i;
            }
        }
        return start;
        fclose(myfile);
    } catch(const exception& e) {
        cerr << e.what() << '\n';
    }
    return -1;
}

int getNewPosInode(const string &path, const string &name){
    int start = getPartStart(path, name);
    int newPos = -1;
    
    if (start != -1){
        SuperBlock sb = getSuperBlock(path, start);
        newPos = sb.s_bm_inode_start - getNextFreeSpaceInodeBmp(path, sb.s_bm_inode_start, sb.s_bm_block_start);
        return (sb.s_inode_start + (sizeof(Inodo) * newPos));
    }
    return -1;
}

int getNewPosFolderBlock(const string &path, const string &name){
    int start = getPartStart(path, name);
    int newPos = -1;
    
    if (start != -1){
        SuperBlock sb = getSuperBlock(path, start);
        newPos = sb.s_bm_block_start - getNextFreeSpaceBlockBmp(path, sb.s_bm_block_start, sb.s_inode_start);
        return (sb.s_block_start + (sizeof(FolderBlock) * newPos));
    }
    return -1;
}

FolderBlock getFBById(const string &path, const string &name, const int &idBlock){
    SuperBlock sp = getSuperBlock(path, getPartStart(path, name));
    FolderBlock fb;
    int cont = 0;
    try {
        FILE *myfile;
        myfile = fopen(path.c_str(), "rb");
        fseek(myfile, sp.s_bm_block_start, SEEK_SET);
        for (int i=0; i<sp.s_blocks_count; i++){
            if (cont == idBlock){
                fread(&fb, sizeof(FolderBlock), 1, myfile);
                return fb;
            }
            cont++;
        }
        fclose(myfile);
    } catch(const exception &e) {
        cerr << e.what() <<endl;
    }
    return RFBV();
}

FolderBlock getFileBlockById(const string &path, const string &name, const int &idBlock){
    SuperBlock sp = getSuperBlock(path, getPartStart(path, name));
    FolderBlock fb;
    int cont = 0;
    try {
        FILE *myfile;
        myfile = fopen(path.c_str(), "rb");
        fseek(myfile, sp.s_bm_block_start, SEEK_SET);
        for (int i=0; i<sp.s_blocks_count; i++){
            if (cont == idBlock){
                fread(&fb, sizeof(FolderBlock), 1, myfile);
                return fb;
            }
            cont++;
        }
        fclose(myfile);
    } catch(const exception &e) {
        cerr << e.what() <<endl;
    }
    return RFBV();
}

bool createRoot(const string &id){
    MountedDisk md = getDiskMtd(id);
    Inodo i = createInode(md.path, 0, -1, 1);
    FolderBlock fb = createFB(md.path, "..", 0, ".", 0);
    int start = getPartStart(md.path, md.name);
    if (start != -1){
        
        addInodo(md.path, getNewPosInode(md.path, md.name), i);
        addFolderBlock(md.path, getNewPosFolderBlock(md.path, md.name), fb);
        return true;
    }
    return false;
}

/* bool createUserFile(const string &id){

} */

