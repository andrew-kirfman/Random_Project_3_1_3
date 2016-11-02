/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP6/cpu_bound_processes/cpu_bound_2.cpp                             */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Standard Library Includes                                                   */
/* --------------------------------------------------------------------------- */

#include <iostream>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <stack>
using namespace std;

/* Custom exception to be thrown when a state is found to be unsolvable */
struct UnsolvableState:exception
{

};

/* Custon exception to be thrown when a problem is found to be unsolvable */
struct UnsolvableProblem:exception
{
    
};


/* Class to represent a cryptarithm in memory */
class Cryptarithm
{
public:
    // Program data structures
    map<string, vector<int> > possible_assignments;
    vector<vector<pair<string, int> > > problem;
    string premise_1;
    string premise_2;
    string result;
    int number_found;

    // Member functions
    Cryptarithm();
    Cryptarithm(string pre_1, string pre_2, string res);
    Cryptarithm(const Cryptarithm &obj);
    vector<pair<string, int> > solve_cryptarithm();
    bool goal_test();
    void simplify_problem();
    void check_consistency();
    void assign_symbol(string symbol, int number);
    void remove_number(int number, string exception);
};

/* Find the intersection of two vectors                                   */
/* Reference: Code adapted from stackoverflow and a c++ reference website */
vector<int> intersection(vector<int> &v1, vector<int> &v2)
{	
    vector<int> result;
    set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(result));
    
    return result;
}


/* Find the difference between two vectors                                */
/* Reference: Code adapted from stackoverflow and a c++ reference website */
vector<int> difference(vector<int> &v1, vector<int> &v2)
{	
    vector<int> result;
    set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(result));
    
    return result;
}

/* Global string replace function                                         */
/* Reference: Code adapted from stackoverflow and a c++ reference website */
void ReplaceStringInPlace(string& subject, const string& search, const string& replace) 
{
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != string::npos) 
    {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}


/* Default constructor */
Cryptarithm::Cryptarithm()
{
    
}


/* Primary cryptarithm constructor */
Cryptarithm::Cryptarithm(string pre_1, string pre_2, string res)
{
    // Initialize precedent and result strings
    premise_1 = pre_1;
    premise_2 = pre_2;
    result = res;
    number_found = 0;

    // Tokenize each one and find unique letters
    vector<string> unique_letters;
    string total_string = pre_1 + pre_2 + res;
    for(int i=0; i<total_string.length(); i++)
    {
        bool isUnique = true;
        for(int j=0; j<unique_letters.size(); j++)
        {
            if(total_string.substr(i, 1) == unique_letters[j])
            {
                isUnique = false;
            }
        }
        if(isUnique)
        {
            unique_letters.push_back(total_string.substr(i, 1));
        }
    }
    
    vector<int> possible_vals_template;
    for(int i=0; i<10; i++)
    {
        possible_vals_template.push_back(i);
    }

    for(int i=0; i<unique_letters.size(); i++)
    {
        possible_assignments[unique_letters[i]] = vector<int>(possible_vals_template);
    }
}


/* Cryptarithm copy constructor */
Cryptarithm::Cryptarithm(const Cryptarithm &obj)
{
    possible_assignments = obj.possible_assignments;
    premise_1 = obj.premise_1;
    premise_2 = obj.premise_2;
    result = obj.result;
    number_found = obj.number_found;      
}


/* Search algorithm goal test.  Returns true if the addition problem */
/* with values plugged in makes sense.                               */
bool Cryptarithm::goal_test()
{
    int carry = 0;
    int problem_length = result.size();
    for(int i=problem_length - 1; i>=0; i--)
    {
        int calculation_result = premise_1[i] + premise_2[i] + carry - 96;
        if(calculation_result >= 10)
        {
            carry = 1;
            calculation_result -= 10;
        }
        else
        {
            carry = 0;
        }
        
        if(calculation_result != (result[i] - 48))
        {
            return false;
        }
    }
    return true;
}


/* Checks the problem against known simplifying conditions and limits */
/* the final state space that will be solved                          */
void Cryptarithm::simplify_problem()
{
    // Since there are no leading zeros, they can not be zero
    string symbol = premise_1.substr(0, 1);
    vector<int> temp_assignments = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    possible_assignments[symbol] = 
        intersection(possible_assignments[symbol], temp_assignments);
    
    symbol = premise_2.substr(0, 1);
    possible_assignments[symbol] = 
        intersection(possible_assignments[symbol], temp_assignments);
    
    symbol = result.substr(0, 1);
    possible_assignments[symbol] = 
        intersection(possible_assignments[symbol], temp_assignments);
    
    
    // If the result is one letter longer than the two arguments then it is 
    // a 1 (because of no leading zeros).  
    if(premise_1.length() == premise_2.length()
        && result.length() == premise_1.length() + 1)
    {
        symbol = result.substr(0, 1);
        
        possible_assignments[symbol] = {1};
        assign_symbol(symbol, 1);
        try
        {
            remove_number(1, symbol);
        }
        catch(UnsolvableState())
        {
            throw UnsolvableProblem();
        }
        number_found++;
    }

    // If the first two numbers to add are the same, their result must be
    // even.  
    if(premise_1[premise_1.length() - 1] == premise_2[premise_2.length() - 1])
    {
        symbol = result.substr(result.length() - 1, 1);

        temp_assignments = {0, 2, 4, 6, 8};
        possible_assignments[symbol] = 
            intersection(possible_assignments[symbol], temp_assignments);
    }

    // If the two premises have the same ending characters and the result
    // is also the same length, that character < 5.  
    if(premise_1[0] == premise_2[0] 
        && premise_1.length() == premise_2.length()
        && premise_1.length() == result.length())
    {
        symbol = premise_1.substr(0, 1);
       
        temp_assignments = {1, 2, 3, 4};
        possible_assignments[symbol] = 
            intersection(possible_assignments[symbol], temp_assignments);
    }

    // If the same character is in one of the premise strings and in the result, 
    // the other character is either 0 or 1 (the result of a carry) but only if
    // it is not the first set of characters.  
    if(premise_1[premise_1.length() - 1] == result[result.length() - 1])
    {
        string symbol = premise_2.substr(premise_2.length() - 1, 1);

        // Since it is the first symbol, there isn't a carry
        possible_assignments[symbol] = {0};
        assign_symbol(symbol, 0);
        try
        {
            remove_number(0, symbol);
        }
        catch(UnsolvableState())
        {
            throw UnsolvableProblem();
        }
        number_found++;
    }
    else if(premise_2[premise_2.length() - 1] == result[result.length() - 1])
    {
        symbol = premise_1.substr(premise_1.length() - 1, 1);

        // Since it is the first symbol, there isn't a carry
        possible_assignments[symbol] = {0};
        assign_symbol(symbol, 0);
        try
        {
            remove_number(0, symbol);
        }
        catch(UnsolvableState())
        {
            throw UnsolvableProblem();
        }
        number_found++;
    }

    // If this same thing occurs later in the problem, that means that it could
    // be either 0 or 1 because of possible carrys.  
    int min_length = (premise_1.length() > premise_2.length()) 
        ? premise_2.length() : premise_1.length();   
    int pos_1 = premise_1.length() - 2;
    int pos_2 = premise_2.length() - 2;
    int pos_r = result.length() - 2;
    for(int i=0; i<min_length - 2; i++)
    {
        if(premise_1[pos_1] == result[pos_r])
        {
            symbol = premise_2.substr(pos_2, 1);

            temp_assignments = {0, 1};
            possible_assignments[symbol] = 
                intersection(possible_assignments[symbol], temp_assignments);
        }
        else if(premise_2[pos_2] == result[pos_r])
        {
            symbol = premise_1.substr(pos_1, 1);

            temp_assignments = {0, 1};
            temp_assignments = {0, 1};
            possible_assignments[symbol] = 
                intersection(possible_assignments[symbol], temp_assignments);
        }
        pos_1--;
        pos_2--;
        pos_r--;
    }
    
    // If the premises have the same ending characters and same length and the
    // result is one longer, then they must be >= 5.  
    if(premise_1.length() == premise_2.length()
        && premise_1.length() + 1 == result.length()
        && premise_1[0] == premise_2[0])
    {
        string symbol = premise_1.substr(0, 1);
        
        vector<int> temp_assignments = {5, 6, 7, 8, 9};
        possible_assignments[symbol] = 
            intersection(possible_assignments[symbol], temp_assignments);
    }
    
    // Make all operands in the problem matrix the same length (add trailing zeros)
    int max_length = (premise_1.length() > premise_2.length()) 
        ? premise_1.length() : premise_2.length(); 
    if(result.length() > max_length)
    {
        max_length = result.length();
    }
    
    for(int i=premise_1.length(); i<max_length; i++)
    {
        premise_1 = "0" + premise_1;
    }
    
    for(int i=premise_2.length(); i<max_length; i++)
    {
        premise_2 = "0" + premise_2;
    } 
}


/* Called as a result of assigning a number to a letter.  As a result */
/* the other leters can not have the same number                      */
void Cryptarithm::remove_number(int number, string exception)
{
    for(auto& iterator : possible_assignments)
    {
        if(iterator.first != exception)
        {
            vector<int> temp = {number};
            iterator.second = difference(iterator.second, temp);
        }
    }
}


/* Replaces strings with their new found numbers */
void Cryptarithm::assign_symbol(string symbol, int number)
{
    ReplaceStringInPlace(premise_1, symbol, to_string(number));
    ReplaceStringInPlace(premise_2, symbol, to_string(number));
    ReplaceStringInPlace(result, symbol, to_string(number));
}


/* Checks to ensure that the numbers that have been deduced so far make */
/* sense.  If they don't, the search algorithm shouldn't continue to    */
/* expand that tree of states.                                          */
void Cryptarithm::check_consistency()
{
    int carry = 0;
    int problem_length = result.length();
    for(int i=problem_length - 1; i>=0; i--)
    {   
        if(premise_1[i] > 57 
        || premise_2[i] > 57
            || result[i] > 57)
        {
            return;
        }
        
        int calculation_result = premise_1[i] + premise_2[i] + carry - 96;
        if(calculation_result >= 10)
        {
            carry = 1;
            calculation_result -= 10;
        }
        else
        {
            carry = 0;
        }
        if(calculation_result != (result[i] - 48))
        {
            throw UnsolvableState();
        }
    }
    return;
}


/* DFS algorithm to solve the given crpytarithmetic problem */
map<string, vector<int> > solve_cryptarithm(Cryptarithm starting_state)
{   
    // Save original strings for printout later
    string o_premise_1 = starting_state.premise_1;
    string o_premise_2 = starting_state.premise_2;
    string o_result = starting_state.result;

    // Constrain the problem space
    starting_state.simplify_problem();
    
    stack<Cryptarithm> problem_states;
    problem_states.push(starting_state);
    int states_expanded = 0;
    unordered_map<string, bool> visited_states;
    visited_states[starting_state.premise_1 
        + starting_state.premise_2 + starting_state.result] = true;
    
    // Perform breadth first search to find the answer
    while(!problem_states.empty())
    {
        Cryptarithm current_state = problem_states.top();
        problem_states.pop();
        
        // Goal test
        if(current_state.number_found == current_state.possible_assignments.size())
        {
            // If goal then print everything nicely and then exit
            if(current_state.goal_test())
            {
                return current_state.possible_assignments;
            }
        }
        
        // If not at goal, generate new states
        for(auto& iterator : current_state.possible_assignments)
        {
            if(iterator.second.size() > 1)
            {
                for(int i=0; i<iterator.second.size(); i++)
                {
                    Cryptarithm temp_state(current_state);
                    temp_state.possible_assignments[iterator.first] = {iterator.second[i]};
                    temp_state.assign_symbol(iterator.first, iterator.second[i]);
                    temp_state.number_found += 1;
                    try
                    {
                        temp_state.remove_number(iterator.second[i], iterator.first);
                        temp_state.check_consistency();
                    }
                    catch(UnsolvableState)
                    {
                        continue;
                    }
                    bool exists = false;
                    if(visited_states.find(temp_state.premise_1 + temp_state.premise_2
                        + temp_state.result) == visited_states.end())
                    {
                        visited_states[temp_state.premise_1 + temp_state.premise_2
                            + temp_state.result] = true;
                        problem_states.push(temp_state);
                        states_expanded++;
                    }
                }
            }
        }
    }
    throw UnsolvableProblem();
}


int solve_cpt()
{
    string premise_1 = "JAVA";
    string premise_2 = "OOP";
    string result = "LANG";
    map<string, vector<int> > possible_solution;
    
    // Initialize the cryptarithm object
    Cryptarithm Cryptarithmetic_Problem = Cryptarithm(premise_1, premise_2, result);
    try
    {
        possible_solution = solve_cryptarithm(Cryptarithmetic_Problem);
    }
    catch(UnsolvableProblem)
    {
        cerr << "Input problem is unsolvable!" << endl;
        return 1;
    }

    return 0;
}


int main()
{
    kill(getpid(), SIGSTOP);

    std::cout << "STARTING CRYPTARITHM SOLVER (5)" << std::endl;
    solve_cpt();

    exit(0);
}




