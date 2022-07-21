#include <iostream>
#include <iterator>
#include <utility>
#include <list>
#include <map>
#include <bits/stdc++.h>
#include <algorithm>

using namespace std;

int main() {

    auto t1 = std::chrono::high_resolution_clock::now();
    
    map<double, double> closed_list;

    priority_queue<vector<double>, vector<vector<double>>, greater<vector<double>>> open_list;
   
    // get graph from txt
    string line;
    ifstream input_file("graph2.txt");
    map<double, vector<vector<double>>> small_graph;
    while (getline(input_file, line)) {
        vector<string> words;
        string word = "";
        line = line + " ";
        for (int i = 0; i < line.length(); i++) {
            if (line[i] != ' ') {
                word = word + line[i];
            }
            else {
                words.push_back(word);
                word = "";
            }
        }
        map<double, vector<vector<double>>>::iterator position = small_graph.find(stod(words[0]));
        if (position != small_graph.end()) {
            position->second.push_back({stod(words[1]), stod(words[2])});
        }
        else{
            small_graph.insert({stod(words[0]), {{stod(words[1]), stod(words[2])}}});
        }
    }
    
    double csfs[21];
    fill_n(&csfs[0], 21, INT_MAX);

    csfs[0] = 0;
    csfs[1] = 0;

    //start algo
    //open_list.insert({0, {1, 0}});
    open_list.push({0, 1, 0});
    while (open_list.size() > 0) {
        vector<double> top = open_list.top();
        open_list.pop();
        closed_list.insert({top[1], top[2]});
        //cout << "inserted " << top[1] << " " << top[2] << endl;
        double csf_parent = top[0];
        double curr_node = top[1];
        //cout << "current node " << curr_node << endl;
        auto children = small_graph.find(top[1]);
        if (top[1] == 20)
            break;
        if (children == small_graph.end())
            continue;
        vector<vector<double>> children_list = children->second;
        for (vector<double> child : children_list) {
            int curr_child = (int)child[0];
            double child_csf = child[1] + csf_parent;
            // in open_list
            //cout << "parent " << curr_node << "and child " << curr_child << endl;
            if (closed_list.find(curr_child) != closed_list.end()) {
                //cout << "continued at " << curr_child << endl;
                continue;
            }
            if (csfs[curr_child] != INT_MAX) {
                // compare
                //cout << "here for " << curr_child << " " << child_csf << endl;
                priority_queue<vector<double>, vector<vector<double>>, greater<vector<double>>> second_open_list;
                while (open_list.size() > 0) {
                    vector<double> second_top = open_list.top();
                    if ((int)second_top[1] == curr_child) {
                        if (csfs[curr_child] > child_csf) {
                            csfs[curr_child] = child_csf;
                            open_list.pop();
                            //cout << "POPED " << second_top[1] << " " << second_top[2] << endl;
                            second_open_list.push({child_csf, second_top[1], curr_node});
                            //cout << "Here" << endl;
                        }
                        else {
                            open_list.pop();
                            second_open_list.push(second_top);
                        }
                    }
                    else {
                        //cout << "PUSHED " << second_top[1] << " " << second_top[2] << endl;

                        second_open_list.push(second_top);
                        open_list.pop();
                    }
                }
                swap(open_list, second_open_list);
            }
            // not in open_list
            else {
                //cout << "not there for " << curr_child << " " << child_csf << endl;
                open_list.push({child_csf ,(double)curr_child, curr_node});
                csfs[curr_child] = child_csf;
            } 
        }
    }
    /*
    while (open_list.size() > 0) {
        vector<double> ans = open_list.top();
        cout << ans[0] << " " << ans[1] << " " << ans[2] << endl;
        open_list.pop();
    }
    */
    /*
    for (auto itr = closed_list.begin(); itr != closed_list.end(); itr++) {
            cout << itr->first << " " << itr->second << endl;
    }
    */
    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms_double = t2 - t1;

    cout<< "OL and CL sizes " << open_list.size() << " " << closed_list.size() << endl;

    cout << "Time elapsed: " << ms_double.count() << "ms" << endl;
    return 0;
}