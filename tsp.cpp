
#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <limits>
#include <cstdint>
#include <algorithm>

using namespace std;

const double INF = numeric_limits<double>::infinity();
const double EPS = 1e-9;

int N;

vector<vector<double>> distanceMatrix;
vector<vector<double>> fuelCostPerKm;
vector<vector<double>> travelTime;
vector<vector<double>> costMatrix;

double maxAllowedTime;
double bestCost = INF;
double bestTime = 0.0;

vector<int> bestRoute;

struct State {
    int currentCity;
    uint64_t visited;
    vector<int> path;

    double costSoFar;
    double timeSoFar;

    double lowerBound;
    double timeLowerBound;
};

// The priority queue expands the state with the lowest cost bound first.
struct CompareState {
    bool operator()(const State& a, const State& b) const {
        if (a.lowerBound != b.lowerBound) {
            return a.lowerBound > b.lowerBound;
        }

        return a.timeLowerBound > b.timeLowerBound;
    }
};

priority_queue<State, vector<State>, CompareState> pq;

// Check whether city i has already been visited.
bool isVisited(uint64_t mask, int i) {
    return (mask & (1ULL << i)) != 0;
}

// Calculate a lower bound for the total cost of completing a route.
//
// Every city that still needs to depart (the current city and all
// unvisited cities) must have an outgoing edge in the completed tour.
//
// We add the minimum possible outgoing cost for each such city.
// This is an optimistic lower bound, not necessarily a feasible route.
double calculateCostBound(
    int current,
    uint64_t visited,
    double currentCost
) {
    double bound = currentCost;

    for (int u = 0; u < N; u++) {
        bool mustDepart =
            (u == current) || !isVisited(visited, u);

        if (!mustDepart) {
            continue;
        }

        double minimumOutgoing = INF;

        for (int v = 0; v < N; v++) {
            if (u == v) {
                continue;
            }

            // A remaining city may travel to an unvisited city
            // or return to the starting city (city 0).
            bool allowedTarget =
                (v == 0) || !isVisited(visited, v);

            if (allowedTarget) {
                minimumOutgoing = min(
                    minimumOutgoing,
                    costMatrix[u][v]
                );
            }
        }

        if (minimumOutgoing == INF) {
            return INF;
        }

        bound += minimumOutgoing;
    }

    return bound;
}

// Calculate an optimistic lower bound on the remaining travel time.
double calculateTimeBound(
    int current,
    uint64_t visited,
    double currentTime
) {
    double bound = currentTime;

    for (int u = 0; u < N; u++) {
        bool mustDepart =
            (u == current) || !isVisited(visited, u);

        if (!mustDepart) {
            continue;
        }

        double minimumOutgoingTime = INF;

        for (int v = 0; v < N; v++) {
            if (u == v) {
                continue;
            }

            bool allowedTarget =
                (v == 0) || !isVisited(visited, v);

            if (allowedTarget) {
                minimumOutgoingTime = min(
                    minimumOutgoingTime,
                    travelTime[u][v]
                );
            }
        }

        if (minimumOutgoingTime == INF) {
            return INF;
        }

        bound += minimumOutgoingTime;
    }

    return bound;
}

// Print the best route found.
void displaySolution() {
    if (bestCost == INF) {
        cout << "\nNo feasible delivery tour exists within "
             << "the given time limit.\n";
        return;
    }

    cout << fixed << setprecision(2);

    cout << "\n-------- OPTIMAL DELIVERY TOUR --------- \n";
    cout<<endl;

    cout << "Route: ";

    for (int city : bestRoute) {
        cout << "City " << city + 1 << " -> ";
    }

    cout << "City " << bestRoute[0] + 1 << '\n';
    cout<<endl;
    cout << "Minimum total fuel cost: " << bestCost << '\n';
    cout<<endl;
    cout << "Total delivery time: " << bestTime << '\n';
    cout<<endl;
    cout << "Maximum allowed time: " << maxAllowedTime << '\n';
    cout<<endl;

    cout << "Time constraint: Satisfied\n";
}

// LC Branch and Bound search.
void solveTSP() {
    uint64_t startMask = 1ULL; // City 1 is the starting city.

    State initial;

    initial.currentCity = 0;
    initial.visited = startMask;
    initial.path = {0};
    initial.costSoFar = 0.0;
    initial.timeSoFar = 0.0;

    initial.lowerBound = calculateCostBound(
        0, startMask, 0.0
    );

    initial.timeLowerBound = calculateTimeBound(
        0, startMask, 0.0
    );

    if (initial.timeLowerBound > maxAllowedTime + EPS) {
        displaySolution();
        return;
    }

    pq.push(initial);

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        // Prune routes that cannot improve the best solution.
        if (current.lowerBound >= bestCost - EPS) {
            continue;
        }

        // Prune states whose optimistic time bound exceeds the limit.
        if (current.timeLowerBound > maxAllowedTime + EPS) {
            continue;
        }

        // All cities have been visited. Return to the starting city.
        if (current.path.size() == static_cast<size_t>(N)) {
            int lastCity = current.currentCity;

            double returnCost = costMatrix[lastCity][0];
            double returnTime = travelTime[lastCity][0];

            double totalCost = current.costSoFar + returnCost;
            double totalTime = current.timeSoFar + returnTime;

            if (totalTime <= maxAllowedTime + EPS &&
                totalCost < bestCost - EPS) {

                bestCost = totalCost;
                bestTime = totalTime;
                bestRoute = current.path;
            }

            continue;
        }

        // Branch: try every city that has not been visited.
        for (int nextCity = 0; nextCity < N; nextCity++) {
            if (isVisited(current.visited, nextCity)) {
                continue;
            }

            double newCost =
                current.costSoFar +
                costMatrix[current.currentCity][nextCity];

            double newTime =
                current.timeSoFar +
                travelTime[current.currentCity][nextCity];

            // Immediate time-feasibility pruning.
            if (newTime > maxAllowedTime + EPS) {
                continue;
            }

            uint64_t newVisited =
                current.visited | (1ULL << nextCity);

            double newCostBound = calculateCostBound(
                nextCity, newVisited, newCost
            );

            // No need to explore a route that cannot improve the best.
            if (newCostBound >= bestCost - EPS) {
                continue;
            }

            double newTimeBound = calculateTimeBound(
                nextCity, newVisited, newTime
            );

            if (newTimeBound > maxAllowedTime + EPS) {
                continue;
            }

            State next;

            next.currentCity = nextCity;
            next.visited = newVisited;
            next.path = current.path;
            next.path.push_back(nextCity);

            next.costSoFar = newCost;
            next.timeSoFar = newTime;
            next.lowerBound = newCostBound;
            next.timeLowerBound = newTimeBound;

            pq.push(next);
        }
    }
}

int main() {
   

    cout << "Enter number of cities (2 to 63): ";
    cin >> N;

    if (N < 2 || N > 63) {
        cout << "Invalid number of cities. Enter 2 to 63.\n";
        return 1;
    }

    distanceMatrix.assign(N, vector<double>(N, 0.0));
    fuelCostPerKm.assign(N, vector<double>(N, 0.0));
    travelTime.assign(N, vector<double>(N, 0.0));
    costMatrix.assign(N, vector<double>(N, 0.0));

    cout << "\nEnter the DISTANCE matrix (" << N << " x " << N
         << ") in kilometres.\n";
    cout << "Enter 0 on the diagonal.\n";
    cout<<endl;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> distanceMatrix[i][j];

            if (distanceMatrix[i][j] < 0 ||
                (i == j && distanceMatrix[i][j] != 0)) {
                cout << "Invalid distance matrix.\n";
                return 1;
            }
        }
    }

    cout << "\nEnter the FUEL COST PER KM matrix.\n";
    cout << "Values can differ by road due to road conditions.\n";
    cout << "Enter 0 on the diagonal.\n";
    cout<<endl;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> fuelCostPerKm[i][j];

            if (fuelCostPerKm[i][j] < 0 ||
                (i == j && fuelCostPerKm[i][j] != 0)) {
                cout << "Invalid fuel cost matrix.\n";
                return 1;
            }

            // Monetary cost = distance x fuel cost per kilometre.
            costMatrix[i][j] =
                distanceMatrix[i][j] * fuelCostPerKm[i][j];
        }
    }

    cout << "\nEnter the TRAVEL TIME matrix (" << N << " x "
         << N << ") in hours.\n";
    cout << "Include expected delays for road conditions.\n";
    cout << "Enter 0 on the diagonal.\n";
    cout<<endl;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> travelTime[i][j];

            if (travelTime[i][j] < 0 ||
                (i == j && travelTime[i][j] != 0)) {
                cout << "Invalid travel time matrix.\n";
                return 1;
            }
        }
    }

    cout << "\nEnter maximum allowed delivery time (hours): ";
    cin >> maxAllowedTime;

    if (maxAllowedTime < 0) {
        cout << "Time limit cannot be negative.\n";
        return 1;
    }

   

    solveTSP();
    displaySolution();

    return 0;
}
