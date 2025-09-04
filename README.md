# Olympics Tournament Management System

## Overview
This project implements a comprehensive Olympics tournament management system . The system manages teams, players, matches, and tournaments using advanced data structures including AVL trees and hash tables.

## Features
- **Team Management**: Add/remove teams and players
- **Match System**: Play matches between teams with automatic winner determination
- **Tournament System**: Organize tournaments with power-based seeding
- **Ranking System**: Track team rankings and wins
- **Team Unification**: Merge teams together

## Data Structures Used
- **AVL Tree**: For maintaining tournament rankings and player strength trees
- **Hash Table**: For efficient team lookup and management
- **Linked List**: For maintaining player order within teams

## Project Structure
```
├── src/                    # Source code files
│   ├── olympics24a2.h     # Main Olympics class header
│   ├── olympics24a2.cpp   # Main Olympics class implementation
│   ├── Team.h             # Team class definition
│   ├── Team.cpp           # Team class implementation
│   ├── AVLTree.h          # AVL Tree implementation
│   ├── HashTable.h        # Hash Table implementation
│   ├── Node.h             # Node structure definitions
│   ├── Contestant.h       # Contestant/Player class
│   ├── wet2util.h         # Utility types and status codes
│   └── main24a2.cpp       # Main program entry point
├── tests/                 # Test files and input/output
├── docs/                  # Documentation files
├── build/                 # Build artifacts
└── CMakeLists.txt         # Build configuration
```

## Building and Running

### Prerequisites
- CMake 3.20 or higher
- C++11 compatible compiler (GCC, Clang, or MSVC)

### Build Instructions
```bash
mkdir build
cd build
cmake ..
make
```

### Running the Program
```bash
./wet2 < input_file.txt
```

## API Reference

### Core Operations
- `add_team(teamId)` - Add a new team
- `remove_team(teamId)` - Remove an existing team
- `add_player(teamId, strength)` - Add a player to a team
- `remove_newest_player(teamId)` - Remove the most recently added player
- `play_match(teamId1, teamId2)` - Play a match between two teams
- `num_wins_for_team(teamId)` - Get number of wins for a team
- `get_highest_ranked_team()` - Get the highest ranked team
- `unite_teams(teamId1, teamId2)` - Merge two teams
- `play_tournament(lowPower, highPower)` - Organize a tournament

### Status Types
- `SUCCESS` - Operation completed successfully
- `ALLOCATION_ERROR` - Memory allocation failed
- `INVALID_INPUT` - Invalid input parameters
- `FAILURE` - Operation failed due to business logic

## Algorithm Complexity
- Team operations: O(log n) average case
- Match operations: O(log n) 
- Tournament operations: O(log n) per match
- Player management: O(log n) for strength tree operations

## Input Format
The program reads commands from standard input:
```
add_team 1
add_player 1 100
add_team 2
add_player 2 150
play_match 1 2
get_highest_ranked_team
```
