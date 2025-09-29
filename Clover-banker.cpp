#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

using namespace std;

// Tính ma trận Need = Max - Allocation
vector<vector<int>> calcNeed(const vector<vector<int>>& Max,
    const vector<vector<int>>& Allocation) {
    if (Max.empty()) return {};
    int n = (int)Max.size();
    int m = (int)Max[0].size();
    vector<vector<int>> Need(n, vector<int>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            Need[i][j] = Max[i][j] - Allocation[i][j];
    return Need;
}

// Kiểm tra an toàn (Banker's safety algorithm)
bool isSafe(const vector<int>& Available,
    const vector<vector<int>>& Max,
    const vector<vector<int>>& Allocation) {
    if (Max.empty()) return true;
    int n = (int)Max.size();
    int m = (int)Available.size();
    vector<vector<int>> Need = calcNeed(Max, Allocation);
    vector<int> Work = Available;
    vector<char> Finish(n, 0);
    vector<int> safeSeq;

    while (true) {
        bool found = false;
        for (int i = 0; i < n; ++i) {
            if (!Finish[i]) {
                bool canExec = true;
                for (int j = 0; j < m; ++j) {
                    if (Need[i][j] > Work[j]) { canExec = false; break; }
                }
                if (canExec) {
                    for (int j = 0; j < m; ++j) Work[j] += Allocation[i][j];
                    Finish[i] = 1;
                    safeSeq.push_back(i);
                    found = true;
                }
            }
        }
        if (!found) break;
    }

    bool safe = true;
    for (int i = 0; i < n; ++i) if (!Finish[i]) { safe = false; break; }

    if (safe) {
        cout << "He thong AN TOAN. Safe sequence: ";
        for (int p : safeSeq) cout << "P" << p << " ";
        cout << "\n";
    }
    else {
        cout << "He thong KHONG AN TOAN!\n";
    }
    return safe;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Thử mở input.txt (working directory là thư mục project hoặc Debug tuỳ cấu hình)
    ifstream fin("input.txt");
    istream* pin = &cin;
    if (fin) {
        pin = &fin;
        cout << "Doc du lieu tu input.txt\n";
    }
    else {
        cout << "Khong tim thay input.txt -> se nhap tu ban phim\n";
    }

    int n, m;
    if (!(*pin >> n >> m)) {
        cerr << "Loi: khong doc duoc n,m\n";
        return 1;
    }

    vector<vector<int>> Max(n, vector<int>(m));
    vector<vector<int>> Allocation(n, vector<int>(m));
    vector<int> Available(m);

    // Doc MAX
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            (*pin) >> Max[i][j];

    // Doc ALLOCATION
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            (*pin) >> Allocation[i][j];

    // Doc AVAILABLE
    for (int j = 0; j < m; ++j)
        (*pin) >> Available[j];

    // In ma tran ra console
    cout << "\nMa tran MAX:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) cout << Max[i][j] << " ";
        cout << "\n";
    }
    cout << "\nMa tran ALLOCATION:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) cout << Allocation[i][j] << " ";
        cout << "\n";
    }
    cout << "\nAVAILABLE:\n";
    for (int j = 0; j < m; ++j) cout << Available[j] << " ";
    cout << "\n";

    // Tinh NEED va in
    vector<vector<int>> Need = calcNeed(Max, Allocation);
    cout << "\nNEED:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) cout << Need[i][j] << " ";
        cout << "\n";
    }
    cout << "\n";

    // Kiem tra an toan hien tai
    isSafe(Available, Max, Allocation);

    // Xu ly mot yeu cau (Request)
    int pid;
    cout << "\nNhap pid (0.." << n - 1 << ") muon yeu cau (hoac nhap -1 de thoat): ";
    if (!(*pin >> pid)) { cerr << "Loi doc pid\n"; return 0; }
    if (pid >= 0 && pid < n) {
        vector<int> Request(m);
        cout << "Nhap Request (" << m << " so):\n";
        for (int j = 0; j < m; ++j) (*pin) >> Request[j];

        bool basic_ok = true;
        for (int j = 0; j < m; ++j) {
            if (Request[j] > Need[pid][j] || Request[j] > Available[j]) { basic_ok = false; break; }
        }
        if (!basic_ok) {
            cout << "Yeu cau KHONG hop le (lon hon Need hoac Available).\n";
        }
        else {
            // cap tam thoi
            for (int j = 0; j < m; ++j) {
                Available[j] -= Request[j];
                Allocation[pid][j] += Request[j];
                Need[pid][j] -= Request[j];
            }
            cout << "Da cap tam thoi, dang kiem tra an toan...\n";
            if (isSafe(Available, Max, Allocation)) {
                cout << "Yeu cau duoc chap nhan va cap phat.\n";
            }
            else {
                // rollback
                for (int j = 0; j < m; ++j) {
                    Available[j] += Request[j];
                    Allocation[pid][j] -= Request[j];
                    Need[pid][j] += Request[j];
                }
                cout << "Yeu cau bi tu choi vi lam he thong KHONG AN TOAN. Da rollback.\n";
            }
        }
    }
    else {
        cout << "Khong xu ly Request (pid = " << pid << ").\n";
    }

    return 0;
}
