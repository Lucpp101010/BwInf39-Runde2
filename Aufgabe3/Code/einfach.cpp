#include <bits/stdc++.h>
using namespace std;

int u, n; // Umfang des Sees, Anzahl der Häuser
vector<int> house; // Liste der Häuser
vector<int> votes; // Stimmenanzahl bei Index; 1, wenn sich dort ein Haus befindet, sonst 0
vector<int> vpref; // Präfixsumme von votes

// Bestimmt in O(1) die Anzahl an Häusern / Stimmen Zwischen den Häusern bei l und r
// Die Grenzen sind einschließlich
int getVotes(int l, int r)
{
	if(r < l) return getVotes(0, r) + getVotes(l, u-1);
	if(l > 0) return vpref[r] - vpref[l-1];
	return vpref[r];
}

// maxInterval[i][j]:
// Maximale Anzahl an Stimmen, die bei einem Vorschlag mit einer Eisbude zwischen i und j
// geholt werden kann, wenn sich bei i und j eine Eisbude befindet.
vector<vector<int>> maxInterval;

// Addition modulo u
int add(int a, int b)
{
	return (a + b) % u;
}

// Subtraktion modulo u
int sub(int a, int b)
{
	return (a - b + u) % u;
}

// Vorberechnung von maxInterval
void precalcMaxIntervals()
{
	maxInterval.assign(u, vector<int>(u));
	// Iteriere über alle Paare von Eisbudenpositionen
	for(int i = 0; i < u; ++i)
	{
		for(int j = 0; j < u; ++j)
		{
			int up = j;
			if(j < i) up += u;
			int m = up - i; // Distanz von i nach j
			// g aus der Dokumentation:
			// Größe des Intervalls, in dem die Bewohner für den Vorschlag stimmen
			int len = m / 2 - 1;
			int ans = 0;
			if(len >= 0)
			{
				// Finde von allen Intervallsummen die Größte
				for(int k = i+1; k + len < up; k++)
				{
					ans = max(ans, getVotes(k % u, (k+len) % u));
				}
			}
			maxInterval[i][j] = ans;
		}
	}
}

// Eisbuden bei Positionen i, j, k
// Berechnet die maximale Stimmenanzahl, die ein Vorschlag erhalten kann
int maxVotes(int i, int j, int k)
{
	// Anzahl an Stimmen, die mit 2 Eisbuden geholt werden können
	// (= Anzahl an Häusern zwischen den Eisbuden)
	int s[3];
	s[0] = getVotes(add(i, 1), sub(j, 1));
	s[1] = getVotes(add(j, 1), sub(k, 1));
	s[2] = getVotes(add(k, 1), sub(i, 1));
	// Anzahl an Stimmen, die mit einer Eisbude geholt werden können
	int m[3];
	m[0] = maxInterval[i][j];
	m[1] = maxInterval[j][k];
	m[2] = maxInterval[k][i];
	// Teste alle Möglichkeiten, die 3 Eisbuden auf die Intervalle zu verteilen
	// (geht nur, weil es so wenige sind!)
	int best = m[0] + m[1] + m[2]; // Je eine Eisbude pro Intervall
	for(int g = 0; g < 3; ++g)
	{
		for(int h = 0; h < 3; ++h)
		{
			// Zwei Eisbuden in einem Intervall und eine in einem anderen
			if(g != h) best = max(best, s[g] + m[h]);
		}
	}
	return best;
}

int main()
{
	// INPUT
	cin >> u >> n;
	house.resize(n);
	votes.resize(u);
	vpref.resize(u+1);
	for(int i = 0; i < n; ++i)
	{
		cin >> house[i];
		votes[house[i]] = 1;
	}

	// Vorberechnung Präfixsumme
	vpref[0] = votes[0];
	for(int i = 1; i < u; ++i)
	{
		vpref[i] = vpref[i-1] + votes[i];
	}
	// Vorberchnung von maxIntervals
    precalcMaxIntervals();

	// Iteriere über alle Eisbudenverteilungen
	for(int i = 0; i < u; ++i)
	{
		for(int j = i+1; j < u; ++j)
		{
			for(int k = j+1; k < u; ++k)
			{
				// Berechne die maxiamle Anzahl an Stimmen eines Vorschlages
				int best = maxVotes(i, j, k);
				// Stabile Verteilung gefunden, falls keine Mehrheit erreicht werden kann
				if(best * 2 <= n)
				{
					cout << "Stabile Verteilung gefunden!\n";
					cout << i << " " << j << " " << k << "\n";
					exit(0);
				}
			}
		}
	}

	cout << "Es wurde keine stabile Verteilung gefunden!\n";
}
