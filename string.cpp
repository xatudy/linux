#include<assert.h>
#include<string.h>
#include<iostream>
using namespace std;

class String 
{ 
public: 
  String( char* str = NULL){
    _size=strlen(str);
    _capacity=_size;
    _str=new char[strlen(str)+1];
    strcpy(_str,str);
  }//通用构造函数 
  String(const String& s){//深拷贝
    _str=new char[strlen(s._str)+1];

    strcpy(_str,s._str);
    _capacity=_size;
    _size=strlen(s._str);
  } //拷贝构造
  String& operator=(String s){
    Swap(s);
    return *this;
  }
void Swap(String& s){
  swap(_str,s._str);
  _capacity=s._capacity;
  _size=s._size;
} 
~String(){
  if(_str){
    delete [] _str;
    _str=NULL;
  }
  _size=0;
  _capacity=0;
} 
char* GetStr(){
  return _str;
}
size_t Size(){

  return _size;
} 
size_t Capacity(){
  return _capacity;
} 

// 增删查改 
void PushBack(char ch){//尾部插入字符串
  if(_size==_capacity){
    Expand(2*_capacity+1);
  }
  _str[_size]=ch;
  ++_size;
  _str[_size]='\0';
} 
void PushBack(const char* str) { //插入字符数组
  int len =strlen(str);
  if(_size+len>_capacity){
    Expand(_size+len+1);
  }
  for(int i=0;i<len;i++){
    _str[i+_size]=str[i];
  }
  _size=_size+len;
  _str[_size]='\0';
  _capacity=_size;


}
void PopBack(){            // 尾删

  if(_size){
    _str[_size-1]='\0';
    --_size;

}
}
void Insert(size_t pos, char ch){ //pos前插入一个字符

  if(_size==_capacity){
    Expand(2*_capacity+1);
  }
  int end=_size;
  //挪动数据
  while(end>=(int)pos){
   _str[end+1]=_str[end];
   --end;
  }
  _str[pos]=ch;
  ++_size;
}
void Insert(size_t pos, const char* str){
  size_t len =strlen(str);
  if(_size==_capacity){
    Expand(2*_capacity+1);
  }
  int end=_size;
  //挪动数据
  while(end>=(int)pos){
    _str[end+1]=_str[end];
    --end;
  }
  while(*str){
    _str[pos++]=*str++;
    _size=_size+len;
  }
}
void Erase(size_t pos, size_t count){ //从指定节点开始删除count长度

  if(pos+count>=_size-1){
    _str[pos]='\0';
    _size=pos;
  }else{
    strcpy(_str+pos,_str+pos+count);
    _size -=count;
  }
}
size_t Find(char ch) const{

  for(int i=0;i<(int)_size;i++){
    if(_str[i]==ch){
      return i;
    }
  }
  return -1;
} 
size_t Find(const char* str) const{
  assert(str);
  const char* str1=_str;
  const char* str2= str;
  size_t len2=strlen(str);
  size_t a1=0,a2=0;
  while(len2<_size){
    while(str1[a1]==str2[a2]){
      ++a1;
      ++a2;
      if(len2==a2){
        return a2;
      }
    }
    a2=0;
    a1++;
  }
  return -1;
} 
char& operator[](size_t pos)
{
  assert(pos<_size);
  return _str[pos];
}
bool operator<(const String& s) const{
  size_t i=0;
  for(;i<_size&&i<s._size;i++){
    if(_str[i]<s._str[i]){
      return true;
    }else if(_str[i]>s._str[i]){
      return false;
    } 
  }
  if(i==_size&&i<s._size){
    return true;
  }else{
    return false;
  }
} 
bool operator<=(const String& s) const{
  return *this<s||*this==s;
} 
bool operator>(const String& s) const{
  return !(*this<=s);
}

bool operator>=(const String& s) const{
  return !(*this<s);
} 
bool operator==(const String& s) const{
  size_t i=0;
  for(;i<_size&&i<s._size;i++){
    if(_str[i]!=s._str[i]){
      return false;
    }
  }
  if(i==_size&&i==s._size){
    return true;
  }else{
    return false;
  }
} 
bool operator!=(const String& s)const{
   
  return !(*this==s);

} 
void Expand(size_t n){ //增容
  char * tmp=new char[n];
  strcpy(tmp,_str);
  _str=tmp;
  _capacity=n-1;
} 
private: 
      char * _str; 
      size_t _size; // 字符个数 
      size_t _capacity; // 容量空间 
};

int main(){



  String s1("hello");
  //s1.push_back('');
  //s1.push_back('world');
  //String s2(s1);
  //cout<<s1.GetStr()<<endl;
  //cout<<s2.GetStr()<<endl;
  s1.PushBack(' ');
    s1.PushBack('C');
    s1.PushBack('h');
    s1.PushBack('i');
    s1.PushBack('n');
    s1.PushBack('a');
    cout<<s1.GetStr()<<endl;
    //cout<<s2.GetStr()<<endl;
  return 0;
}


