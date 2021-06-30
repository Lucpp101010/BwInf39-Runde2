#include <bits/stdc++.h>
using namespace std;

int u, n; // Umfang des Sees, Anzahl der Häuser
vector<int> house; // Liste der Häuser
// maxSize[s][i][j]:
// Maximaler Abstand der Eisbude bei Haus i von einer nach Haus i+n/3-5+1
// sodass genau s Stimmen mehr geholt werden als die Hälfte (ceil) der Anzahl der Häuser zwischen den Eisbudem
vector<vector<int>> maxSize[2];

// Vorberechnung von maxSize
void precalcMaxSize()
{
	// Iteriere über die mx' bzw. Delta-Werte
	for(int s = 0; s < 2; ++s)
	{
		maxSize[s].assign(n, vector<int>(7));

		// Iteriere über die relevanten Anzahlen an Häusern zwischen zwei Eisbuden
        for(int d = 0; d < 7; ++d)
        {
			// Anzahl an Häusern zwischen zwei Eisbuden
            int sum = n/3 + (d-5);
			if(sum < 0) continue; // Die Anzahl an Häusern kann nicht negativ sein
			// mx' aus der Doku
			// Stimmenzahl mit einer Eisbude, die nicht überschritten werden darf
            int wanted = (sum + 1) / 2 + s;
			// Falls die Anzahl der Häuser nicht größer als mx' ist,
			// wird mx' sicher nie überschritten -> size beliebig groß
            if(wanted >= sum)
            {
                for(int i = 0; i < n; ++i) maxSize[s][i][d] = u;
                continue;
            }

			// deque für Monotonie-Trick
            deque<pair<int, int>> q;
			// Index vom rechten Haus. Es werden alle Häuser in (i, j] betrachtet
            int j = sum;
			// Initialisieren der deque
            for(int i = 1; i + wanted <= j; ++i)
            {
				// Abstand der Häuser mit Indexdifferenz mx'
                int val = (house[(i+wanted) % n] - house[i] + u) % u;
				// Entferne das letzte Element der deque, solange es nicht kleiner val ist
                while(!q.empty() && val <= q.back().first) q.pop_back();
                q.emplace_back(val, i);
			}
			// G steht vorne in der deque
			// Es gilt: size = 2 * G + 1
            maxSize[s][0][d] = 2*q.front().first+1;

            for(int i = 1; i < n; ++i)
            {
                j = i + sum;
                int val = (house[j%n] - house[(j-wanted+n)%n] + u) % u;
				// Entferne das vordere Element, falls es aus links aus dem Fenster fällt
                if(q.front().second <= i) q.pop_front();
                while(!q.empty() && val <= q.back().first) q.pop_back();
                q.emplace_back(val, j-wanted);

                maxSize[s][i][d] = 2*q.front().first+1;
            }
        }
	}
}


int main()
{
	// INPUT
	cin >> u >> n;
	house.resize(n);
	for(int i = 0; i < n; ++i)
	{
		cin >> house[i];
	}
    // Es wird aufgrund der BWINF-Beispiele erwartet, dass die Häuser sortiert sind.
	// In diesem Fall kann die O(n) Laufzeit gesichert werden. Damit das Programm aber
	// auch mit anderen Fällen klar kommt, werden die Daten sonst sortiert in O(n log(n))
	if(!is_sorted(house.begin(), house.end()))
    	sort(house.begin(), house.end());

	precalcMaxSize();

	// Erste Eisbude
    for(int i = 0; i < n; ++i)
    {
		// Anzahl an Häusern zwischen der ersten und zweiten Eidbude (+ n/3)
        for(int x = -min(5, n/3); x <= 1; ++x)
        {
			// Anzahl an Häusern zwischen der zweiten und dritten Eidbude (+ n/3)
            for(int y = -min(5, n/3); y <= 1; ++y)
            {
				// Befindet sich Eisbude 2 bei einem Haus (h2 = 1)
				// Oder VOR dem angegebenen Haus (h2 = 0)
				for(int h2 = 0; h2 < 2; ++h2) 
				{
					// Wie h2, nur für Eisbude 3
					for(int h3 = 0; h3 < 2; ++h3)
					{
						// k soll i nicht "überrunden"
						if(2 * n/3 + x + y + 1 + h2 >= n) continue;

						// Bestimmen der Eisbudenpositionen j, k

						// Es befinden sich n/3 + x Häuser zwischen i und j
						// i steht bei einem Haus -> 1 addieren
						int j = (i + n/3 + x + 1) % n;
						// Falls j bei einem Haus steht muss 1 addiert werden
						int k = (j + n/3 + y + h2) % n;

						// Abstand zwischen Eisbuden k und i
						// Wie davor gilt: i = (k + n/3 + z + h3)
						// Umstellen ergibt z
						int z = (i-k+n)%n - n/3 - h3;
						if(z < -5 || z > 1) continue;
						
						// sum Werte für Testen der Stabilität
						int s[3];
						s[0] = x + n/3;
						s[1] = y + n/3;
						s[2] = z + n/3;
						// Delta / mx' Werte
						for(int p1 = 0; p1 < 2; ++p1)
						{
							for(int p2 = 0; p2 < 2 - p1; ++p2)
							{
								for(int p3 = 0; p3 < 2 - p2 - p1; ++p3)
								{
									// mx Werte für Stabilitätstest
									int m[3];
									m[0] = (s[0] + 1) / 2 + p1;
									m[1] = (s[1] + 1) / 2 + p2;
									m[2] = (s[2] + 1) / 2 + p3;

									// maximale Abstände zwischen den Eisbuden / size Werte
									int a = maxSize[p1][i][x+5];
									int b = maxSize[p2][(j-1+h2+n)%n][y+5];
									int c = maxSize[p3][(k-1+h3+n)%n][z+5];

									// Überprüfe ob stabil
									int best = m[0] + m[1] + m[2]; // Je eine Eisbude pro Intervall
									for(int g = 0; g < 3; ++g)
									{
										for(int h = 0; h < 3; ++h)
										{
											// Zwei Eisbuden in einem Intervall und eine in einem anderen
											if(g != h) best = max(best, s[g] + m[h]);
										}
									}
									// Nicht stabil!
									if(best * 2 > n) continue;

									// Bestimme die genauen Eisbudenpositionen e1, e2, e3
									int e1 = house[i];
									int e2, e3;
									if(h2) e2 = house[j % n];
									else e2 = (e1 + min(a, (house[j] - e1 - 1 + u) % u)) % u;
									if(h3) e3 = house[k % n];
									else e3 = (e2 + min(b, (house[k] - e2 - 1 + u) % u)) % u;

									// Die Eisbuden werden so platziert, dass sie nicht nach dem Haus kommen,
									// bei dem sie sein sollten. Nun wird überprüft, ob sie nach dem vorherigen
									// Haus kommen -> liegen sie zwischen den richtigen Häusern?
									if((house[(j-1+n)%n] - e1 + u) % u >= (e2 - e1 + u) % u) continue;
									if((house[(k-1+n)%n] - e2 + u) % u >= (e3 - e2 + u) % u) continue;

									// Finale Überprüfung, ob die Abstände der Eisbuden klein genug sind
									if((e2 - e1 + u) % u > a) continue;
									if((e3 - e2 + u) % u > b) continue;
									if((e1 - e3 + u) % u > c) continue;

									// Es wurde eine Lösung gefunden!
									// Sortiere die Positionen und gib sie aus!
									vector<int> ans = {e1, e2, e3};
									sort(ans.begin(), ans.end());
									cout << "Stabile Verteilung gefunden!\n";
									cout << ans[0] << " " << ans[1] << " " << ans[2] << "\n";
									exit(0);
								}
							}
						}

					}
				}
            }
        }
    }
	cout << "Es wurde keine stabile Verteilung gefunden!\n";
}
