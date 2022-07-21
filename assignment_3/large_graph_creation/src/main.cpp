#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main() {
    ofstream outFile("graph5.txt");
    double n1, n2, n3, n4;
    int lowest = 1, highest = 10, weight;
    int range = (highest - lowest) + 1;
    string space = " ";
    int nodes = 100;
    for (int i = 1; i <= nodes; i++) {
        for (int j = 1; j <= nodes; j++) {
            if (i != nodes && j != nodes) {
                n1 = (nodes * (i - 1) + j);
                n2 = (nodes * (i - 1) + j + 1);
                n3 = (nodes * i + j);
                n4 = (nodes * i + j + 1);
                weight = lowest + rand() % range;
                outFile << n1 << space << n2 << space << weight << "\n";
                weight = lowest + rand() % range;
                outFile << n1 << space << n3 << space << weight << "\n";
                weight = lowest + rand() % range;
                outFile << n1 << space << n4 << space << weight << "\n";
            }
            else if (i != nodes && j == nodes) {
                n1 = (nodes * (i - 1) + j);
                n3 = (nodes * i + j);
                weight = lowest + rand() % range;
                outFile << n1 << space << n3 << space << weight << "\n";
            } else if (i == nodes && j!= nodes) {
                n1 = (nodes * (i - 1) + j);
                n2 = (nodes * (i - 1) + j + 1);
                weight = lowest + rand() % range;
                outFile << n1 << space << n2 << space << weight;
                if (j != nodes - 1) {
                    outFile << "\n";
                }
            }
        }
    }
    outFile.close();
    return 0;
}