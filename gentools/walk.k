#!/usr/local/bin/konoha
/*
   System.walk(String path);
   emulate python's os.walk;
*/
 
import "array.k"
 
String[] System.walk(String path){
    String[] p = new String[]();
    if(System.hasDir(path)){
        foreach(String t in System.listDir(path)){
            String newpath = path + "/" + t;
            if(System.hasDir(newpath)){
                p.addAll(System.walk(newpath));
            } else {
                p.add(newpath);
            }
        }
    }
    return p;
}

// example. read all dirs recurcivly without ".svn" file
/*
void main(String[] args){
    String[] list = System.walk(args[1]);
    String[] ret = new String[]();
    foreach(String line in list) {
        if(line.indexOf(".svn") < 0){
            ret.add(line + " \\");
        }
    }
    foreach(String l in ret){
        OUT << l << EOL;
    }
}
*/
