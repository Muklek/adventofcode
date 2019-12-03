#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>


typedef std::string st;
typedef std::vector<int> vi;


int getTotalFuel(const vi& ship){
  int totalFuel{0}, localFuel{0};

  for(int moduleMass : ship){
    while(moduleMass >= 0){
      localFuel = (static_cast<int>(moduleMass/3)) - 2;
      if(localFuel > 0) totalFuel += localFuel;
      moduleMass = localFuel;
    }
  }

  return totalFuel;
}



int main(){
  st line;
  std::ifstream file;

  vi nums;

  file.open("input.txt");

  while(std::getline(file, line)){
    int number{0};
    std::istringstream(line) >> number;
    nums.push_back(number);
  }

  file.close();

  int response{getTotalFuel(nums)};
  std::cout<<response<<std::endl;

}
