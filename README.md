# Colony Management System

## Overview
This program is a Colony Management System designed and implemented by Filiz İpek Oktay. This C++ program allows users to manage resources, buildings, and the layout of a colony through a command-line interface.

## Features
- **Resource Management:** Load available resources from a specified file.
- **Building Consumption:** Specify resource consumption per building type.
- **Colony Layout:** Define the initial layout of the colony using a matrix of cells.
- **Building Construction:** Add new buildings to the colony, considering resource availability.
- **Building Removal:** Remove existing buildings from the colony and return consumed resources to the stock.
- **Display Options:** Print the colony, display resource consumption by each building, and view the current resource stock.
- **User Interaction:** Utilize an interactive menu to perform various actions and manage the colony effectively.

## File Structure
- **main.cpp:** Contains the main source code for THE1 program.
- **stock.txt:** Example file containing initial resource stock information.
- **consumption.txt:** Example file containing building consumption details.
- **colony.txt:** Example file defining the initial layout of the colony.

## How to Run
1. Compile the source code with a C++ compiler (e.g., g++).
   ```
   g++ main.cpp -o THE1
   ```

2. Run the compiled executable.
   ```
   ./THE1
   ```

3. Follow the on-screen prompts to interact with the Colony Management System.

## Usage Instructions
1. **Resource Files:**
   - Enter the file names for resources stock, consumption, and colony when prompted.
   - Ensure correct file formats and follow the guidelines for file content.

2. **Colony Management:**
   - Construct new buildings, remove existing buildings, and view the colony layout.
   - Monitor resource consumption for each building type.

3. **Display Options:**
   - Print the colony, resource consumption by each building, and the current resource stock.
   - Choose specific resources to display their consumption by each building.

4. **Exit:**
   - Terminate the program by choosing option 7 in the main menu.

## Notes
- Ensure correct file names and formats to avoid errors during file loading.
- Follow on-screen instructions for each menu option to perform desired actions.

**Thank you for using Colony Management System!**
