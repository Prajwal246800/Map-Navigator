# DSA-SEM3-miniProject
# Map Management System

This project implements a map management system in C that allows users to manage places and routes between them within a map.

## Overview
The program offers the following functionalities:
### 1 . Adding a New Place: Users can add new places to the map, storing their names in a file for future reference.
### 2. Creating Roads: Allows the creation of connections (roads) between existing places within the map.
### 3. Deleting Roads and Places: Users can remove connections between places and delete existing places from the map.
### 4. Displaying Information: Options to display all places, show connections of a specific place, and find routes between places.
### 5. Finding Shortest Route: Finds the shortest route between two selected places in the map.


## Instructions
### Getting Started
To run the program:
1. Compile the 'main.c' file.
2. Execute the compiled file to start the map management system.

### Usage
Follow the on-screen prompts and menu options to perform various actions within the program:
1. Enter the corresponding number for the desired action from the menu.
2. For certain actions (adding a place, creating roads, etc.), follow the instructions prompted by the program.

## File Management
The program stores place information in a file named PlaceInfo.txt. This file holds the names of the places within the map.

## Usage Examples
### Adding a Place
```1. Add a new place```

```Enter the name of the new place: NewCity```

```Place added successfully!```

### Creating a Road
```2. Add a new road```

```Enter the indices of the places to connect (separated by a space): 0 1```

```Road added successfully!```

### Finding Shortest Route
```9. Display Shortest Route```

```Enter the indices of the places to find the shortest path (separated by a space): 0
 2```
```Shortest path: PlaceA <- PlaceB <- PlaceC```


Clone the Repository:
  git clone https://github.com/Prajwal246800/DSA-MiniProject-SEM-3.git
  
Navigate to the Directory:
  cd DSA-MiniProject-SEM-3
  
Compile the Code:
  gcc DSAMiniProject.c -o map_navigation
  
Run the Program:
  ./map_navigation
