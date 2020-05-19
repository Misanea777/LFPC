#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <deque>

using namespace std;

int l = 88;

struct Deriv{
    string lhs;
    deque<string> rhs;
};

Deriv conv(deque<string> some){
    Deriv temp;
    temp.lhs = some[0];
    some.pop_front();
    temp.rhs = some;
    return temp;
}

vector<Deriv> readFIle()
{
  vector<Deriv> tmmp;
  string line;
  ifstream myfile("GNF.txt");
  deque<string> tmp;
  regex rgx("(\\->)|(\\|)");
    if(myfile.is_open())
    {
      while (getline(myfile, line))
      {
        if (line == "") continue;
        sregex_token_iterator iter(line.begin(), line.end(), rgx, -1);
        sregex_token_iterator end;
        for ( ; iter != end; ++iter)
             tmp.push_back(*iter);
        tmmp.push_back(conv(tmp));
        tmp.clear();    
      }
      myfile.close();
    }    
  return tmmp;
}
void removeNUll(vector<Deriv>* ptr, int i){
for (int k = 0; k < (*ptr).size(); k++)
  {
    for (int j = 0; j < (*ptr)[k].rhs.size(); j++)
    {
      if (!((*ptr)[k].rhs[j].find((*ptr)[i].lhs) != std::string::npos)) continue;
      string tmp = (*ptr)[k].rhs[j];
      tmp.erase(std::remove(tmp.begin(), tmp.end(), (*ptr)[i].lhs[0]), tmp.end());
      (*ptr)[k].rhs.push_back(tmp); 
    }
  
  }  
}
void emptyDer(vector<Deriv>* ptr ){
  for (int i = 0; i < (*ptr).size(); i++)
  {
    for (int j = 0; j < (*ptr)[i].rhs.size(); j++)
    {
      if((*ptr)[i].rhs[j] != "*") continue;
      (*ptr)[i].rhs.erase((*ptr)[i].rhs.begin() + j);
      removeNUll(ptr, i);
    }
  }
}

void remUnPro(vector<Deriv>* ptr, int index, string lhs){
  for (int i = 0; i < (*ptr).size(); i++)
    if ((*ptr)[i].lhs == lhs)
      (*ptr)[index].rhs.insert((*ptr)[index].rhs.end(),(*ptr)[i].rhs.begin(), (*ptr)[i].rhs.end());
}

void unitDer(vector<Deriv>* ptr){
  for (int i = 0; i < (*ptr).size(); i++)
  {
    for (int j = 0; j < (*ptr)[i].rhs.size(); j++)
    {
      if((*ptr)[i].rhs[j].size() != 1) continue;
      if((*ptr)[i].rhs[j][0] > 90 || (*ptr)[i].rhs[j][0] < 65) continue; 
      string tmp = (*ptr)[i].rhs[j];
      (*ptr)[i].rhs.erase((*ptr)[i].rhs.begin()+j);
      remUnPro(ptr,i,tmp);
    }
  }
}



void newV(vector<Deriv>* ptr, string temp){
  Deriv tmp;
  tmp.lhs = l;
  l++;
  tmp.rhs.push_back(temp.substr(1,2));
  for (int i = 0; i < (*ptr).size(); i++)
  {
    for (int j = 0; j < (*ptr)[i].rhs.size(); j++)
    {
      if ((*ptr)[i].rhs[j].size()>2){
          (*ptr)[i].rhs[j].erase(1, 2);
          (*ptr)[i].rhs[j].append(tmp.lhs);
      }
    }
  }
    (*ptr).push_back(tmp);
}

void newDer(vector<Deriv>* ptr, string tmp){
  Deriv temp;
  temp.lhs = l;
  l++;
  temp.rhs.push_back(tmp.substr(0,1));
  for (int i = 0; i < (*ptr).size(); i++)
  {
    for (int j = 0; j < (*ptr)[i].rhs.size(); j++)
    {
      if ((*ptr)[i].rhs[j][0] == tmp[0] && (*ptr)[i].rhs[j].size()>1){
        (*ptr)[i].rhs[j][0] = temp.lhs[0];
      }
    }
  }
  (*ptr).push_back(temp);
}

void makeCNF(vector<Deriv>* ptr){
  for (int i = 0; i < (*ptr).size(); i++)
  {
    for (int j = 0; j < (*ptr)[i].rhs.size(); j++)
    {
      if ((*ptr)[i].rhs[j].size() > 2)
        newV(ptr, (*ptr)[i].rhs[j]);
      if ((*ptr)[i].rhs[j].size() > 1 && !isupper((*ptr)[i].rhs[j][0]))
        newDer(ptr,(*ptr)[i].rhs[j]);
    }
  }
}

int main(){
  vector<Deriv> checkSome = readFIle();
  emptyDer(&checkSome);
  unitDer(&checkSome);
  makeCNF(&checkSome);
  for (int i = 0; i < checkSome.size(); i++)
  {
    cout<<checkSome[i].lhs<<" --> ";
    for (int j = 0; j < checkSome[i].rhs.size(); j++)
    {
      cout<<checkSome[i].rhs[j]<<" ";
    }
    cout<<"\n";
  }
    return 0;
}