/******************************************************************************
 * Created by Smlight
 *****************************************************************************/

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
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
const int TOTALT = 30;  // number of iteration

/* trim from start (in place) */
static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                  [](int ch) { return !std::isspace(ch); }));
}

/* trim from end (in place) */
static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](int ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}

/* trim from both ends (in place) */
static inline void trim(std::string &s) {
  ltrim(s);
  rtrim(s);
}

/* Is a protobuf basic type? (could be speeded up) */
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
    // map<string, vector<pair<string, string>>> mp;
    map<string, pair<int, int>> range;
    string line;
    vector<vector<string>> all;  // all lines separated into 3 parts
    int line_num = 0, nest_lev = 0;
    int start_pos = 0;
    string now_in_range("");
    while (getline(input, line)) {
      trim(line);
      ++line_num;
      stringstream ss(line);
      string a, b, c;
      ss >> a >> b;
      getline(ss, c);
      if (a == "message") {
        if (!start_pos) start_pos = line_num;
        range[b] = {line_num, -1};
        now_in_range = b;
      } else {
        // nothing to do rightnow
      }
      for (auto c : line) {
        if (c == '{') {
          ++nest_lev;
        } else if (c == '}') {
          --nest_lev;
        }
      }
      /* A local class definition finished */
      if (nest_lev == 0 && now_in_range != "") {
        range[now_in_range].second = line_num;
        now_in_range = "";
      }
      ltrim(c);
      /* Remove default value for convenience */
      auto pos = c.find("default");
      if (pos != string::npos) {
        c.replace(c.begin() + pos - 1, c.end() - 1, "");
      }

      all.push_back({a, b, c});
    }
    input.close();
    for (int i = 1; i <= TOTALT; ++i) {
      char buf[32];
      sprintf(buf, "%03d", i);
      string i_leading0(buf);
      string new_all;
      for (int j = 1; j <= line_num; ++j) {
        auto &line = all[j - 1];
        string &a = line[0];
        string &b = line[1];
        string &c = line[2];
        string new_line;
        int type = -1;
        if (a == "message") {
          new_line = a + " " + b + i_leading0 + " " + c;
        } else if ((type = is_basict(b)) != -1) {
          type += rand() % (9 - type);
          new_line = a + " " + BASIC_TYPES[type] + " " + c;
        } else {
          auto pos = b.find('.');
          string t0, t1;
          if (pos != string::npos) {
            t0 = string(b.begin(), b.begin() + pos);
            t1 = string(b.begin() + pos, b.end());
          } else {
            t0 = b;
            t1 = "";
          }
          if (range.count(t0)) {
            new_line = a + " " + t0 + i_leading0 + t1 + " " + c;
          } else {
            new_line = a + " " + b + " " + c;
            trim(new_line);
          }
        }
        new_all += new_line;
        new_all += "\n";
      }
      ofstream output("proto/" + w + i_leading0 + ".proto");
      output << new_all;
      output.close();
    }
  }
  return 0;
}
