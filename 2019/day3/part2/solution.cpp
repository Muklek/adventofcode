#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include<unordered_map>
#include<functional>
#include<climits>
#include<tuple>

typedef std::string st;
typedef std::tuple<int,int, int> ti;
typedef std::vector<st> vst;
typedef std::vector<vst> vvst;
typedef std::vector<ti> vt;
typedef std::unordered_map<int, vt> mivt;

typedef std::function<void (int&, const int&)> fv;
typedef std::function<bool (int&, int&)> fb;


class GeneratePath{
  public:
    GeneratePath(const vvst* const);
    ~GeneratePath() = default;
    int getShortestPath();

  private:
    const vvst* const paths{nullptr};
    mivt mapY, mapX;
    int y{2}, x{2};
    int weight{INT_MAX}, localWeight{0};
    int reach{0}, comReach{0};
    char cardinal{' '};

    void compute();
    void storePoint(int&, const fv&);
    void storePoint(const int&, const int&, const int&, mivt&);
    void matchPoint(int&, const fv&);
    void matchPoint(int&, int&, int&, const mivt&);
    int intersectPointWeight(const mivt&, const ti&,const int&, const int&);
};

fv const increment = [](int& val, const int& reach) -> void{val+=reach;};
fv const decrement = [](int& val, const int& reach) -> void{val-=reach;};


void GeneratePath::storePoint(const int& preVal, const int& newVal,
                              const int& index, mivt& map){
  ti values{std::make_tuple(newVal, preVal, comReach)};
  if(map.find(index) != map.end()) map.at(index).push_back(values);
  else map.insert({index, {values}});
}

void GeneratePath::storePoint(int& val, const fv& change){
  int const preY{y}, preX{x};
  change(val, reach);
  if(y == preY) storePoint(preX, x, y, mapY);
  else if(x == preX) storePoint(preY, y, x, mapX);
}

int GeneratePath::intersectPointWeight(const mivt& map, const ti& prevLine,
                                       const int& index, const int& val){
  int intersectX{0}, intersectY{0};

  if(&map == &mapX){
    intersectY = val;
    intersectX = index;
  }
  else{
    intersectY = index;
    intersectX = val;
  }

  int prevLinePos{std::get<0>(prevLine)};
  int prevLineWeight{std::get<2>(prevLine)};

  if(intersectX == x){
    localWeight = std::abs(std::abs(intersectY - y) - comReach);
    localWeight += std::abs(std::abs(intersectX - prevLinePos) - prevLineWeight);
  }
  else{
    localWeight = std::abs(std::abs(intersectX - x) - comReach);
    localWeight += std::abs(std::abs(intersectY - prevLinePos - prevLineWeight));
  }
}

void GeneratePath::matchPoint(int& init, int& max, int& val, const mivt& map){
  for(int i{init}; i<=max; i++){
    if(map.find(i) != map.end())
      for(const ti& values : map.at(i))
        if((std::get<0>(values) <= val && std::get<1>(values) >= val) ||
           (std::get<1>(values) <= val && std::get<0>(values) >= val)){
          localWeight = 0;
          intersectPointWeight(map, values, i, val);
          if(localWeight != 0) weight = std::min(weight, localWeight);
        }
  }
}


void GeneratePath::matchPoint(int& val, const fv& change){
  int preY{y}, preX{x};
  change(val, reach);

  if(y == preY){
    if(x > preX) matchPoint(preX, x, y, mapX);
    else if(x < preX) matchPoint(x, preX, y, mapX);
  }
  else if(x == preX){
    if(y > preY) matchPoint(preY, y, x, mapY);
    else if(y < preY) matchPoint(y, preY, x, mapY);
  }
}

void GeneratePath::compute(){

  for(st const dir : paths->at(0)){
    cardinal = dir[0];
    reach = std::stoi(dir.substr(1, dir.size()-1));
    comReach += reach;

    if(cardinal == 'U') storePoint(y, increment);
    if(cardinal == 'L') storePoint(x, decrement);
    if(cardinal == 'R') storePoint(x, increment);
    if(cardinal == 'D') storePoint(y, decrement);
  }

  x = 2;
  y = 2;
  comReach = 0;

  for(st const dir: paths->at(1)){
    cardinal = dir[0];
    reach = std::stoi(dir.substr(1, dir.size()-1));
    comReach += reach;

    if(cardinal == 'U') matchPoint(y, increment);
    if(cardinal == 'L') matchPoint(x, decrement);
    if(cardinal == 'R') matchPoint(x, increment);
    if(cardinal == 'D') matchPoint(y, decrement);
  }
}

int GeneratePath::getShortestPath(){
  return weight;
}

GeneratePath::GeneratePath(const vvst* const l):paths(l){
  compute();
};


int main(){
  st line, token;
  std::ifstream file;

  vvst paths;

  file.open("input.txt");

  while(std::getline(file, line)){
    vst localPath;

    std::istringstream iss(line);
    while(std::getline(iss, token, ','))
      localPath.push_back(token);

    paths.push_back(localPath);
  }

  file.close();

  vvst paths2{{"R8","U5","L5","D3"},
              {"U7","R6","D4","L4"}};

  GeneratePath gp{&paths};
  std::cout<<gp.getShortestPath()<<std::endl;

}
