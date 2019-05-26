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
#include <stack>
#include <string>
#include <vector>

using std::ifstream;
using std::map;
using std::ofstream;
using std::pair;
using std::stack;
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
  string relative("modules/fuzzing/proto/");
  for (auto &w : FILE_LIST) {
    string file_path = relative + w + ".proto";
    ifstream input(file_path);
    // map<string, vector<pair<string, string>>> mp;
    map<string, pair<int, int>> range;
    string line;
    vector<vector<string>> all;  // all lines separated into 3 parts
    int line_num = 0;
    int content_start_line = 0;
    stack<pair<string, int>> st;
    while (getline(input, line)) {
      trim(line);
      ++line_num;
      stringstream ss(line);
      string a, b, c;
      ss >> a >> b;
      getline(ss, c);
      if (a == "message" || a == "enum") {
        if (content_start_line == 0) content_start_line = line_num;
        st.push(std::make_pair(b, line_num));
      } else {
        // nothing to do rightnow
      }
      for (auto c : line) {
        /* A local class definition has finished */
        if (c == '}') {
          auto p = st.top();
          st.pop();
          range[p.first] = {p.second, line_num};
        }
      }
      ltrim(c);
      /* Remove default and packed hint for convenience */
      vector<string> elim({"default", "packed"});
      for (auto &w : elim) {
        auto pos = c.find(w);
        if (pos != string::npos) {
          --pos;
          while (std::isspace(*(c.begin() + pos - 1))) --pos;
          c.replace(c.begin() + pos, c.end() - 1, "");
        }
      }

      all.push_back({a, b, c});
    }
    input.close();
    ofstream output(relative + w + "_fuzzed" + ".proto");
    for (int i = 1; i <= TOTALT; ++i) {
      char buf[32];
      sprintf(buf, "%03d", i);
      string i_leading0(buf);
      string new_all;
      for (int j = (i == 1) ? 1 : content_start_line; j <= line_num; ++j) {
        auto &line = all[j - 1];
        string &a = line[0];
        string b = line[1];
        string &c = line[2];
        string new_line;
        int type = -1;
        if (a == "message" || a == "enum") {
          new_line = a + " " + b + i_leading0 + " " + c;
        } else if ((type = is_basict(b)) != -1) {
          type += rand() % (9 - type);
          new_line = a + " " + BASIC_TYPES[type] + " " + c;
        } else {
          long unsigned int prepos = 0;
          auto pos = b.find('.', prepos);
          while (pos != string::npos) {
            string t(b.begin() + prepos, b.begin() + pos);
            if (range.count(t)) {
              b.replace(b.begin() + prepos, b.begin() + pos, t + i_leading0);
            } else {
              break;
            }
            prepos = pos + 3 + 1;
            pos = b.find('.', prepos);
          }
          string t(b.begin() + prepos, b.end());
          if (range.count(t)) {
            b.replace(b.begin() + prepos, b.end(), t + i_leading0);
          }
          new_line = a + " " + b + " " + c;
          trim(new_line);
        }
        new_all += new_line;
        new_all += "\n";
      }
      output << new_all;
    }
    output.close();
  }
  return 0;
}
