#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <queue>

using namespace std;

void testsort(unsigned int* v, unsigned int n) {
    for (unsigned int i = 0; i < n - 1; i++)
    {
        if (v[i] > v[i + 1]) {
            cout << "SORTARE INCORECTA\n";
            return;
        }
    }
    cout << "SORTARE CORECTA\n";
}

void radixsort(unsigned int* v, unsigned int n, unsigned int maxim) {
    queue<unsigned int> c0, c1;
    unsigned int p = 0, doi, i, j;
    for (doi = 1; doi <= maxim; doi *= 2) {
        for (i = 0; i < n; i++) {
            if ((v[i] >> p) & 1)
                c1.push(v[i]);
            else
                c0.push(v[i]);
        }
        p++;
        j = 0;
        while (!c0.empty()) {
            v[j] = c0.front();
            c0.pop();
            j++;
        }
        while (!c1.empty()) {
            v[j] = c1.front();
            c1.pop();
            j++;
        }
    }
}

void merge(unsigned int* v, unsigned int st, unsigned int dr) {
    unsigned int mij = st + (dr-st) / 2, nst = mij - st + 1, ndr = dr - mij, i, j, k;
    unsigned int* stanga = new unsigned int[nst];
    unsigned int* dreapta = new unsigned int[ndr];
    for (unsigned int i = 0; i < nst; i++)
        stanga[i] = v[st + i];
    for(unsigned int j = 0; j < ndr; j++)
        dreapta[j] = v[mij + 1 + j];
    i = 0;
    j = 0;
    k = st;
    while (i < nst && j < ndr) {
        if (stanga[i] < dreapta[j]) {
            v[k] = stanga[i];
            i++;
        }
        else {
            v[k] = dreapta[j];
            j++;
        }
        k++;
    }
    while (i < nst) {
        v[k] = stanga[i];
        i++;
        k++;
    }
    while (j < ndr) {
        v[k] = dreapta[j];
        j++;
        k++;
    }
}

void mergesort(unsigned int* v, unsigned int st, unsigned int dr) {
    if (st >= dr)
        return;
    unsigned int mij = st + (dr - st) / 2;
    mergesort(v, st, mij);
    mergesort(v, mij + 1, dr);
    merge(v, st, dr);
}

void shellsort(unsigned int* v, unsigned int n) {
    for (unsigned int dist = n / 2; dist > 0; dist /= 2) {
        for (unsigned int i = dist; i < n; i++) {
            unsigned int aux = v[i];
            unsigned int j;
            for (j = i; j >= dist && v[j - dist] > aux; j -= dist)
                v[j] = v[j - dist];
            v[j] = aux;
        }
    }
}

void heapify(unsigned int* v, int n, int i)
{
    int maxim = i;
    int st = 2 * i + 1;
    int dr = 2 * i + 2;

    if (st < n && v[st] > v[maxim])
        maxim = st;

    if (dr < n && v[dr] > v[maxim])
        maxim = dr;

    if (maxim != i) {
        swap(v[i], v[maxim]);
        heapify(v, n, maxim);
    }
}

void heapsort(unsigned int* v, unsigned int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(v, n, i);

    for (int i = n - 1; i > 0; i--) {
        swap(v[0], v[i]);
        heapify(v, i, 0);
    }
}

void countsort(unsigned int* v, unsigned int n, unsigned int maxim) {
    unsigned int* frecv = new unsigned int[maxim + 1];
    for (int i = 0; i < maxim+1; i++) {
        frecv[i]=0;
    }
    for (int i = 0; i < n; i++) {
        frecv[v[i]]++;
    }
    int j = 0;
    for (int i = 0; i < maxim + 1; i++)
        while (frecv[i] > 0) {
            v[j] = i;
            frecv[i]--;
            j++;
        }
    delete[] frecv;
}

int main()
{
    int t;
    ifstream in("teste.txt");
    in >> t;
    for (int i = 0; i < t; i++) {
        cout << "TEST " << i + 1 <<"-------------------------------------------------" << endl;

        unsigned int n, maxim;
        in >> n >> maxim;
        unsigned int* v = new unsigned int[n];
        unsigned int* copie = new unsigned int[n];
        cout << "numar elemente = " << n << endl << "maxim = " << maxim << endl << endl;

        for (unsigned int j = 0; j < n; j++) {
            v[j] = rand() % (maxim + 1);
            copie[j] = v[j];
        }

        auto start = chrono::steady_clock::now();
        sort(v, v+n);
        auto stop = chrono::steady_clock::now();
        cout << "Algoritm nativ de sortare timp de executie: " << chrono::duration_cast<chrono::nanoseconds>(stop - start).count() << "ns" << endl;
        testsort(v, n);
        cout << endl;

        for (unsigned int j = 0; j < n; j++)
            v[j] = copie[j];

        start = chrono::steady_clock::now();
        radixsort(v, n, maxim);
        stop = chrono::steady_clock::now();
        cout << "RadixSort timp de executie: " << chrono::duration_cast<chrono::nanoseconds>(stop - start).count() << "ns" << endl;
        testsort(v, n);
        cout << endl;

        for (unsigned int j = 0; j < n; j++)
            v[j] = copie[j];

        if (n <= 10000000) {
            start = chrono::steady_clock::now();
            mergesort(v, 0, n);
            stop = chrono::steady_clock::now();
            cout << "MergeSort timp de executie: " << chrono::duration_cast<chrono::nanoseconds>(stop - start).count() << "ns" << endl;
            testsort(v, n);
            cout << endl;
        }
        else
            cout << "MergeSort nu poate sorta.\n"<<endl;

        for (unsigned int j = 0; j < n; j++)
            v[j] = copie[j];

        start = chrono::steady_clock::now();
        shellsort(v, n);
        stop = chrono::steady_clock::now();
        cout << "ShellSort timp de executie: " << chrono::duration_cast<chrono::nanoseconds>(stop - start).count() << "ns" << endl;
        testsort(v, n);
        cout << endl;

        for (unsigned int j = 0; j < n; j++)
            v[j] = copie[j];

        start = chrono::steady_clock::now();
        heapsort(v, n);
        stop = chrono::steady_clock::now();
        cout << "HeapSort timp de executie: " << chrono::duration_cast<chrono::nanoseconds>(stop - start).count() << "ns" << endl;
        testsort(v, n);
        cout << endl;

        for (unsigned int j = 0; j < n; j++)
            v[j] = copie[j];

        start = chrono::steady_clock::now();
        countsort(v, n, maxim);
        stop = chrono::steady_clock::now();
        cout << "CountSort timp de executie: " << chrono::duration_cast<chrono::nanoseconds>(stop - start).count() << "ns" << endl;
        testsort(v, n);
        cout << endl;

        delete[] v;
    }
    in.close();
    return 0;
}