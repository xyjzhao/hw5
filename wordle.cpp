#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// Add prototypes of helper functions here
void wordle_helper(
    std::string& current,
    const std::string& in,
    std::string remaining_floating,
    const std::set<std::string>& dict,
    std::set<std::string>& results,
    size_t pos);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    std::set<std::string> results;
    
    // Handle edge cases
    if (in.empty()) {
        return results;
    }
    
    // Initialize the current word with the input pattern
    std::string current = in;
    
    // Start the recursive process
    wordle_helper(current, in, floating, dict, results, 0);
    
    return results;
}

// Define any helper functions here
void wordle_helper(
    std::string& current,
    const std::string& in,
    std::string remaining_floating,
    const std::set<std::string>& dict,
    std::set<std::string>& results,
    size_t pos)
{
    // Base case: we've reached the end of the word
    if (pos == in.length()) {
        // Check if we've used all the floating letters
        if (remaining_floating.empty() && dict.find(current) != dict.end()) {
            results.insert(current);
        }
        return;
    }
    
    // If this position already has a fixed letter, move to the next position
    if (in[pos] != '-') {
        current[pos] = in[pos];
        wordle_helper(current, in, remaining_floating, dict, results, pos + 1);
        return;
    }
    
    // Count remaining blank positions (including the current one)
    size_t blanks_remaining = 0;
    for (size_t i = pos; i < in.length(); i++) {
        if (in[i] == '-') {
            blanks_remaining++;
        }
    }
    
    // First try using floating letters
    for (size_t i = 0; i < remaining_floating.length(); i++) {
        char c = remaining_floating[i];
        current[pos] = c;
        
        // Remove this floating letter from the remaining set
        string new_remaining = remaining_floating;
        new_remaining.erase(i, 1);
        
        wordle_helper(current, in, new_remaining, dict, results, pos + 1);
    }
    
    // Only try other letters if we have enough blank positions left for all remaining floating letters
    if (blanks_remaining > remaining_floating.length()) {
        // Try all other letters (excluding those in floating set)
        for (char c = 'a'; c <= 'z'; c++) {
            // Skip if this letter is in the remaining floating set
            if (remaining_floating.find(c) != string::npos) {
                continue;
            }
            
            current[pos] = c;
            wordle_helper(current, in, remaining_floating, dict, results, pos + 1);
        }
    }
}