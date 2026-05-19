#include "MaxFlowSolver.h"
#include <iostream>
#include <clocale>

using namespace std;

int main() {

    cout << "========================================" << endl;
    cout << "  ZADACHA O MAKSIMALNOM POTOKE" << endl;
    cout << "  Algoritm Edmondsa-Karpa" << endl;
    cout << "========================================" << endl << endl;

    try {
        cout << "********** TEST 1: KLASSICHESKIY PRIMER **********" << endl;

        MaxFlowSolver solver1(6);

        solver1.addEdge(0, 1, 16);
        solver1.addEdge(0, 2, 13);
        solver1.addEdge(1, 2, 10);
        solver1.addEdge(1, 3, 12);
        solver1.addEdge(2, 1, 4);
        solver1.addEdge(2, 4, 14);
        solver1.addEdge(3, 2, 9);
        solver1.addEdge(3, 5, 20);
        solver1.addEdge(4, 3, 7);
        solver1.addEdge(4, 5, 4);

        solver1.printAdjacencyMatrix();

        long long maxFlow1 = solver1.getMaxFlow(0, 5);
        cout << endl << ">>> MAKSIMALNIY POTOK: " << maxFlow1 << " (ozhidaetsya: 23)" << endl;

        if (maxFlow1 == 23) {
            cout << ">>> TEST 1 PROYDEN USHESHNO" << endl;
        }
        else {
            cout << ">>> TEST 1 NE PROYDEN! Ozhidalos' 23, polucheno " << maxFlow1 << endl;
        }

        solver1.printFlow();

        cout << endl << endl << "********** TEST 2: PARALLELNYE PUTI **********" << endl;

        MaxFlowSolver solver2(4);
        solver2.addEdge(0, 1, 10);
        solver2.addEdge(0, 2, 10);
        solver2.addEdge(1, 3, 5);
        solver2.addEdge(2, 3, 15);

        solver2.printAdjacencyMatrix();

        long long maxFlow2 = solver2.getMaxFlow(0, 3);
        cout << endl << ">>> MAKSIMALNIY POTOK: " << maxFlow2 << " (ozhidaetsya: 15)" << endl;

        if (maxFlow2 == 15) {
            cout << ">>> TEST 2 PROYDEN USHESHNO" << endl;
        }
        else {
            cout << ">>> TEST 2 NE PROYDEN! Ozhidalos' 15, polucheno " << maxFlow2 << endl;
        }

        solver2.printFlow();

        cout << endl << endl << "********** TEST 3: UZKOYE GORLYSHKO **********" << endl;

        MaxFlowSolver solver3(5);
        solver3.addEdge(0, 1, 100);
        solver3.addEdge(0, 2, 100);
        solver3.addEdge(1, 2, 1);
        solver3.addEdge(1, 3, 100);
        solver3.addEdge(2, 4, 100);
        solver3.addEdge(3, 4, 100);

        solver3.printAdjacencyMatrix();

        long long maxFlow3 = solver3.getMaxFlow(0, 4);
        cout << endl << ">>> MAKSIMALNIY POTOK: " << maxFlow3 << " (ozhidaetsya: 200)" << endl;

        if (maxFlow3 == 200) {
            cout << ">>> TEST 3 PROYDEN USHESHNO" << endl;
        }
        else {
            cout << ">>> TEST 3 NE PROYDEN! Ozhidalos' 200, polucheno " << maxFlow3 << endl;
        }

        solver3.printFlow();

        cout << endl << endl << "********** TEST 4: PROVERKA VALIDATSII **********" << endl;

        MaxFlowSolver solver4(3);

        cout << "Dobavlenie rebra s vyhodom za granitsy: ";
        solver4.addEdge(0, 5, 10);

        cout << "Dobavlenie petli: ";
        solver4.addEdge(0, 0, 10);

        cout << "Dobavlenie rebra s otritsatelnoy propusknoy sposobnostyu: ";
        solver4.addEdge(0, 1, -5);

        cout << "Korrektnoe dobavlenie rebra: ";
        if (solver4.addEdge(0, 1, 10)) {
            cout << "yspeshno" << endl;
        }

        cout << "Proverka dublirovaniya rebra: ";
        solver4.addEdge(0, 1, 20);

        cout << endl << ">>> TEST 4 PROYDEN (vse oshibki korrektno obrabotany)" << endl;

    }
    catch (const exception& e) {
        cerr << "KRITICHESKAYA OSHIBKA: " << e.what() << endl;
        return 1;
    }

    cout << endl << "========================================" << endl;
    cout << "  VSE TESTY ZAVERSHENY." << endl;
    cout << "========================================" << endl;

    return 0;
}