/*
   Array.addAll(Array l0);
   add all list of "l0" at the end of "this" array.
   Not surport Int[],Float[] but Number[]
*/
void Array.addAll(Array list){
    foreach(Any l in list){
        this.add(l);
    }
}

/*
   void Int[].addAll(Int[] list){
   foreach(int i in list){
   this.add(i);
   }
   }


   void Float[].addAll(Float[] list){
   foreach(Float i in list){
   this.add(i);
   }
   }
   */
/*
// example
void main(String[] args){
String[] a = ["a","b","c","d"];
String[] b = ["e","f","g"];

print a;
print b;

a.addAll(b);
print a;
}
*/
