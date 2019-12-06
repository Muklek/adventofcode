#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include<unordered_map>
#include<functional>
#include<climits>

typedef std::string st;
typedef std::pair<int,int> pii;
typedef std::vector<st> vst;
typedef std::vector<vst> vvst;
typedef std::vector<pii> vp;
typedef std::unordered_map<int, vp> mivp;

typedef std::function<void (int&, const int&)> fv;
typedef std::function<bool (int&, int&)> fb;


class GeneratePath{
  public:
    GeneratePath(const vvst* const);
    ~GeneratePath() = default;
    int getShortestPath();

  private:
    const vvst* const paths{nullptr};
    mivp mapY, mapX;
    int y{2}, x{2};
    int shortestPath{INT_MAX}, reach{0};
    char cardinal{' '};

    void compute();
    void storePoint(int&, const fv&);
    void storePoint(const int&, const int&, const int&, mivp&);
    void matchPoint(int&, const fv&);
    void matchPoint(int&, int&, int&, const mivp&);
};

fv const increment = [](int& val, const int& reach) -> void{val+=reach;};
fv const decrement = [](int& val, const int& reach) -> void{val-=reach;};


void GeneratePath::storePoint(const int& newVal, const int& preVal,
                              const int& index, mivp& map){
  pii values;
  if(newVal < preVal) values = std::make_pair(newVal, preVal);
  else values = std::make_pair(preVal, newVal);

  if(map.find(index) != map.end()) map.at(index).push_back(values);
  else map.insert({index, {values}});
}

void GeneratePath::storePoint(int& val, const fv& change){
  int const preY{y}, preX{x};
  change(val, reach);
  if(y == preY) storePoint(preX, x, y, mapY);
  else if(x == preX) storePoint(preY, y, x, mapX);
}

void GeneratePath::matchPoint(int& init, int& max, int& val, const mivp& map){
  for(int i{init}; i<=max; i++){
    if(map.find(i) != map.end())
      for(const pii& values : map.at(i))
        if(values.first <= val && values.second >= val){
          int weight = std::abs(2-val) + std::abs(2-i);
          if(weight != 0) shortestPath = std::min(shortestPath, weight);
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

    if(cardinal == 'U') storePoint(y, increment);
    if(cardinal == 'L') storePoint(x, decrement);
    if(cardinal == 'R') storePoint(x, increment);
    if(cardinal == 'D') storePoint(y, decrement);
  }

  x = 2;
  y = 2;

  for(st const dir: paths->at(1)){
    cardinal = dir[0];
    reach = std::stoi(dir.substr(1, dir.size()-1));

    if(cardinal == 'U') matchPoint(y, increment);
    if(cardinal == 'L') matchPoint(x, decrement);
    if(cardinal == 'R') matchPoint(x, increment);
    if(cardinal == 'D') matchPoint(y, decrement);
  }
}

int GeneratePath::getShortestPath(){
  return shortestPath;
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

  GeneratePath gp{&paths};
  std::cout<<gp.getShortestPath()<<std::endl;

}
