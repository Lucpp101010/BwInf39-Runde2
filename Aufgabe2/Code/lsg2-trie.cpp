#include <bits/stdc++.h>
using namespace std;

#define sz(x) ((int)x.size())

// m : Anzahl der Sorten   n : Anzahl der Beobachtungen
int m, n;

int cnt_donald;
vector<bool> donald;
// Schüssel / Früchte aus Beobachtung i
vector<vector<bool>> nums, fruits; 
// Beobachtungen, in denen die Frucht bzw. Schüssel vorkam
vector<vector<bool>> A, B;

// ========================================================
// ================ Datenstrukturen =======================
// ========================================================

// Trie Knoten
struct Node
{
    int next[2] = {-1, -1}; // Die Indices der Kindknoten
    // Falls der Knoten ein Blatt ist:
    int good = 0;
    int bad = 0;
    // Schüsseln, die diese Personenmenge haben
    vector<int> group;
};
// Enthält die Knoten des Tries
vector<Node> nodes;

// Falls die args im Trie ist:
//      Gibt den Wert des zugehörigen Blattes zurück
// sonst:
//      Fügt args zum Trie hinzu und gibt den Wert des neu erstellten Blattes zurück
Node& getNode(const vector<bool>& args)
{
    int cur = 0;
    for(int i = 0; i < sz(args); ++i)
    {
        if(nodes[cur].next[args[i]] != -1)
        {
            cur = nodes[cur].next[args[i]];
        }
        else
        {
            nodes[cur].next[args[i]] = sz(nodes);
            cur = sz(nodes);
            nodes.push_back(Node());
        }
    }
    return nodes[cur];
}

// Knoten für den String Trie. Wird zum Komprimieren der Strings verwendet.
struct StrNode
{
    int next[128] = {};
    int id = -1;
};
// Enthält die Knoten des String Tries
vector<StrNode> strNodes;
// Counter für die Id der komprimierten Fruchtnamen
int strId = 0;
// vergleiche getNode. Gibt die Id zurück
int getFruitIndex(const string& fruit)
{
    int cur = 0;
    for(char c : fruit)
    {
        int nxt = (int)c;
        if(strNodes[cur].next[nxt] == 0)
        {
            strNodes[cur].next[nxt] = sz(strNodes);
            strNodes.push_back(StrNode());
        }
        cur = strNodes[cur].next[nxt];
    }
    if(strNodes[cur].id == -1)
    {
        strNodes[cur].id = strId++;
    }
    return strNodes[cur].id;
}

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
	vector<int> vec(sz(fruitstr));
	for(int i = 0; i < sz(fruitstr); ++i)
	{
		vec[i] = getFruitIndex(fruitstr[i]);
	}
	return vec;
}

// Gibt einen BitVekor zurück, der die Bits an den Indices gesetzt hat, die den int Werten entsprechen.
vector<bool> toBools(const vector<int>& ints)
{
    vector<bool> res(m, false);
    for(int i : ints)
    {
        res[i] = true;
    }
    return res;
}

// Liest eine Zeile mit Fruchtnamen ein und bringt die Menge in das Bitstring Format
vector<bool> readFruits()
{
	return toBools(compress(readLine<string>()));
}
// Liest eine Zeile mit Schüsseln ein und bringt die Menge in das Bitstring Format
vector<bool> readNumbers()
{
	vector<int> ints = readLine<int>();
	for(int i = 0; i < sz(ints); ++i) ints[i]--; // Input ist 1-indiziert
	return toBools(ints);
}


// ========================================================
// ===================== Lösung ===========================
// ========================================================


void solve()
{
    // Iteriere über alle Früchte
	for(int i = 0; i < m; ++i)
    {
        // Will Donald diese Frucht?
        bool ans = donald[i];
        // Erhöhe die entsprechende Anzahl der gewünschten oder nicht gewünschten Sorten im Trie Knoten
        Node& node = getNode(A[i]);
        if(ans) node.good++;
        else node.bad++;
    }

    // Schüsseln, die garantiert zu Donalds Wunschsorten gehören oder auf keinen Fall
    vector<int> take, dont;
    // Iteriere über alle Schüsseln
    for(int i = 0; i < m; ++i)
    {
        Node& node = getNode(B[i]);
        // Falls keine der Früchte mit derselben Personenmenge eine Wunschsorte ist,
        // sollte Donald auf keinen Fall zu dieser Schüssel gehen
        if(node.good == 0) dont.push_back(i);
        // Falls nur Wunschsorten unter diesen waren, sollte Donald sie auf jeden Fall nehmen
        else if(node.bad == 0) take.push_back(i);
        // Der Fall, in dem nur manche Früchte gewünscht wurden, wird später behandelt
        // Dazu wird die Information benötigt, welche Schüsseln in diese Kategorie fallen
        else node.group.push_back(i);
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
            Node& node = getNode(B[i]);
            if(sz(node.group))
            {
                cout << "\t" << node.good << "\t\t\t\t | ";
                for(int j : node.group)
                {
                    cout << j + 1 << " ";
                }
                cout << "\n";
                // Lösche die Daten, damit die Mengen nicht mehrfach ausgegeben werden
                node.group.clear();
            }
        }
    }
}



int main()
{
    // Die Tries benötigen einen Wurzelknoten!
    nodes.push_back(Node());
    strNodes.push_back(StrNode());

    // Lies den Input ein
	m = readLine<int>()[0];
	donald = readFruits();
    cnt_donald = 0;
    for(bool b : donald) cnt_donald += b;
	n = readLine<int>()[0];
	nums.resize(n); fruits.resize(n);
	for(int i = 0; i < n; ++i)
	{
		nums[i] = readNumbers();
		fruits[i] = readFruits();
	}
    // Konstruiere in O(nm) die Personenmengen A_i und B_j
    A.resize(m); B.resize(m);
    for(int i = 0; i < m; ++i)
    {
        A[i].resize(n); B[i].resize(n);
        for(int j = 0; j < n; ++j)
        {
            A[i][j] = fruits[j][i];
            B[i][j] = nums[j][i];
        }
    }

	solve();
}
