#include<iostream>
#include<vector>
#include<string>

typedef std::string st;
typedef std::ifstream fi;
typedef std::vector<st> vst;




int main(){
  st line;
  fi file;
  vst pos;

  file.open("input.txt");

  while(std::getline(file, line, ',')){
    pos.push_back(line);
  }

  file.close();
}

