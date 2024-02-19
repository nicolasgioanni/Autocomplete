#include "autocomplete.h"
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

void testBSTAll();

// takes in a file, and will parse the file by weight and city
void Autocomplete::readFile(const string &fileName) {
  std::ifstream ifs(fileName);
  if (!ifs.is_open()) {
    std::cout << "Cannot open!" << std::endl;
    exit(1);
  }
  std::string temp;
  std::getline(ifs, temp);
  while (std::getline(ifs, temp)) {
    std::istringstream ss(temp);
    std::vector<std::string> list;
    std::string temp2;
    while (ss >> temp2) {
      list.push_back(temp2);
    }
    if (!list.empty()) {
      std::string city;
      for (size_t i = 1; i < list.size(); i++) {
        city += list[i];
        city += ((i == list.size() - 1) ? "" : " ");
      }
      int num = std::stoi(list[0]);
      phrases[city] = num;
    }
  }
  ifs.close();
  phrases.rebalance();
  // cout << phrases << endl;
}

// performs the autocomplete using insertion and getAll functions
vector<BSTMap::value_type>
Autocomplete::complete(const BSTMap::key_type &prefix) const {
  vector<BSTMap::value_type> list1;
  vector<BSTMap::value_type> list2 = phrases.getAll(prefix);
  for (int i = 0; i < list2.size(); i++) {
    string a = list2[i].first.substr(0, prefix.size());
    if (a == prefix) {
      list1.push_back(list2[i]);
    }
  }

  // Sort the list1 vector in descending order using Insertion Sort
  for (int i = 0; i < list1.size(); i++) {
    BSTMap::value_type b = list1[i];
    int c = i - 1;
    while (c >= 0 && list1[c].second < b.second) {
      list1[c + 1] = list1[c];
      c--;
    }
    list1[c + 1] = b;
  }

  return list1;
}
