#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#define NO_OF_CHARS 256

using namespace std;

// Function to create the bad character heuristic table
void preprocess_bad_char(const string &pattern, int badchar[NO_OF_CHARS]) {
    // Initialize all positions to -1
    fill_n(badchar, NO_OF_CHARS, -1);

    // Record the last occurrence of each character in the pattern
    for (int i = 0; i < pattern.size(); i++) {
        badchar[(int)pattern[i]] = i;
    }
}

// Function to preprocess the pattern and create the good suffix table
void preprocess_good_suffix(const string &pattern, vector<int> &suffix, vector<int> &good_suffix) {
    int m = pattern.size();
    suffix.resize(m);
    good_suffix.resize(m);
    
    // Initialize the suffix array
    suffix[m - 1] = m; // The entire pattern is a suffix of itself.
    int f = 0, g = m - 1; // f is the index for matching, g is to tracks the end of the matching suffix.

    for (int i = m - 2; i >= 0; i--) { // Start from the second last character to the first character.
        if (i > g && suffix[i + m - 1 - f] < i - g) {
            // If i is beyond g (no overlap with the suffix matched by f)
            // and suffix[i + m - 1 - f] is smaller than the distance between i and g,
            // we can safely copy the value from suffix[i + m - 1 - f].
            suffix[i] = suffix[i + m - 1 - f];
        } else {
            // If i is within g or suffix[i + m - 1 - f] is greater than or equal to i - g,
            // we need to calculate suffix[i] from scratch.
            if (i < g) {
                g = i; // Update g to the current position i.
            }
            f = i; // Set f to the current position i.
            // Decrement g as long as the characters in the pattern match.
            while (g >= 0 && pattern[g] == pattern[g + m - 1 - f]) {
                g--; // Decrement g to extend the matching suffix backwards.
            }
            suffix[i] = f - g; // The length of the matching suffix is f - g.
        }
    }

    // Initialize the good suffix array to the length of the pattern
    fill(good_suffix.begin(), good_suffix.end(), m);

    // Fill the good suffix array based on the suffix array
    for (int i = m - 1, j = 0; i >= 0; i--) {
        if (suffix[i] == i + 1) {
            // If suffix[i] == i + 1, it means the suffix pattern[0...i] is a prefix of the pattern
            // Update all positions from j to m-1-i in the good suffix array
            for (; j < m - 1 - i; j++) {
                if (good_suffix[j] == m) {
                    good_suffix[j] = m - 1 - i;
                }
            }
        }
    }

    // Fill the remaining entries of the good suffix array
    for (int i = 0; i <= m - 2; i++) {
        good_suffix[m - 1 - suffix[i]] = m - 1 - i;
    }
}

// Function to search for the pattern in the given text using the Boyer-Moore algorithm
void search(const string &text, const string &pattern) {
    int m = pattern.size();
    int n = text.size();

    // Arrays for the heuristics
    int badchar[NO_OF_CHARS];
    vector<int> suffix, good_suffix;

    // Initialize the bad character heuristic
    preprocess_bad_char(pattern, badchar);
    // Preprocess the good suffix heuristic
    preprocess_good_suffix(pattern, suffix, good_suffix);

    int shift = 0; // Start position in the text
    while (shift <= (n - m)) {
        int j = m - 1;

        // Compare pattern from right to left with the current window in text
        while (j >= 0 && pattern[j] == text[shift + j]) {
            j--;
        }

        // If the pattern is found
        if (j < 0) {
            cout << "Pattern found at index " << shift << endl;
            // Use the good suffix heuristic to decide the next shift
            shift += good_suffix[0];
        } else {
            // Use the maximum of the bad character and good suffix heuristics
            shift += max(good_suffix[j], j - badchar[(int)text[shift + j]]);
        }
    }
}

int main() {
    string text = "012001020030001020102";
    string pattern = "010";
    
    // Display the text and pattern
    cout << "Text: " << text << endl;
    cout << "Pattern: " << pattern << endl << endl;
    
    // Call the search function
    search(text, pattern);
    return 0; // Return 0 to indicate successful execution
}

