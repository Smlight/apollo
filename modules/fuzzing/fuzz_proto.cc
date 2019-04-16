/******************************************************************************
 * Created by Smlight
 *****************************************************************************/

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <locale>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace apollo {
namespace fuzzing {

string FILE_LIST[8] = {"chassis",
                       "localization",
                       "perception_obstacle",
                       "planning",
                       "pose",
                       "prediction_obstacle",
                       "routing",
                       "traffic_light_detection"};
string BASIC_TYPES[9] = {"bool",  "int32",  "uint32", "int64", "uint64",
                         "float", "double", "string", "bytes"};

// trim from start (in place)
static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                  [](int ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](int ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
  ltrim(s);
  rtrim(s);
}

int main() {
  for (auto &w : FILE_LIST) {
    string file_path = "proto/" + w + ".proto";
    ifstream pre(file_path);

    map<string, vector<pair<string, string>>> mp;
    map<string, pair<int>> range;
    string line;
    vector<string> all;
    int line_num = 0, nest_lev = 0;
    string now_in_range("");
    while (getline(pre, line)) {
      trim(line);
      ++line_num;
      stringstream ss(line);
      string a, b;
      ss >> a >> b;
      if (a == "message") {
        range[b] = {line_num, -1};
        now_in_range = b;
      } else {
      }
      for (auto c : line) {
        if (c == '{') {
          ++nest_lev;
        } else if (c == '}') {
          --nest_lev;
        }
      }
      if (nest_lev == 0) {
        range[now_in_range] = line_num;
        now_in_range = "";
      }
      all.push_back(line);
    }

    pre.close();
  }
  return 0;
}

}  // namespace fuzzing
}  // namespace apollo