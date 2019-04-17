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

using std::ifstream;
using std::map;
using std::ofstream;
using std::pair;
using std::string;
using std::stringstream;
using std::vector;

const string FILE_LIST[8] = {"chassis",
                             "localization",
                             "perception_obstacle",
                             "planning",
                             "pose",
                             "prediction_obstacle",
                             "routing",
                             "traffic_light_detection"};
const string BASIC_TYPES[9] = {"bool",  "int32",  "uint32", "int64", "uint64",
                               "float", "double", "string", "bytes"};
const int TOTALT = 30;

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

int is_basict(const std::string &s) {
  for (int i = 0; i < 9; i++) {
    if (BASIC_TYPES[i] == s) return i;
  }
  return -1;
}

int main(int argc, char **argv) {
  srand((unsigned)time(NULL));
  for (auto &w : FILE_LIST) {
    string file_path = "proto/" + w + ".proto";
    ifstream input(file_path);
    map<string, vector<pair<string, string>>> mp;
    map<string, pair<int, int>> range;
    string line;
    vector<string> all;
    int line_num = 0, nest_lev = 0;
    int start_pos = 0;
    string now_in_range("");
    while (getline(input, line)) {
      trim(line);
      ++line_num;
      stringstream ss(line);
      string a, b;
      ss >> a >> b;
      if (a == "message") {
        if (!start_pos) start_pos = line_num;
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
      if (nest_lev == 0 && now_in_range != "") {
        range[now_in_range].second = line_num;
        now_in_range = "";
      }
      all.push_back(line);
    }
    input.close();
    for (int i = 1; i <= TOTALT; ++i) {
      string new_all;
      for (int j = 1; j < start_pos; ++j) {
        string &line = all[j - 1];
        new_all += line;
        new_all += "\n";
      }
      for (int j = start_pos; j <= line_num; ++j) {
        string &line = all[j - 1];
        stringstream ss(line);
        string a, b;
        ss >> a >> b;
        string new_line;
        getline(ss, new_line);
        ltrim(new_line);
        int type = -1;
        if (a == "message") {
          char buf[1024];
          sprintf(buf, "%s%03d", b.c_str(), i);
          new_line = a + " " + buf + " " + new_line;
        } else if ((type = is_basict(b)) != -1) {
          type += rand() % (9 - type);
          new_line = a + " " + BASIC_TYPES[type] + " " + new_line;
        } else if (range.count(b)) {
          char buf[1024];
          sprintf(buf, "%s%03d", b.c_str(), i);
          new_line = a + " " + buf + " " + new_line;
        } else {
          new_line = a + " " + b + " " + new_line;
          trim(new_line);
        }
        new_all += new_line;
        new_all += "\n";
      }
      char buf[1024];
      sprintf(buf, "proto/%s%03d.proto", w.c_str(), i);
      ofstream output(buf);
      output << new_all;
      output.close();
    }
  }
  return 0;
}
