#include "algorithms.h"
#include "display.h"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Process {
    int pid;
    int arrival;
    int burst;
    int priority;
};

struct Result {
    int pid, burst, arrival, priority;
    int wait;
    int turnaround;
};

struct Segment {
    int pid;
    int units;
};

static vector<Process> makeProcesses() {
    return {{1,0,24,2}, {2,1,3,1}, {3,2,3,1}, {4,3,8,3}};
}

static void showHeader(string name) {
    int w = 54;
    cout << "\n";
    Display::boxTop(w);
    Display::boxLine("  ALGORITHM: " + name, w);
    Display::boxSeparator(w);
    Display::boxLine("  P1=Burst24  P2=Burst3  P3=Burst3  P4=Burst8", w);
    Display::boxLine("  Arrivals: 0,1,2,3   Priorities: 2,1,1,3", w);
    Display::boxBottom(w);
}

static void showGantt(vector<Segment> tl) {
    cout << "\n  GANTT CHART:\n\n";
    for (auto& s : tl) {
        if (s.units <= 0) continue;
        if (s.pid == 0) {
            cout << "  idle|";
            for (int i = 0; i < min(s.units, 24); i++) cout << ".";
            if (s.units > 24) cout << "...(" << s.units << ")";
            cout << "|\n";
        } else {
            cout << "  P" << s.pid << " |";
            for (int i = 0; i < min(s.units, 24); i++) cout << "#";
            if (s.units > 24) cout << "...(" << s.units << ")";
            cout << "|\n";
        }
    }
    cout << "\n";
}

static void showTable(vector<Result> rows) {
    sort(rows.begin(), rows.end(), [](Result a, Result b) {
        return a.pid < b.pid;
    });

    cout << "  +-----+-------+---------+----------+------+------------+\n";
    cout << "  | PID | Burst | Arrival | Priority | Wait | Turnaround |\n";
    cout << "  +-----+-------+---------+----------+------+------------+\n";

    double tw = 0, tt = 0;
    for (auto& row : rows) {
        tw += row.wait;
        tt += row.turnaround;
        cout << "  |  P" << row.pid
             << " | " << setw(5) << row.burst
             << " | " << setw(7) << row.arrival
             << " | " << setw(8) << row.priority
             << " | " << setw(4) << row.wait
             << " | " << setw(10) << row.turnaround << " |\n";
    }

    int n = rows.size();
    cout << "  +-----+-------+---------+----------+------+------------+\n";
    cout << fixed << setprecision(2);
    cout << "  Avg Wait: " << tw / n << "\n";
    cout << "  Avg Turnaround: " << tt / n << "\n\n";
}

static void segPush(vector<Segment>& tl, int pid, int units) {
    if (units <= 0) return;
    if (!tl.empty() && tl.back().pid == pid)
        tl.back().units += units;
    else
        tl.push_back({pid, units});
}

void Algorithms::fcfs() {
    showHeader("FCFS (First Come First Served)");

    vector<Process> procs = makeProcesses();
    sort(procs.begin(), procs.end(), [](Process a, Process b) {
        return a.arrival != b.arrival ? a.arrival < b.arrival : a.pid < b.pid;
    });

    vector<Segment> tl;
    vector<Result> results;
    int t = 0;

    for (auto& p : procs) {
        if (t < p.arrival) {
            segPush(tl, 0, p.arrival - t);
            t = p.arrival;
        }
        int start = t;
        segPush(tl, p.pid, p.burst);
        t += p.burst;
        results.push_back({p.pid, p.burst, p.arrival, p.priority,
                           start - p.arrival, t - p.arrival});
    }

    showGantt(tl);
    showTable(results);
}

void Algorithms::sjf() {
    showHeader("SJF (Shortest Job First)");

    vector<Process> procs = makeProcesses();
    sort(procs.begin(), procs.end(), [](Process a, Process b) {
        return a.arrival != b.arrival ? a.arrival < b.arrival : a.pid < b.pid;
    });

    vector<Segment> tl;
    vector<Result> results;
    vector<bool> done(procs.size(), false);
    int finished = 0, t = 0;

    while (finished < (int)procs.size()) {
        int pick = -1;
        for (int i = 0; i < (int)procs.size(); i++) {
            if (done[i] || procs[i].arrival > t) continue;
            if (pick == -1 || procs[i].burst < procs[pick].burst ||
               (procs[i].burst == procs[pick].burst && procs[i].pid < procs[pick].pid))
                pick = i;
        }

        if (pick == -1) {
            int nxt = 99999;
            for (int i = 0; i < (int)procs.size(); i++)
                if (!done[i]) nxt = min(nxt, procs[i].arrival);
            segPush(tl, 0, nxt - t);
            t = nxt;
            continue;
        }

        Process p = procs[pick];
        int start = t;
        segPush(tl, p.pid, p.burst);
        t += p.burst;
        done[pick] = true;
        finished++;
        results.push_back({p.pid, p.burst, p.arrival, p.priority,
                           start - p.arrival, t - p.arrival});
    }

    showGantt(tl);
    showTable(results);
}

void Algorithms::roundRobin(int q) {
    if (q <= 0) q = 4;
    showHeader("Round Robin (quantum=" + to_string(q) + ")");

    vector<Process> procs = makeProcesses();
    sort(procs.begin(), procs.end(), [](Process a, Process b) {
        return a.arrival != b.arrival ? a.arrival < b.arrival : a.pid < b.pid;
    });

    int n = procs.size();
    vector<int> rem(n), doneAt(n, -1);
    for (int i = 0; i < n; i++) rem[i] = procs[i].burst;

    vector<Segment> tl;
    queue<int> rq;
    int t = procs.front().arrival;
    int nextIdx = 0;

    while (nextIdx < n && procs[nextIdx].arrival <= t) {
        rq.push(nextIdx++);
    }

    while (true) {
        if (rq.empty()) {
            if (nextIdx >= n) break;
            segPush(tl, 0, procs[nextIdx].arrival - t);
            t = procs[nextIdx].arrival;
            while (nextIdx < n && procs[nextIdx].arrival <= t) {
                rq.push(nextIdx++);
            }
            continue;
        }

        int i = rq.front();
        rq.pop();
        int slice = min(q, rem[i]);
        segPush(tl, procs[i].pid, slice);
        t += slice;
        rem[i] -= slice;

        while (nextIdx < n && procs[nextIdx].arrival <= t) {
            rq.push(nextIdx++);
        }

        if (rem[i] > 0) rq.push(i);
        else doneAt[i] = t;
    }

    vector<Result> results;
    for (int i = 0; i < n; i++) {
        int tat = doneAt[i] - procs[i].arrival;
        int wait = max(0, tat - procs[i].burst);
        results.push_back({procs[i].pid, procs[i].burst, procs[i].arrival,
                           procs[i].priority, wait, tat});
    }

    showGantt(tl);
    showTable(results);
}

void Algorithms::priority() {
    showHeader("Priority Scheduling (lower=higher)");

    vector<Process> procs = makeProcesses();
    sort(procs.begin(), procs.end(), [](Process a, Process b) {
        return a.arrival != b.arrival ? a.arrival < b.arrival : a.pid < b.pid;
    });

    vector<Segment> tl;
    vector<Result> results;
    vector<bool> done(procs.size(), false);
    int finished = 0, t = 0;

    while (finished < (int)procs.size()) {
        int pick = -1;
        for (int i = 0; i < (int)procs.size(); i++) {
            if (done[i] || procs[i].arrival > t) continue;
            if (pick == -1 || procs[i].priority < procs[pick].priority ||
               (procs[i].priority == procs[pick].priority && procs[i].pid < procs[pick].pid))
                pick = i;
        }

        if (pick == -1) {
            int nxt = 99999;
            for (int i = 0; i < (int)procs.size(); i++)
                if (!done[i]) nxt = min(nxt, procs[i].arrival);
            segPush(tl, 0, nxt - t);
            t = nxt;
            continue;
        }

        Process p = procs[pick];
        int start = t;
        segPush(tl, p.pid, p.burst);
        t += p.burst;
        done[pick] = true;
        finished++;
        results.push_back({p.pid, p.burst, p.arrival, p.priority,
                           start - p.arrival, t - p.arrival});
    }

    showGantt(tl);
    showTable(results);
}

