#include <iostream>
#include <vector>
#include <random>
#include <stack>
#include <algorithm>
#include <getopt.h>

class Graph {
private:
    int V;
    std::vector<std::vector<int>> adj;

public:
    Graph(int v) : V(v), adj(v, std::vector<int>(v, 0)) {}

    void addEdge(int u, int v) {
        adj[u][v]++;
        adj[v][u]++;
    }

    bool hasEulerCircuit() {
        for (int i = 0; i < V; i++) {
            int degree = 0;
            for (int j = 0; j < V; j++) {
                degree += adj[i][j];
            }
            if (degree % 2 != 0) {
                return false;
            }
        }
        return true;
    }

    std::vector<int> findEulerCircuit() {
        if (!hasEulerCircuit()) {
            return {};
        }

        std::vector<int> circuit;
        std::stack<int> stack;
        int current = 0;  // Start from vertex 0

        stack.push(current);

        while (!stack.empty()) {
            current = stack.top();
            bool found = false;

            for (int i = 0; i < V; i++) {
                if (adj[current][i] > 0) {
                    stack.push(i);
                    adj[current][i]--;
                    adj[i][current]--;
                    found = true;
                    break;
                }
            }

            if (!found) {
                circuit.push_back(current);
                stack.pop();
            }
        }

        std::reverse(circuit.begin(), circuit.end());
        return circuit;
    }

    void generateRandomGraph(int edges, unsigned seed) {
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> dis(0, V - 1);

        for (int i = 0; i < edges; i++) {
            int u = dis(gen);
            int v = dis(gen);
            addEdge(u, v);
        }
    }

    void printGraph() {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                std::cout << adj[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

int main(int argc, char* argv[]) {
    int vertices = 5;
    int edges = 10;
    unsigned seed = std::random_device{}();

    int opt;
    while ((opt = getopt(argc, argv, "v:e:s:")) != -1) {
        switch (opt) {
            case 'v':
                vertices = std::stoi(optarg);
                break;
            case 'e':
                edges = std::stoi(optarg);
                break;
            case 's':
                seed = std::stoul(optarg);
                break;
            default:
                std::cerr << "Usage: " << argv[0] << " [-v vertices] [-e edges] [-s seed]" << std::endl;
                return 1;
        }
    }

    Graph g(vertices);
    g.generateRandomGraph(edges, seed);

    std::cout << "Generated Graph:" << std::endl;
    g.printGraph();

    if (g.hasEulerCircuit()) {
        std::cout << "The graph has an Euler circuit." << std::endl;
        std::vector<int> circuit = g.findEulerCircuit();
        std::cout << "Euler Circuit: ";
        for (int v : circuit) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "The graph does not have an Euler circuit." << std::endl;
    }

    return 0;
}