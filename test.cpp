
#include<assert.h>
#include<iostream>
using namespace std;


class Date 
{ 
public: 
  Date(int year = 1900, int month = 1, int day = 1) {
    _year=year;
    _month=month;
    _day=day;
    int days=GetMonthDays(year,month);
    if(day==-1||day<1||day>days){
      cout<<"日期不合法"<<endl;
      assert(false);
    }
  }
// 如何检查一个日期是否合法
Date(const Date &d)//拷贝构造函数当作是构造函数的重载
{
  _year=d._year;
  _month=d._month;
  _day=d._month;
}

~Date(){

}
//打印日期
void display(){
  cout<<_year<<"-"<<_month<<"-"<<_day<<endl;
}
//d1 > d2 
bool operator>(const Date& d){

  if(_year>d._year){
    return true;
  }else if(_year==d._year&&_month>d._month){
    return true;
  }else if(_year>d._year&&_month>d._month&&_day>d._day){
    return true;
  }else{
    return false;
  }
}
//operator>=
bool operator>=(const Date& d){
  return (*this>d)||(*this==d);
}
//operator< 
bool operator<(const Date & d){
  return !(*this>=d);
}
//operator<= 
bool operator<=(const Date& d){
  return !(*this>d);
}
//operator==
bool operator==(const Date& d){
  return (_year=d._year&&_month==d._month&&_day==d._day);
}
//operator!= 
bool operator!=(const Date& d){
  return (_year!=d._year||_month!=d._month||_day!=d._day);
}
//d1 + 100 
Date operator=(const Date& d){ //复制运算符的重载
 
  if(this!= &d){
     this->_year=d._year;
     this->_month=d._month;
     this->_day=d._day;
  }
  return *this;
}
Date operator+(int day){//计算加上几天后的日期
  //if(day<0){
    //return operator-(-day);
  //}

  //Date tmp=*this;
  //int sumdays=tmp._day+day;
  //while(sumdays>GetMonthDays(tmp._year,tmp._month)){
     //sumdays -=GetMonthDays(tmp._year,tmp._month);
     //tmp._month++;
     //if(tmp._month>12){
       //tmp._year++;
       //_month %=12;
       //tmp._month=1;
     //}else{
       //tmp._day=sumdays;
     //}
  //}
  //return tmp;
Date tmp(*this);  
    tmp += day;  
    return tmp; 
} 
Date operator+=(int day){
  //*this=operator+(day);
  //return *this 
  
 _day += day;  
    while (_day > GetMonthDays(_year,_month))  
    {  
        _day -= GetMonthDays(_year,_month);  
        _month++;  
        if (_month > 12)  
        {  
            _month = 1;  
            _year += 1;  
        }  
    }  
    return *this;  
}
Date operator-(int day){
  //if(day<0){
    //return operator+(-day);
  //}
  //Date tmp=*this;
  //if(day>tmp._day){
    //day -=tmp._day;
    //if(tmp._month==1){
      //--tmp._year;
      //tmp._month=12;
    //}else{
      //tmp._month--;
    //}
    //tmp._day=GetMonthDays(tmp._year,tmp._month);
  //}
  //tmp._day -=day;
  //return tmp;
Date tmp(*this);  
    tmp -= day;  
    return tmp;
} 
Date operator-=(int day){
  //*this=operator-(day);
  //return *this;
_day -= day;  
    while (_day < 1)  
    {  
        --_month;  
        if (_month < 1)  
        {  
            --_year;  
            _month = 12;  
        }  
        _day += GetMonthDays(_year,_month);  
    }  
    return *this;  
} 
Date operator++(){//前置++
  _day+=1;
  if(_day>GetMonthDays(_year,_month)){
    if(_month==12){
      ++_year;
      _month=1;
    }else{
      ++_month;
    }
     _day=1;
  }
  return *this;
} 
Date operator++(int){

  Date tmp;
  *this=operator++();
  return tmp;
}//后置++ 
Date operator--(){

  _day-=1;
  if(_day<1){
    --_month;
    if(_month<1){
      --_year;
      _month=12;
    }
    _day=GetMonthDays(_year,_month);
  }
  return *this;
} 
Date operator--(int){
  Date tmp;
  *this=operator--();
  return tmp;
} 
int operator-( Date& d){
  //计算两个日期间相差的天数
  if(_year<d._year){
    Date tmp=*this;
    *this=d;
    d=tmp;
  }else if(_year==d._year&&_month<d._month){
    Date tmp=*this;
    *this=d;
    d=tmp;
  }else if(_year==d._year&&_month==d._month&&_day<d._day){

    Date tmp=*this;
    *this=d;
    d=tmp;
  }
  Date tmp1(*this);
  Date tmp2(d);
  int ret=0;
    while(!(tmp1==tmp2)){
      tmp2.operator++();
      ret++;
    }

    return ret;
}

bool leapyear(int year){

  if(((year%4==0) && (year%100!=0))|| (year%400==0)){
    return true;
  }
    return false;

}
// if /switch/array 
int GetMonthDays(int year, int month){     //根据已知年月获得该日期的天数

  int monthdays[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
  int days=monthdays[month];

  if(month==2&&leapyear(year)){
    days+=1;
  }
  return days;

}
private: 
int _year; 
int _month; 
int _day; 
};


int main(){

  Date d1(2018,6,5);
  Date d2(2018,6,6);
  d1.display();
  d2.display();
  Date ret=d1-3;
  ret.display();
 //Date ret=d1+30;
//ret.display();
  //Date rey=d2+60;
 // rey.display();
 // Date reu=d1-30;
 // reu.display();
 //Date rei=d2-60;
 // rei.display();

}

