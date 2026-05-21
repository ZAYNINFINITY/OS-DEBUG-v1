#include "users.h"
#include "display.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace users {

static string trim(const string& s) {
    size_t a = 0, b = s.size();
    while (a < b && isspace((unsigned char)s[a])) a++;
    while (b > a && isspace((unsigned char)s[b - 1])) b--;
    return s.substr(a, b - a);
}

static string sanitize_name(string s) {
    s = trim(s);
    if (s.empty()) return "Student";
    if (s.size() > 18) s = s.substr(0, 18);
    // Disallow delimiter used in files
    for (char& c : s) if (c == '|') c = '_';
    return s;
}

static vector<string> load_users() {
    vector<string> v;
    ifstream f("users.txt");
    if (!f.is_open()) return v;
    string line;
    while (getline(f, line)) {
        line = sanitize_name(line);
        if (!line.empty()) v.push_back(line);
    }
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    return v;
}

static void save_users(const vector<string>& v) {
    ofstream f("users.txt", ios::trunc);
    for (const auto& u : v) f << u << "\n";
}

string select_or_create() {
    vector<string> v = load_users();

    while (true) {
        Display::clear();
        int w = 56;
        Display::boxTop(w);
        Display::boxLine("  USER PROFILE", w);
        Display::boxSeparator(w);

        if (v.empty()) {
            Display::boxLine("  No users yet.", w);
            Display::boxLine("  1   Create new user", w);
            Display::boxLine("  2   Continue as Student", w);
            Display::boxBottom(w);

            cout << "\n  >> Select [1-2]: ";
            cout.flush();
            int c;
            cin >> c;
            cin.ignore();
            if (c == 2) return "Student";
            if (c != 1) continue;
        } else {
            Display::boxLine("  Select a user:", w);
            Display::boxSeparator(w);
            int idx = 1;
            for (const auto& u : v) {
                Display::boxLine("  " + to_string(idx) + "   " + u, w);
                idx++;
                if (idx > 8) break; // keep menu small
            }
            Display::boxSeparator(w);
            Display::boxLine("  9   Create new user", w);
            Display::boxBottom(w);

            int maxPick = (int)min<size_t>(v.size(), 8);
            cout << "\n  >> Select [1-" << (maxPick >= 1 ? maxPick : 1) << "] or [9]: ";
            cout.flush();
            int c;
            cin >> c;
            cin.ignore();
            if (c >= 1 && c <= maxPick) return v[c - 1];
            if (c != 9) continue;
        }

        // Create new user
        Display::clear();
        Display::divider();
        cout << "  Enter username (max 18 chars): ";
        cout.flush();
        string name;
        cin.clear();
        getline(cin, name);
        name = sanitize_name(name);
        if (name.empty()) name = "Student";

        bool exists = find(v.begin(), v.end(), name) != v.end();
        if (!exists) {
            v.push_back(name);
            sort(v.begin(), v.end());
            v.erase(unique(v.begin(), v.end()), v.end());
            save_users(v);
        }
        return name;
    }
}

} // namespace users

