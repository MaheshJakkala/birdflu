//birdflu.c
#include "birdflu.h"

bool iself(Entry e)
{
    int32 fd;
    signed int ret;
    int8 path[64];
    char buff[4];

    if(e.type != file)
        return false;
    
   // zero(path,64);
    memset(path,0,64);
    snprintf($c path, 63,"%s/%s", $c e.dir, $c e.file);
    ret = open($c path, O_RDONLY);
    if(ret < 1)
        return false;
    else 
        fd=$4 ret;
    
    //zero($c buff,4);
    memset($c buff,0,4);
    read($i fd, buff, 4);
    close($i fd);

    //7f 45 4c 46
    if(
        (buff[0]==0x7f)
        && (buff[1]==0x45)
        && (buff[2]==0x4c)
        && (buff[3]==0x46)
    )
        return true;
    else    
        return false;
}

bool testfun(Entry e)
{
    return (e.type==dir);
}

Database *filter(Database *input,function f)
{
    int32 n;
    Database *output;
    Entry*p;
    bool predicate;

    output=mkdatabase();
    for(n=0;n<input->num;n++)
    {
        p=&input->entries[n];
        predicate=f(*p);
        if(predicate) 
            addtodb(output, *p);
    }
    destroydb(input);

    return output;

}
Database* mkdatabase()
{
    Entry *p;
    Database* db;
    int32 size;

    db = (Database*)malloc(sizeof(Database));
    assert(db);
    db->num=0;
    db->cap=Blocksize;
    size=Blocksize*sizeof(struct s_entry);
    p=(Entry*)malloc($i size);
    assert(p);
   // zero($i p,size);
   db->entries=p;
   memset(db->entries, 0, sizeof(Entry) * Blocksize);
    

    return db;

}
void destroydb(Database* db)
{
    db->cap=0;
    db->num=0;
    free(db->entries);
    free(db);
    
    return ;
}
void showdb(Database* db)
{
    printf("cap:\t%d\nnum:\t%d\n",db->cap,db->num);
    int32 n;
    for(n=0;n<db->num;n++)
        printf(" %s/ %s%c\n",db->entries[n].dir,db->entries[n].file
        ,(db->entries[n].type == dir)? '/':0 
        );
    return ;
} 
void addtodb(Database *db,Entry e1)
{
    int32 size,cap;
    int32 ix;
    if(db->num==db->cap)
    {
        cap=db->cap+Blocksize;
        size=cap*sizeof(struct s_entry);
        db->entries=realloc(db->entries,size);
        assert(db->entries);
        db->cap=cap;

    }
    ix=db->num;
    memcpy($c &db->entries[ix], $c &e1,sizeof(struct s_entry));
    db->num++;

    return;

}
bool adddir(Database*db,int8*path)
{
    Entry e;
    int32 fd;
    int64 n;
    signed int ret;
    struct linux_dirent *p;
    int8*p2;
    int8 buff[102400],tmp[64];
    char* filename;

    ret=open($c path,O_RDONLY|O_DIRECTORY);
    if(ret<1)
        return false;
    else   
        fd = $4 ret;
    do{
    memset($c buff,0,sizeof(buff));
    ret=syscall(SYS_getdents,$i fd, buff,(sizeof(buff)-1));

    if(ret<0)
    {
        close($i fd);
        return false;
    }
    else if(!ret) break;
    n=ret;

    for(p2 =buff;n;n-=p->d_reclen, p2+=p->d_reclen)
    {
        p=(struct linux_dirent*)p2;
        //zero($1 &e,sizeof(struct s_entry));
        memset(&e,0, sizeof(struct s_entry));

        filename=((p->d_name)-1);
        if(onedot(filename) || twodots(filename)) continue;

        if(p->d_type & DT_REG)
        {
            e.type=file;
            strncpy($c e.dir, $c path, 63);
            strncpy($c e.file, $c filename, 31);
            addtodb(db,e);
        }
        
       else if(p->d_type & DT_DIR)
        {
            e.type=dir;
            strncpy($c e.dir, $c path, 63);
            strncpy($c e.file, $c filename, 31);
            addtodb(db,e);

        
            memset(tmp,0,64);
            //zero(tmp,64);
            snprintf($c tmp,63,"%s/%s",$c path, $c e.file );
            adddir(db,tmp);
        }
    }
    }while(true);
    close($i fd);
    return true;
}
int main(int argc,char*argv[])
{

    Database* db1,*db2;

    assert(argc>1);

    db1=mkdatabase();
    adddir(db1,$1 argv[1]);;
    db2=filter(db1,&iself);
    showdb(db2);
    destroydb(db2);
    return 0;
}