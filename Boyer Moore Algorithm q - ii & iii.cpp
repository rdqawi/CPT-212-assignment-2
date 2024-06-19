#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

#define NO_OF_CHARS 256

// Function to generate the bad character heuristic array
void initializelastmap(const string &pattern, int badchar[NO_OF_CHARS]) {
    // Initialize all occurrences of characters as -1
    fill_n(badchar, NO_OF_CHARS, -1);
    // Fill the actual value of the last occurrence of a character in the pattern
    for (int i = 0; i < pattern.size(); i++) {
        badchar[(int)pattern[i]] = i;
    }
}

// Function to search for the pattern in the given text using Boyer-Moore algorithm
void search(const string &text, const string &pattern) {
    int m = pattern.size();
    int n = text.size();

    int badchar[NO_OF_CHARS];
    initializelastmap(pattern, badchar); // Fill the bad character heuristic array

    int shift = 0; // Start position in the text
    while (shift <= (n - m)) {
        int j = m - 1;

        // Keep reducing j while characters of pattern and text are matching
        while (j >= 0 && pattern[j] == text[shift + j]) {
            j--;
        }

        if (j < 0) { // If the pattern is present at the current shift
            cout << "Pattern found at index " << shift << endl;
            // Shift the pattern to align with the next character in text
            shift += (shift + m < n) ? m - badchar[text[shift + m]] : 1;
        } else {
            // Shift the pattern so that the bad character in text aligns with the last occurrence in pattern
            shift += max(1, j - badchar[text[shift + j]]);
        }
    }
}

int main() {
    string text = "ABCDBACDABCCCBABCAABCD";
    string pattern = "ABC";
    
    // Display the text and pattern
    cout << "Text: " << text << endl;
    cout << "Pattern: " << pattern << endl << endl;
    
    search(text, pattern); // Call the search function
    return 0; // Return 0 to indicate successful execution
}

