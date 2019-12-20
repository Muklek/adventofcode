#include<iostream>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include<unordered_map>


typedef std::string st;
typedef std::vector<st> vst;
typedef std::pair<int, vst> pivst;
typedef std::pair<st,st> pst;
typedef std::pair<st, pivst> stivst;
typedef std::vector<pst> vpst;
typedef std::unordered_map<st, pivst> stpivst;


class Universe{
  public:
    Universe(vpst&);
    ~Universe() = default;

    int getTotalOrbit();

  private:
    int orbitNum{0}, totalOrbit{0};
    stpivst nodes;

    void build(vpst&);
    void propagateOrbit(st&, int);
    void calcTotalOrbit();
};



void Universe::calcTotalOrbit(){
  for(const stivst& node : nodes){
    totalOrbit += node.second.first;
  }
}

void Universe::propagateOrbit(st& parent, int orbit){
  vst planets{parent};

  for(int i{0}; i<planets.size(); i++){
    vst& childs{nodes.at(planets[i]).second};

    for(const st& child : childs)
      planets.emplace_back(child);
    
  }
  for(const st& planet : planets)
    nodes.at(planet).first += orbit;
}


void Universe::build(vpst& list){
  bool parentExist{false}, childExist{false};
  st parent{""}, child{""};

  for(const pst& val : list){
    parent = val.first;
    child = val.second;

    parentExist = nodes.find(parent) != nodes.end();
    childExist = nodes.find(child) != nodes.end();

    if(!parentExist && !childExist){
      nodes.insert({parent, {0, {child}}});
      nodes.insert({child, {1, {}}});
    }
    else if(!parentExist && childExist){
      nodes.insert({parent, {0, {child}}});
      propagateOrbit(child, 1);
    }
    else if(parentExist && !childExist){
      nodes.at(parent).second.emplace_back(child);
      orbitNum = nodes.at(parent).first + 1;
      nodes.insert({child, {orbitNum, {}}});
    
    }
    else if(parentExist && childExist){
      nodes.at(parent).second.emplace_back(child);
      orbitNum = nodes.at(parent).first + 1;
      propagateOrbit(child, orbitNum);
    }
  }
}

int Universe::getTotalOrbit(){
  return totalOrbit;
}

Universe::Universe(vpst& list){
  build(list);
  calcTotalOrbit();
}


int main(){
  st line;
  st token;
  std::ifstream file;

  pst val{"",""};
  vpst list;

  file.open("input.txt");

  while(getline(file, line)){
    std::istringstream iss(line) ;

    while(getline(iss, token, ')')){
      if(val.first == "") val.first = token;
      else val.second = token;
    }
    list.emplace_back(val);
    val.first = "";
    val.second = "";
  }

  file.close();

  Universe un{list};
  std::cout<<un.getTotalOrbit()<<std::endl;

}

