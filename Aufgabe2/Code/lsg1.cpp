#include <bits/stdc++.h>
using namespace std;

#define sz(x) ((int)x.size())

// m : Anzahl der Sorten   n : Anzahl der Beobachtungen
int m, n;
map<string, int> f2i;
int cnt_donald;
set<int> donald;
// Beobachtungen, in denen die Frucht bzw. Schüssel vorkam
vector<vector<int>> A, B;

// ========================================================
// ===================== Input ============================
// ========================================================

// Util Funktion für den Input
// Liest eine ganze Zeile und trennt den Input nach Leerzeichen
// T muss dazu kompatibel mit dem std::stringstream sein!
template<typename T>
vector<T> readLine()
{
	string line;
	getline(cin, line);
	stringstream ss(line);
	vector<T> vec;
	T val;
	while(ss >> val) vec.push_back(val);
	return vec;
}

// Komprimiert einen Vector mit Fruchtnamen in einen, der ihre Indizes enthält
vector<int> compress(const vector<string>& fruitstr)
{
	vector<int> vec;
	for(int i = 0; i < sz(fruitstr); ++i)
	{
        if(!f2i.count(fruitstr[i])) f2i.emplace(fruitstr[i], sz(f2i));
        vec.push_back(f2i[fruitstr[i]]);
	}
	return vec;
}

// Liest eine Zeile mit Fruchtnamen ein und bringt die Menge in das Bitstring Format
vector<int> readFruits()
{
	return compress(readLine<string>());
}
// Liest eine Zeile mit Schüsseln ein und bringt die Menge in das Bitstring Format
vector<int> readNumbers()
{
	vector<int> ints = readLine<int>();
	for(int i = 0; i < sz(ints); ++i) ints[i]--; // Input ist 1-indiziert
	return ints;
}


// ========================================================
// ===================== Lösung ===========================
// ========================================================

struct Data 
{
    int good = 0;
    int bad = 0;
    vector<int> group;
};

void solve()
{
    // Iteriere über alle Früchte
    map<vector<int>, Data> f;
	for(int i = 0; i < m; ++i)
    {
        // Will Donald diese Frucht?
        bool ans = donald.count(i);
        Data& d = f[A[i]];
        if(ans) d.good++;
        else d.bad++;
    }

    // Schüsseln, die garantiert zu Donalds Wunschsorten gehören oder auf keinen Fall
    vector<int> take, dont;
    // Iteriere über alle Schüsseln
    for(int i = 0; i < m; ++i)
    {
        Data& d = f[B[i]];
        if(d.good == 0) dont.push_back(i);
        // Falls nur Wunschsorten unter diesen waren, sollte Donald sie auf jeden Fall nehmen
        else if(d.bad == 0) take.push_back(i);
        // Der Fall, in dem nur manche Früchte gewünscht wurden, wird später behandelt
        // Dazu wird die Information benötigt, welche Schüsseln in diese Kategorie fallen
        else d.group.push_back(i);
    }

    // ============= Output ================

    // Falls alle Wunschsorten eindeutig gefunden werden konnten, gib diese aus
    if(sz(take) == cnt_donald)
    {
        cout << "Antwort gefunden: \n";
        for(int i : take)
        {
            cout << i+1 << " ";
        }
        cout << "\n";
    }
    // Mache sonst die "Informative Ausgabe"
    else
    {
        cout << "Die Angaben reichen leider nicht aus!\n\n";
        cout << "Gehe zu diesen Schüsseln:\n";
        for(int i : take)
        {
            cout << i+1 << " ";
        }
        cout << "\nGehe aber nicht zu diesen Schüsseln:\n";
        for(int i : dont)
        {
            cout << i+1 << " ";
        }
        cout << "\nDie folgenden Schüsseln enthalten ein paar der Wunschsorten:\n";
        cout << "Anzahl Wunschsorten\t | unter diesen Schüsseln\n";
        // Gehe erneut alle Schüsseln durch
        for(int i = 0; i < m; ++i)
        {
            Data& d = f[B[i]];
            if(sz(d.group))
            {
                cout << "\t" << d.good << "\t\t\t\t | ";
                for(int j : d.group)
                {
                    cout << j + 1 << " ";
                }
                cout << "\n";
                // Lösche die Daten, damit die Mengen nicht mehrfach ausgegeben werden
                d.group.clear();
            }
        }
    }
}



int main()
{
    // Lies den Input ein
	m = readLine<int>()[0];
	vector<int> __d =  readFruits();
    donald = set<int>(__d.begin(), __d.end());
    cnt_donald = sz(donald);
	n = readLine<int>()[0];
    A.resize(m); B.resize(m);
	for(int i = 0; i < n; ++i)
	{
        vector<int> nums, fruits;
		nums = readNumbers();
		fruits = readFruits();
        for(int j : fruits) A[j].push_back(i);
        for(int j : nums)   B[j].push_back(i);
	}

	solve();
}
