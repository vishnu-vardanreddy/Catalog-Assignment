#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

using json = nlohmann::json;

class Root {
  public:
	int x;
	long long y;
	int base;

	Root(int x, long long y, int base) : x(x), y(y), base(base) {}
};

long long baseToDecimal(const std::string &num, int base) { return std::stoll(num, nullptr, base); }

Root parseRoot(const std::string &key, const json &rootObj) {
	int base = std::stoi(rootObj["base"].get<std::string>());
	std::string value = rootObj["value"].get<std::string>();

	long long y = baseToDecimal(value, base);
	int x = std::stoi(key);

	return Root(x, y, base);
}

double lagrangeInterpolation(const std::vector<Root> &roots, double x) {
	double result = 0.0;

	for (size_t i = 0; i < roots.size(); i++) {
		double term = roots[i].y;

		for (size_t j = 0; j < roots.size(); j++) {
			if (i != j) {
				term *= (x - roots[j].x) / (roots[i].x - roots[j].x);
			}
		}

		result += term;
	}

	return result;
}

int main() {
	// Load JSON data from file
	std::ifstream file("input.json");
	if (!file.is_open()) {
		std::cerr << "Could not open the file!" << std::endl;
		return 1;
	}

	json inputRoots;
	file >> inputRoots; // Read JSON data
	file.close();

	std::vector<Root> roots;

	try {
		// Retrieve n and k from the keys
		int n = inputRoots["keys"]["n"].get<int>();
		int k = inputRoots["keys"]["k"].get<int>();

		std::cout << "n: " << n << ", k: " << k << std::endl;

		// Parse each root
		for (auto &[key, rootObj] : inputRoots.items()) {
			if (key != "keys") { // Skip the keys section
				roots.push_back(parseRoot(key, rootObj));
			}
		}

		std::cout << "Parsed roots:" << std::endl;
		for (const Root &root : roots) {
			std::cout << "x: " << root.x << ", y: " << root.y << " (base " << root.base << ")"
					  << std::endl;
		}

		// The constant term is the value of the polynomial at x = 0
		double constantTerm = lagrangeInterpolation(roots, 0);

		std::cout << "The constant term (c) of the polynomial is: " << constantTerm << std::endl;
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return 0;
}
