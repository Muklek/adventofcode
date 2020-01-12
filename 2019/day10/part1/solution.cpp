#include<iostream>
#include<fstream>
#include<string>
#include<vector>

typedef int16_t i;
typedef std::string s;
typedef std::vector<s> vs;


inline bool isVisibleAsteroid(const vs& rawmap,
                              float const xCen, float const yCen,
                              float const xRef, float const yRef){
  float x{xCen}, y{yCen}, change{0};
  float m{0}, n{0};

  if(xRef == xCen){
    y > yRef ? change = -1 : change = 1;
    y += change;

    for(; y != yRef; y += change)
      if(rawmap[y][x] != '.') return false;
  }
  else if(xCen > xRef || xCen < xRef){
    x > xRef ? change = -1 : change = 1;
    x += change;
    for(; x != xRef; x += change){
      m = (yRef - yCen) / (xRef - xCen);
      n = yRef - (m * xRef);
      y = (m * x) + n;

      if(static_cast<i>(y) - y == 0 && rawmap[y][x] != '.')
        return false;
    }
  }
  return true;
}


inline void getObservableAsteriods(const vs& rawmap, i& localCount,
                                   i const xRef, i const yRef) {
  for(int y{0}; y<rawmap.size(); y++)
    for(int x{0}; x<rawmap[y].size(); x++){

      if(y == yRef && x == xRef) continue;

      if(rawmap[y][x] != '.' && isVisibleAsteroid(rawmap, x, y, xRef, yRef)){
        ++localCount;
      }
    }
}

int16_t findLargestPosition(const vs& rawmap) {
  i count{0}, localCount{0};

  for(int y{0}; y<rawmap.size(); y++)
    for(int x{0}; x<rawmap[y].size(); x++){
      if(rawmap[y][x] != '.'){
        localCount = 0;
        getObservableAsteriods(rawmap, localCount, x, y);
        count = std::max(count, localCount);
      }
    }
  return count;
}


int main(){
  s line;
  vs rawmap;
  std::ifstream file;

  file.open("input.txt");
  while(std::getline(file, line)){
    rawmap.emplace_back(line);
  }
  file.close();

  i result{findLargestPosition(rawmap)};
  std::cout<<result<<std::endl;
}

