#include <bits/stdc++.h>
#define INPUT_FILE_NUM 6
#define THREAD_COUNT 5
using namespace std;
pthread_mutex_t mutexLock; //   pthread_mutex_lock( & mutexLock);  pthread_mutex_unlock( & mutexLock);



class Solution
{
private:
    /* data */
public:
    Solution(/* args */){

    };
};
const string FILE_NAMES[6] = {"a.txt", "b.txt", "c.txt", "d.txt", "e.txt", "f.txt"};
void readFile(string fileName) {
    ifstream cin(fileName);
    int a;
    cin >> a;
}


void writeFile(Solution solution, string fileName, string folderName = "solution/") {
    ofstream fout(folderName + "/sol_" + fileName);
    fout << 0 << endl;
//    printf("writeFile Done : %s\n", fileName.c_str());
}















void solve(string fileName) { 
    readFile(fileName);
    writeFile(Solution(),fileName);

    cout << fileName << " Done." << endl;
}

int main(int argc, char** argv) {
    if (argc == 1) { // NO ARGUMENTS, RUN ALL
        vector<thread> threads;        
        for (int i = 0; i < INPUT_FILE_NUM; i++)
            threads.push_back(thread(solve,FILE_NAMES[i]));
        for (auto &th : threads)
            th.join();
    } else {
        int c = argv[1][0] >= 'a' ? argv[1][0] - 'a' :  ((argv[1][0] < 'A') ? argv[1][0] - '1' : argv[1][0] - 'A');
        solve(FILE_NAMES[c]);    
    }
}