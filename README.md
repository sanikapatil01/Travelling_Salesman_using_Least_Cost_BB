
# SwiftShip Delivery Route Optimization Using LC Branch and Bound

## 1. Project Overview

SwiftShip is a logistics company that delivers packages across multiple cities. The company needs to determine the least-cost delivery route that visits every city exactly once and returns to the starting city.

This project implements the Travelling Salesman Problem (TSP) using the **Least Cost (LC) Branch and Bound algorithm** in C++.

The program considers the distance between cities, fuel costs that may vary according to road conditions, and travel-time constraints. It searches for the minimum-cost tour that satisfies the specified delivery deadline.

## 2. Aim

To implement the Travelling Salesman Problem using the Least Cost Branch and Bound algorithm to find an optimal delivery route while minimizing transportation costs and satisfying a maximum travel-time constraint.

## 3. Problem Statement

A delivery truck must visit N cities, visit each city exactly once, and return to its starting city.

The cost of travelling between cities varies according to distance and fuel expenses. Additionally, the complete journey must finish within a specified time limit.

A brute-force solution examines all possible city orders, requiring factorial time. This becomes impractical as the number of cities increases.

The proposed solution uses Branch and Bound to explore promising routes and eliminate partial routes that cannot produce a better feasible solution.

## 4. Objectives

- Represent the cities and travel costs using matrices.
- Accept distance, fuel cost and travel-time data from the user.
- Calculate the monetary cost of travelling between each pair of cities.
- Implement the Least Cost Branch and Bound algorithm.
- Use a priority queue to expand the most promising partial route.
- Prune branches that cannot improve the best solution.
- Enforce the maximum delivery-time constraint.
- Display the optimal feasible route, total cost and total travel time.

## 5. Technologies Used

| Technology | Purpose |
|---|---|
| C++ | Implementation language |
| STL vectors | Store distance, cost and time matrices |
| Priority queue | Select the partial route with the lowest cost bound |
| Branch and Bound | Search for an optimal tour while pruning unpromising branches |
| Bitmask | Track visited cities efficiently |
| STL algorithms | Perform minimum-value and related operations |

## 6. Theory

### 6.1 Travelling Salesman Problem

The Travelling Salesman Problem is a combinatorial optimization problem in which a traveller must visit every city exactly once and return to the starting city while minimizing total travel cost.

For N cities, a brute-force algorithm may examine (N - 1)! possible tours when the starting city is fixed.

For example:

- 5 cities: 24 possible tours.
- 10 cities: 362,880 possible tours.
- 20 cities: 121,645,100,408,832,000 possible tours.

These values illustrate why examining every route becomes impractical for larger instances.

### 6.2 Branch and Bound

Branch and Bound is an optimization technique that divides a problem into smaller subproblems and calculates bounds on the best solution each subproblem could produce.

It uses three main operations:

- **Branching:** Generate new partial routes by selecting unvisited cities.
- **Bounding:** Estimate the minimum possible cost of completing a partial route.
- **Pruning:** Discard a partial route if it cannot lead to a better feasible solution.

### 6.3 Least Cost Branch and Bound

In Least Cost Branch and Bound, a priority queue stores partial solutions according to their lower-bound cost.

The algorithm expands the state with the smallest bound first, rather than exploring every route in a fixed depth-first order.

This strategy directs the search toward promising solutions while retaining the ability to find the global optimum.

## 7. Cost and Time Calculations

### Transportation cost

The program calculates the cost of each road as:

Road Cost(i, j) = Distance(i, j) × Fuel Cost Per Kilometre(i, j)

The total tour cost is:

Total Cost = Sum of the costs of all travelled edges, including the return journey.

### Time constraint

The total journey time is the sum of the travel times for every edge in the complete tour.

A tour is feasible only when:

Total Travel Time <= Maximum Allowed Time

The program rejects routes that exceed the deadline and uses an optimistic lower bound on remaining travel time to prune additional infeasible branches.

## 8. Algorithm

1. Start the program.
2. Read the number of cities.
3. Read the distance matrix.
4. Read the fuel cost per kilometre matrix.
5. Calculate the monetary cost matrix.
6. Read the travel-time matrix.
7. Read the maximum permitted delivery time.
8. Create the initial state with City 1 as the starting point.
9. Calculate the initial cost and time lower bounds.
10. Insert the initial state into a minimum-priority queue.
11. Remove the state with the lowest cost bound.
12. If the state cannot improve the best solution or cannot meet the deadline, prune it.
13. Otherwise, generate a new state for each unvisited city.
14. Calculate the new cost, time and lower bounds.
15. Insert promising states into the priority queue.
16. When all cities have been visited, add the return journey to City 1.
17. Update the best solution if the complete tour is feasible and has a lower cost.
18. Continue until the queue becomes empty.
19. Display the optimal feasible route or report that no feasible tour was found.
20. Stop.

## 9. Input Format

The program accepts the following information:

1. Number of cities, N.
2. An N × N distance matrix in kilometres.
3. An N × N fuel cost per kilometre matrix.
4. An N × N travel-time matrix in hours.
5. Maximum allowed delivery time in hours.

City 1 is used as the starting city.

The matrices can represent different conditions for different directions of travel. The current implementation assumes that every pair of distinct cities has a usable road.

## 10. Sample Input

The following example contains four cities.

### Distance matrix

```text
0 10 15 20
10 0 35 25
15 35 0 30
20 25 30 0
```

### Fuel cost per kilometre matrix

```text
0 2 2 2
2 0 2 2
2 2 0 2
2 2 2 0
```

### Travel-time matrix

```text
0 1 1.5 2
1 0 2.5 2
1.5 2.5 0 2
2 2 2 0
```

### Maximum allowed delivery time

```text
8
```

## 11. Sample Output

```text
Searching for the least-cost feasible tour...

========== OPTIMAL DELIVERY TOUR ==========
Route: City 1 -> City 2 -> City 4 -> City 3 -> City 1
Minimum total fuel cost: 160.00
Total delivery time: 6.50
Maximum allowed time: 8.00
Time constraint: Satisfied
```

The route has a total distance of 80 km. Since the fuel cost is 2 monetary units per kilometre for each road, the total transportation cost is 160 monetary units.

The total travel time is 6.5 hours, which satisfies the eight-hour deadline.

## 12. How to Compile and Run

### Prerequisites

- GCC or another C++17-compatible compiler.
- A terminal or command prompt.
- The source file `swiftship.cpp`.

### Compile

```bash
g++ -std=c++17 swiftship.cpp -o swiftship
```

### Run on Windows PowerShell

```powershell
.\swiftship.exe
```

### Run on Linux or macOS

```bash
./swiftship
```

Enter the matrices and time limit when prompted.

## 13. Complexity Analysis

Let N represent the number of cities.

### Time complexity

The brute-force approach examines factorially many tours, approximately O(N!) for a fixed starting city.

Branch and Bound reduces unnecessary exploration by pruning partial routes. However, its worst-case time complexity remains factorial or exponential, depending on how the search is bounded and represented.

In this implementation, computing each lower bound examines possible outgoing edges for the remaining cities and takes O(N²) time. Priority-queue operations add further overhead.

Consequently, the algorithm may still require substantial time for large inputs.

### Space complexity

The distance, fuel-cost, travel-time and monetary-cost matrices each require O(N²) space.

The priority queue may store exponentially many partial states in the worst case. Each state also stores its route, which can require O(N) space.

Therefore, the overall worst-case space complexity is exponential.

## 14. Advantages

- Searches for a globally optimal feasible tour when the search completes.
- Prioritizes promising partial routes.
- Avoids exploring branches that cannot improve the current best solution.
- Supports different road distances and fuel expenses.
- Enforces a maximum travel-time constraint.
- Uses a bitmask to track visited cities efficiently.

## 15. Limitations

- Worst-case computation time remains exponential.
- Large instances may consume considerable memory.
- The implementation assumes a complete road network.
- It does not model vehicle capacity, package priorities, traffic changes during delivery or multiple trucks.
- Fuel expenses are represented through user-provided per-kilometre monetary costs.
- Travel times are treated as fixed values rather than live traffic estimates.

## 16. Applications

- Logistics and package delivery.
- Transportation route planning.
- Vehicle routing research.
- Manufacturing and machine sequencing.
- Warehouse picking routes.
- Planning tours that must visit multiple destinations.

## 17. Future Scope

- Support missing roads and disconnected road networks.
- Integrate real-world maps and traffic information.
- Support multiple delivery trucks and vehicle capacities.
- Add delivery windows and package priority constraints.
- Use stronger lower bounds to improve pruning.
- Compare performance with dynamic programming and heuristic algorithms.
- Create a graphical interface for visualizing delivery routes.

## 18. Conclusion

The SwiftShip project demonstrates the application of the Travelling Salesman Problem and the Least Cost Branch and Bound algorithm to delivery route optimization.

By considering distance-based fuel costs and maximum travel time, the system searches for a minimum-cost feasible tour. The priority queue expands the most promising states first, while lower-bound calculations and pruning reduce unnecessary exploration.

Although Branch and Bound does not eliminate the exponential worst-case complexity of TSP, it provides an exact optimization approach that can be effective for suitably sized instances.

## 19. Project Structure

```text
SwiftShip/
├── swiftship.cpp
└── README.md
```
