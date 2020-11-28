#pragma once

#include <ncurses.h>
#include <string>
#include <sstream>
#include <map>
#include <vector>

////////////////////////////////// MACROS ////////////////////////////////////

// These macros just define a simple way of finding the middle of stdscr
#define MIDHEIGHT           (LINES/2)
#define MIDWIDTH            (COLS/2)

// This macro is for finding the center of a user-created WINDOW
#define CENTER(w, x, y)     {                          \
                              getmaxyx((w), (y), (x)); \
                              x = ((x) / 2) - 1;       \
                              y = ((y) / 2) - 1;       \
                            }

///////////////////////////////// STRUCTS ////////////////////////////////////

/*
 * The Point is my way of creating easy to use coordinates, and it uses the
 * more typical Cartesian style of ordering (x first, y second), as opposed to
 * ncurses' y-before-x convention. While this convention is understandable
 * from the perspective of computer graphics, I simply prefer the x-before-y
 * style more.
 */
struct Point {

    int x, y;

    Point() : x(0), y(0) {}
    Point(int xIn, int yIn) : x(xIn), y(yIn) {}

    static bool pointsHaveEqualX(Point a, Point b) {
        return a.x == b.x;
    }

    static bool pointsHaveUnequalX(Point a, Point b) {
        return !pointsHaveEqualX(a, b);
    }

    static bool pointsHaveEqualY(Point a, Point b) {
        return a.y == b.y;
    }

    static bool pointsHaveUnequalY(Point a, Point b) {
        return !pointsHaveEqualY(a, b);
    }

};

/*
 * The Box is my way of creating boxes/borders by specifying a set of four
 * corners. This struct does not contain any information on how the border
 * looks, only where it appears.
 */
struct Box {

    Point ul, ur, ll, lr; // Upper left, upper right, lower left, lower right

    // Default constructor uses LINES and COLS (stdscr)
    Box() : ul(Point(0, 0)), ur(Point(COLS - 1, 0)),
            ll(Point(0, LINES - 1)), lr(Point(COLS - 1, LINES - 1)) {}

    // Construct Boxes with two points, or all four corners
    // Two points is safer, because it ensures that the corners all line up
    Box(Point ulIn, Point lrIn) : ul(ulIn), ur(Point(lrIn.x, ulIn.y)),
            ll(Point(ulIn.x, lrIn.y)), lr(lrIn) {}
    Box(Point ulIn, Point urIn, Point llIn, Point lrIn) :
        ul(ulIn), ur(urIn), ll(llIn), lr(lrIn) {}

};

/////////////////////////////// DRAWING UTILS ////////////////////////////////

// Drawing functions can take an optional WINDOW *, otherwise use stdscr

// Get attributes by a friendly, human-readable name
int getAttribute(std::string name);

// Combine an arbitrary amount of attributes into one attribute
// The first parameter tells the function how many attributes to expect
int combineAttributes(int num, ...);

// Draw character at a given point.
void drawCharAtPoint(char ch, Point p, WINDOW * win = NULL);

// Draw string at a given point
void drawStringAtPoint(std::string text, Point p, WINDOW * win = NULL);

// Draw a string centered on a given point
void drawCenteredStringAtPoint(std::string text, Point p, WINDOW * win = NULL);

// Set attributes for the given WINDOW (or default to stdscr)
void setAttributes(int attr, WINDOW * win = NULL);

// Unset attributes for the given WINDOW (or default to stdscr)
void unsetAttributes(int attr, WINDOW * win = NULL);

// Draw horizontal line between two points, using given character
void drawCustomHLineBetweenPoints(char ch, Point a, Point b, WINDOW * win = NULL);

// Draw vertical line between two points, using given character
void drawCustomVLineBetweenPoints(char ch, Point a, Point b, WINDOW * win = NULL);

// Draw horizontal line between two points, using ALTCHARSET
void drawHLineBetweenPoints(Point a, Point b, WINDOW * win = NULL);

// Draw vertical line between two points, using ALTCHARSET
void drawVLineBetweenPoints(Point a, Point b, WINDOW * win = NULL);

// Draw a box with user-defined characters
// Characters should always be in the following order:
// Top, Bottom, Left, Right, Upper Left, Upper Right, Lower Left, Lower Right
void drawCustomBox(Box b, char * chars, WINDOW * win = NULL);

// Draw a default box
void drawBox(Box b, WINDOW * win = NULL);

// Fill a box with the given character
void fillBoxWithChar(Box b, char ch, WINDOW * win = NULL);

// Clear screen in given box by drawing spaces
// I do this instead of using clear() to avoid latency issues
void clearBox(Box b, WINDOW * win = NULL);

/////////////////////////////// BASE CLASSES /////////////////////////////////

/*
 * The Engine class is a basic wrapper for initializing and running an ncurses
 * application. The user creates a subclass of the Engine and defines an
 * implementation for the init() and run() methods. The default behavior of
 * the Engine base class takes care of all the low level ncurses stuff like
 * setting up the environment and colors, as well as destroying stdscr when
 * it is deleted.
 */
class Engine {

protected:
    // Create basic color pairs using transparent background
    void initializeColorPairs();
    // Set various environment variables to reasonable defaults
    void initializeScreenVariables();
    // Initialize curses environment
    void setupCursesEnvironment();
    // Make sure to clean up after ourselves
    void teardownCursesEnvironment();

public:
    // Setup curses when the Engine is created
    Engine();
    // Teardown curses when the Engine is destroyed
    virtual ~Engine();

    // The user must implement the following two methods in their subclass:
    /*
     * init() is where you should initialize any members of your Engine
     * subclass, and prepare to start running the application.
     */
    virtual void init() = 0;
    /*
     * run() is where you start accepting user input and handling events.
     */
    virtual void run() = 0;

};

/*
 * The Panel class is a handy abstraction for making panels in your app that
 * use separate screens. Supply it with a Box for its corners in relation to
 * the terminal screen as a whole and it will handle sizing and resizing for
 * you! When users subclass this base class, they have the option of making
 * their own drawPanel() function, but the default is provided out of the box.
 */
class Panel {

protected:
    WINDOW * win;
    std::string title;
    Box globalDimensions;   // globalDimensions is in relation to stdscr
    Box localDimensions;    // localDimensions is in relation to win
    int lines, columns;

    // Create new internal window based on global position
    void setupWindow();
    // Safely destroy internal window
    void teardownWindow();
    // Default draws a box around the window
    void drawBorder();
    // Default finds middle of top line, draws title string
    void drawTitle();
    // Refresh internal window
    void refreshWindow();
    // Destroy old window, make a new one
    void replaceWindow();
    // Clear the space of the internal window
    void clearScreen();

public:
    // Title string is optional
    Panel(Box globalDimensionsIn, std::string titleIn = "");
    // Clean up after ourselves
    virtual ~Panel();

    // The user has the choice of leaving this, or overriding it. The default
    // just draws the border and title, then refreshes.
    virtual void drawPanel();
    // Given a new Box of dimensions, reset the internal sizes and window
    void resizePanel(Box newGlobalDimensions);

    WINDOW * getWin();

    void setTitle(std::string newTitle);

};

///////////////////////////// LAYOUT UTILITIES ///////////////////////////////

/*
 * Custom exception for denoting invalid ratio strings
 */
struct InvalidRatioException : public std::exception {

private:
    const char * message;

public:
    InvalidRatioException(const char * messageIn) : message(messageIn) {}
    const char * what() throw () {
        return message;
    }

};

/*
 * The Layouts class is a utility class designed so that users can either ask
 * for a custom layout by passing it a valid ratio string (e.g., "1:1:2"), or
 * they can use one of the default basic layouts. All the class does is return
 * a vector of Boxes, so the user can use them when making their Panels.
 */
class Layouts {

private:
    // This function is used to see if the numbers in ratio strings are ints
    static bool isInteger(const std::string & s) {
       if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) {
           return false;
       }

       char * p;
       strtol(s.c_str(), &p, 10);

       return (*p == 0);
    }

    /*
     * A ratio string is valid if and only if it contains an arbitrary series
     * of integer values delimited by a colon (:) character, e.g. "1:1:2".
     * Colons cannot appear more than once in a row (so "1::1:2" is not valid)
     * and cannot appear on either end of the string (":1:1:2" isn't valid).
     */
    static void validateRatio(std::string ratio) {
        // Check for at least one colon
        if(ratio.find(':') == std::string::npos) {
            const char * message = "Ratios must contain at least one colon.";
            throw InvalidRatioException(message);
        }

        // Check for colons on either end
        char first = *(ratio.begin());
        char last = *(--(ratio.end()));
        if(first == ':' || last == ':') {
            const char * message = "Ratios cannot begin or end with a colon.";
            throw InvalidRatioException(message);
        }

        // Check for double colons
        int length = ratio.length();
        for(int i = 1; i < length; i++) {
            if(ratio[i] == ':' && ratio[i - 1] == ':') {
                const char * message = "Ratios can only be delimited by single colons.";
                throw InvalidRatioException(message);
            }
        }

        // Split string on colons and check for integers
        std::stringstream ss(ratio);
        std::string token;
        while(std::getline(ss, token, ':')) {
            // Check if token is integer
            if(!isInteger(token)) {
                const char * message = "Ratios can only contain valid integers.";
                throw InvalidRatioException(message);
            }
            // Integer cannot be 0
            int num = std::stoi(token);
            if(num == 0) {
                const char * message = "Ratios cannot contain 0 as an integer.";
                throw InvalidRatioException(message);
            }
        }

        // If all the above checks out, ratio is valid
        return;
    }

    // Split the ratio string on colons and cast numbers to ints.
    static std::vector<int> extractNumsFromString(std::string ratio) {
        std::vector<int> nums;
        std::stringstream ss(ratio);
        std::string token;
        while(std::getline(ss, token, ':')) {
            int num = std::stoi(token);
            nums.push_back(num);
        }

        return nums;
    }

    /*
     * Create Box dimensions and track the position on the screen so far,
     * resulting in a set of distinct Boxes that take up the proper space
     * and positions desired.
     */
    static std::vector<Box> calculateHBoxes(std::vector<int> nums, Box * dimensions) {
        // Get an overall ratio base
        int base = 0;
        for(int num : nums) {
            base = base + num;
        }

        std::vector<Box> boxes;

        // Initialize dimensions based on dimensions (stdscr by default)
        int fullWidth, fullHeight;
        int startX, startY;
        if(dimensions != NULL) {
            // Base width and height on passed window
            fullWidth = (dimensions->ur.x - dimensions->ul.x);
            fullHeight = (dimensions->lr.y - dimensions->ur.y);
            startX = dimensions->ul.x;
            startY = dimensions->ul.y;
        } else {
            // Base width and height on stdscr
            fullWidth = COLS - 1;
            fullHeight = LINES - 1;
            startX = 0;
            startY = 0;
        }

        int lastX = startX - 1;
        for(int num : nums) {
            // Calculate how many rows and columns this box takes
            int rows = fullHeight;
            float frac = (float) num / base;
            int columns = (int) (frac * fullWidth);

            // Truncate columns if they would go offscreen
            if(lastX + 1 + columns >= fullWidth) {
                columns = fullWidth - (lastX + 1);
            }

            // Turn those rows and columns into Box dimensions
            Point ul(lastX + 1, startY);
            Point lr(lastX + 1 + columns, startY + rows);
            Box corners(ul, lr);

            boxes.push_back(corners);

            // Update lastX to move new Box over by old Box width
            lastX = lr.x;
        }

        return boxes;
    }

    // Similar to calculateHBoxes, but for vertical layouts.
    static std::vector<Box> calculateVBoxes(std::vector<int> nums, Box * dimensions) {
        // Get an overall ratio base
        int base = 0;
        for(int num : nums) {
            base = base + num;
        }

        std::vector<Box> boxes;

        // Initialize dimensions based on dimensions (stdscr by default)
        int fullWidth, fullHeight;
        int startX, startY;
        if(dimensions != NULL) {
            // Base width and height on passed window
            fullWidth = (dimensions->ur.x - dimensions->ul.x);
            fullHeight = (dimensions->lr.y - dimensions->ur.y);
            startX = dimensions->ul.x;
            startY = dimensions->ul.y;
        } else {
            // Base width and height on stdscr
            fullWidth = COLS - 1;
            fullHeight = LINES - 1;
            startX = 0;
            startY = 0;
        }

        int lastY = startY - 1;
        for(int num : nums) {
            // Calculate how many rows and columns this box takes
            float frac = (float) num / base;
            int rows = (int) (frac * fullHeight);
            int columns = fullWidth;

            // Truncate rows if they would go offscreen
            if(lastY + 1 + rows >= fullHeight) {
                rows = fullHeight - (lastY + 1);
            }

            // Turn those rows and columns into Box dimensions
            Point ul(startX, lastY + 1);
            Point lr(startX + columns, lastY + 1 + rows);
            Box corners(ul, lr);

            boxes.push_back(corners);

            // Update lastX to move new Box over by old Box width
            lastY = lr.y;
        }

        return boxes;
    }

public:
    /*
     * The user must pass a proper ratio string, and it will return a vector
     * of Boxes, each one representing the position of a Panel in the given
     * layout. An optional Box can be passed, which can be used to make
     * "sub-layouts" within a user-defined area. A good use case for this is
     * to make layouts within a Panel's globalDimensions.
     *
     * NOTE: The user is responsible for catching any InvalidRatioExceptions
     * that are thrown, otherwise the Engine class' automatic curses cleanup
     * will not run, and the terminal may become stuck in curses mode upon
     * a runtime error.
     */
    static std::vector<Box> customHLayout(std::string ratio, Box * dimensions = NULL) {
        std::vector<Box> boxes;
        // Check for proper ratio string
        try {
            validateRatio(ratio);
        } catch(InvalidRatioException& e) {
            throw InvalidRatioException(e.what());
        }

        // Extract numbers from ratio
        std::vector<int> nums = extractNumsFromString(ratio);

        // Populate vector with boxes based on ratio nums
        boxes = calculateHBoxes(nums, dimensions);

        return boxes;
    }

    /* 
     * This is similar to customHLayout(), but for vertical layouts. The user
     * is still responsible for catching InvalidRatioExceptions.
     */
    static std::vector<Box> customVLayout(std::string ratio, Box * dimensions = NULL) {
        std::vector<Box> boxes;
        // Check for proper ratio string
        try {
            validateRatio(ratio);
        } catch(InvalidRatioException& e) {
            throw InvalidRatioException(e.what());
        }

        // Extract numbers from ratio
        std::vector<int> nums = extractNumsFromString(ratio);

        // Populate vector with boxes based on ratio nums
        boxes = calculateVBoxes(nums, dimensions);

        return boxes;
    }

    /*
     * What follows are a bunch of default layouts in both orientations. Since
     * these functions will use valid ratio strings, there is no need to catch
     * any InvalidRatioExceptions.
     */
    static std::vector<Box> HSplit(Box * dimensions = NULL) {
        return customHLayout("1:1", dimensions);
    }
    static std::vector<Box> HTwoThirdsLeft(Box * dimensions = NULL) {
        return customHLayout("2:1", dimensions);
    }
    static std::vector<Box> HTwoThirdsRight(Box * dimensions = NULL) {
        return customHLayout("1:2", dimensions);
    }
    static std::vector<Box> HThirds(Box * dimensions = NULL) {
        return customHLayout("1:1:1", dimensions);
    }
    static std::vector<Box> VSplit(Box * dimensions = NULL) {
        return customVLayout("1:1", dimensions);
    }
    static std::vector<Box> VTwoThirdsAbove(Box * dimensions = NULL) {
        return customVLayout("2:1", dimensions);
    }
    static std::vector<Box> VTwoThirdsBelow(Box * dimensions = NULL) {
        return customVLayout("1:2", dimensions);
    }
    static std::vector<Box> VThirds(Box * dimensions = NULL) {
        return customVLayout("1:1:1", dimensions);
    }

};
