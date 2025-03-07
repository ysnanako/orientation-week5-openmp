#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

#define iteration 5
#define vector_size 1000*1000
#define vector_order 1000
#define thread_number 4

void ReadFile(vector<float> &a, vector<float> &b)
{
    ifstream fin;
    float temp;
    fin.open("A.txt");
    if(!fin)
    {
        cerr << "ERROR: input A.txt failed. \n";
        exit(1);
    }
    while(fin >> temp)
        a.push_back(temp);    
    fin.close();

    fin.open("B.txt");
    if(!fin)
    {
        cerr << "ERROR: input B.txt failed. \n";
        exit(1);
    }
    while(fin >> temp)
        b.push_back(temp);    
    fin.close();
}
void WriteFile(const vector<float> &a_openmp, const vector<float> &b_openmp)
{
    ofstream fout;

    fout.open("A_openmp.txt");
    if(!fout)
    {
        cerr << "ERROR: output A_openmp.txt failed. \n";
        exit(1);
    }
    for(size_t i = 0; i < a_openmp.size(); ++i)
        fout << a_openmp[i] << " ";
    fout.close();

    fout.open("B_openmp.txt");
    if(!fout)
    {
        cerr << "ERROR: output B_openmp.txt failed. \n";
        exit(1);
    }
    for(size_t i = 0; i < b_openmp.size(); ++i)
        fout << b_openmp[i] << " ";
    fout.close();
}
void OPENMP_Cal(vector<float> &a_openmp, vector<float> &b_openmp, int N)
{
    vector<float> c_openmp(vector_size);
    vector<float> d_openmp(vector_size);
    omp_set_num_threads(thread_number);
    // CD //
#pragma omp parallel for default(none) shared(a_openmp, b_openmp, c_openmp, d_openmp)
    for(int i = 0; i < vector_size; ++i)
    {
        c_openmp[i] = a_openmp[i] + b_openmp[i];
        d_openmp[i] = a_openmp[i] - b_openmp[i];
    }
    // A //
#pragma omp parallel for default(none) shared(a_openmp, c_openmp, d_openmp) collapse(2)
    for(int i = 0; i < vector_size; ++i)
    {
        for(int j = 0; j < vector_order; ++j)
        {
            int row = i / vector_order;
            int column = i % vector_order;
            a_openmp[i] += c_openmp[row * vector_order + j] * d_openmp[j * vector_order + column];
        }
    }
    // B //
#pragma omp parallel for default(none) shared(a_openmp, b_openmp)
    for(int i = 0; i < vector_size; ++i)
    {
        b_openmp[i] = a_openmp[i] + b_openmp[i];
    }
}
int main()
{
    auto start = chrono::high_resolution_clock::now();

    vector<float> a, b, a_openmp, b_openmp;
    ReadFile(a, b);
    a_openmp = a;
    b_openmp = b;
    for(int i = 0; i < iteration; ++i)
        OPENMP_Cal(a_openmp, b_openmp, vector_order);
    WriteFile(a_openmp, b_openmp);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "thread number: " << thread_number << "\n";
    cout << "elapsed time: " << elapsed.count() << " seconds\n";
    return 0;
}