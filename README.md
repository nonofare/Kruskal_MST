# Kruskal's Algorithm Implementation in C++

This project provides a simple and clean C++ implementation of **Kruskal's Algorithm**, a classic greedy algorithm used
to find the **Minimum Spanning Tree (MST)** of a connected, undirected, and weighted graph. The code demonstrates key
graph processing techniques such as edge sorting and the Union-Find (Disjoint Set Union) data structure.

## Table of Contents

- [Features](#features)
- [How It Works](#how-it-works)
- [Example Usage](#example-usage)
- [Installation](#installation)
- [Contributing](#contributing)
- [License](#license)

## Features

- **Greedy MST Construction:** Uses Kruskal's algorithm to generate a minimum spanning tree.
- **Disjoint Set Union (Union-Find):** Efficient data structure for cycle detection.
- **Simple Input Format:** Edges can be provided with vertices and weights in plain text.
- **Verbose Output:** Displays step-by-step edge selection and MST result.

## How It Works

1. **Sort Edges:** All edges are sorted in non-decreasing order of weight.
2. **Initialize Sets:** Each vertex is placed in its own set.
3. **Edge Selection:** Iterate through the sorted edges, and for each edge:
    - If it connects two different sets, add it to the MST.
    - Merge the sets (union operation).
4. **Result:** The MST is built from the selected edges.

## Example Usage

```cpp
#include "kruskal.h"

int main() {
    int V = 4; // Number of vertices
    std::vector<Edge> edges = {
        {0, 1, 10},
        {0, 2, 6},
        {0, 3, 5},
        {1, 3, 15},
        {2, 3, 4}
    };

    KruskalMST(V, edges);
    return 0;
}
```

## Installation

1. **Clone the repository:**
   ```bash
   git clone https://github.com/nonofare/Kruskal_algorithm.git
   ```

2. **Navigate to the project directory:**
   ```bash
   cd Kruskal_algorithm
   ```

3. **Compile the project:**

   If you have a `main.cpp` file with your own graph input and want to use the provided implementation:

   ```bash
   g++ main.cpp kruskal.cpp -o kruskal_mst
   ```

   Or, to test the included example:

   ```bash
   g++ example_main.cpp kruskal.cpp -o kruskal_mst
   ```

4. **Run the executable:**
   ```bash
   ./kruskal_mst
   ```

   You should see the edges selected for the Minimum Spanning Tree along with their weights.

## Contributing

Contributions are welcome! If you spot any bugs or have ideas for enhancements, feel free to open an issue or submit a
pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.
