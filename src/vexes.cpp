#include "vexes.hpp"

//////////////////////////////// CONSTANTS ///////////////////////////////////

// This map is used to make using attributes easier and more readable
static const std::map<std::string, int> attributes = {
    {"standout", A_STANDOUT},
    {"underline", A_UNDERLINE},
    {"reverse", A_REVERSE},
    {"blink", A_BLINK},
    {"dim", A_DIM},
    {"bold", A_BOLD},
    {"protected", A_PROTECT},
    {"invisible", A_INVIS},
    {"alternate", A_ALTCHARSET},
    {"extract", A_CHARTEXT},
    {"black", COLOR_PAIR(0)},
    {"red", COLOR_PAIR(1)},
    {"green", COLOR_PAIR(2)},
    {"yellow", COLOR_PAIR(3)},
    {"blue", COLOR_PAIR(4)},
    {"magenta", COLOR_PAIR(5)},
    {"cyan", COLOR_PAIR(6)},
    {"white", COLOR_PAIR(7)}
};

/////////////////////////////// DRAWING UTILS ////////////////////////////////

int getAttribute(std::string name) {
    auto iter = attributes.find(name);
    // Check if name exists, otherwise return A_NORMAL
    if(iter == attributes.end()) {
        return A_NORMAL;
    } else {
        return iter->second;
    }
}

int combineAttributes(int num, ...) {
    va_list argList;
    int attr = A_NORMAL;
    va_start(argList, num);

    for(int i = 0; i < num; i++) {
        attr |= va_arg(argList, int);
    }

    va_end(argList);
    return attr;
}

void drawCharAtPoint(char ch, Point p, WINDOW * win) {
    if(win != NULL) {
        wmove(win, p.y, p.x);
        waddch(win, ch);
    } else {
        move(p.y, p.x);
        addch(ch);
    }
}

void drawStringAtPoint(std::string text, Point p, WINDOW * win) {
    if(win != NULL) {
        wmove(win, p.y, p.x);
        waddstr(win, text.c_str());
    } else {
        move(p.y, p.x);
        addstr(text.c_str());
    }
}

void drawCenteredStringAtPoint(std::string text, Point p, WINDOW * win) {
    // Compute new point offset by half of string's length
    size_t offset = text.size() / 2;
    Point newPoint(p.x - offset, p.y);
    
    // Delegate to drawStringAtPoint with new point
    drawStringAtPoint(text, newPoint, win);
}

void setAttributes(int attr, WINDOW * win) {
    if(win != NULL) {
        wattron(win, attr);
    } else {
        attron(attr);
    }
}

void unsetAttributes(int attr, WINDOW * win) {
    if(win != NULL) {
        wattroff(win, attr);
    } else {
        attroff(attr);
    }
}

void drawCustomHLineBetweenPoints(char ch, Point a, Point b, WINDOW * win) {
    // Only draw line if points are on the same Y level
    if(Point::pointsHaveUnequalY(a, b)) { return; }

    // To avoid issues of order, we check which x is larger
    if(a.x < b.x) {
        for(int i = a.x; i < b.x + 1; i++) {
            drawCharAtPoint(ch, Point(i, a.y), win);
        }
    } else if(a.x > b.x) {
        for(int i = b.x; i < a.x + 1; i++) {
            drawCharAtPoint(ch, Point(i, b.y), win);
        }
    } else {
        // x is equal, so we only draw a single character
        drawCharAtPoint(ch, a, win);
    }
}

void drawCustomVLineBetweenPoints(char ch, Point a, Point b, WINDOW * win) {
    // Only draw line if points are on the same X level
    if(Point::pointsHaveUnequalX(a, b)) { return; }

    // To avoid issues of order, we check which y is larger
    if(a.y < b.y) {
        for(int i = a.y; i < b.y + 1; i++) {
            drawCharAtPoint(ch, Point(a.x, i), win);
        }
    } else if(a.y > b.y) {
        for(int i = b.y; i < a.y + 1; i++) {
            drawCharAtPoint(ch, Point(b.x, i), win);
        }
    } else {
        // y is equal, so we only draw a single character
        drawCharAtPoint(ch, a, win);
    }
}

void drawHLineBetweenPoints(Point a, Point b, WINDOW * win) {
    // Delegate to the custom line with the HLINE character
    setAttributes(getAttribute("alternate"), win);
    drawCustomHLineBetweenPoints(ACS_HLINE, a, b, win);
    unsetAttributes(getAttribute("alternate"), win);
}

void drawVLineBetweenPoints(Point a, Point b, WINDOW * win) {
    // Delegate to the custom line with the VLINE character
    setAttributes(getAttribute("alternate"), win);
    drawCustomVLineBetweenPoints(ACS_VLINE, a, b, win);
    unsetAttributes(getAttribute("alternate"), win);
}

void drawCustomBox(Box b, char * chars, WINDOW * win) {
    // Draw top and bottom of box
    drawCustomHLineBetweenPoints(chars[0], b.ul, b.ur, win);
    drawCustomHLineBetweenPoints(chars[1], b.ll, b.lr, win);

    // Draw left and right of box
    drawCustomVLineBetweenPoints(chars[2], b.ul, b.ll, win);
    drawCustomVLineBetweenPoints(chars[3], b.ur, b.lr, win);

    // Draw corners of the box
    drawCharAtPoint(chars[4], b.ul, win);
    drawCharAtPoint(chars[5], b.ur, win);
    drawCharAtPoint(chars[6], b.ll, win);
    drawCharAtPoint(chars[7], b.lr, win);
}

void drawBox(Box b, WINDOW * win) {
    // Define alternate characters to use and set attribute on
    char alts[] = {
                    ACS_HLINE, ACS_HLINE, ACS_VLINE, ACS_VLINE,
                    ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER
                  };
    setAttributes(getAttribute("alternate"), win);
    drawCustomBox(b, alts, win);
    unsetAttributes(getAttribute("alternate"), win);
}

void fillBoxWithChar(Box b, char ch, WINDOW * win) {
    for(int y = b.ul.y; y < b.lr.y + 1; y++) {
        // Draw one line at a time to take advantage of addch()
        if(win != NULL) {
            wmove(win, y, b.ul.x);
        } else {
            move(y, b.ul.x);
        }

        for(int x = b.ul.x; x < b.lr.x + 1; x++) {
            if(win != NULL) {
                waddch(win, ch);
            } else {
                addch(ch);
            }
        }
    }
}

void clearBox(Box b, WINDOW * win) {
    fillBoxWithChar(b, ' ', win);
}

/////////////////////////////// BASE CLASSES /////////////////////////////////

/* ENGINE */

Engine::Engine() {
    setupCursesEnvironment();
}

Engine::~Engine() {
    teardownCursesEnvironment();
}

void Engine::setupCursesEnvironment() {
    initializeScreenVariables();
    initializeColorPairs();
}

void Engine::initializeScreenVariables() {
    initscr();		        // Begin curses mode
    cbreak();		        // Disable line buffering
    keypad(stdscr, TRUE);	// Enable extra keys
    noecho();		        // Disable echoing keys to console
    start_color();		    // Enable color mode
    curs_set(0);		    // Set cursor to be invisible
    timeout(50);		    // Make getch a non-blocking call
}

void Engine::initializeColorPairs() {
    int backgroundColor = -1; // Transparency
    use_default_colors();

    init_pair(0, COLOR_BLACK, backgroundColor);
    init_pair(1, COLOR_RED, backgroundColor);
    init_pair(2, COLOR_GREEN, backgroundColor);
    init_pair(3, COLOR_YELLOW, backgroundColor);
    init_pair(4, COLOR_BLUE, backgroundColor);
    init_pair(5, COLOR_MAGENTA, backgroundColor);
    init_pair(6, COLOR_CYAN, backgroundColor);
    init_pair(7, COLOR_WHITE, backgroundColor);
}

void Engine::teardownCursesEnvironment() {
    endwin(); // Destroy stdscr
}

/* PANEL */
Panel::Panel(Box globalDimensionsIn, std::string titleIn) {
    title = titleIn;
    globalDimensions = globalDimensionsIn;

    // Calculate sizes based on global dimensions
    lines = globalDimensions.ll.y - globalDimensions.ul.y;
    columns = globalDimensions.ur.x - globalDimensions.ul.x;

    // Use sizes in creating local dimensions
    Point ul(0, 0); Point lr(columns, lines);
    localDimensions = Box(ul, lr);

    // Finally, create the internal window
    setupWindow();
}

Panel::~Panel() {
    teardownWindow();
}

void Panel::setupWindow() {
    win = newwin(lines + 1, columns + 1, globalDimensions.ul.y, globalDimensions.ul.x);
}

void Panel::teardownWindow() {
    delwin(win);
}

void Panel::drawPanel() {
    drawBorder();
    drawTitle();
    refreshWindow();
}

void Panel::drawBorder() {
    drawBox(localDimensions, win);
}

void Panel::drawTitle() {
    Point titlePoint(columns / 2, 0);
    drawCenteredStringAtPoint(title, titlePoint, win);
}

void Panel::refreshWindow() {
    wrefresh(win);
}

void Panel::resizePanel(Box newGlobalDimensions) {
    globalDimensions = newGlobalDimensions;
    lines = newGlobalDimensions.ll.y - newGlobalDimensions.ul.y;
    columns = newGlobalDimensions.ur.x - newGlobalDimensions.ul.x;

    Point ul(0, 0); Point lr(columns, lines);
    localDimensions = Box(ul, lr);

    replaceWindow();
}

void Panel::replaceWindow() {
    teardownWindow();
    setupWindow();
}

void Panel::clearScreen() {
    clearBox(localDimensions, win);
}

WINDOW * Panel::getWin() {
    return win;
}

void Panel::setTitle(std::string newTitle) {
    title = newTitle;
}

/* FORM */
Form::Form(Point origin) :
    origin(origin), prompt(""), promptLength((int)prompt.size()), buffer("") {
    lines = 1; columns = COLS - (promptLength + 2);
    win = newwin(lines, COLS - 1, origin.y, origin.x);
    keypad(win, TRUE);
}

Form::Form(Point origin, int length) :
    origin(origin), prompt(""), promptLength((int)prompt.size()), buffer("") {
    lines = 1; columns = length - (promptLength + 1);
    win = newwin(lines, length, origin.y, origin.x);
    keypad(win, TRUE);
}

Form::Form(Point origin, std::string prompt) :
    origin(origin), prompt(prompt), promptLength((int)prompt.size()), buffer("") {
    lines = 1; columns = COLS - (promptLength + 2);
    win = newwin(lines, COLS - 1, origin.y, origin.x);
    keypad(win, TRUE);
}

Form::Form(Point origin, int length, std::string prompt) :
    origin(origin), prompt(prompt), promptLength((int)prompt.size()), buffer("") {
    lines = 1; columns = length - (promptLength + 1);
    win = newwin(lines, length, origin.y, origin.x);
    keypad(win, TRUE);
}

Form::~Form() {
    delwin(win);
}

void Form::drawForm() {
    clearForm();
    drawPrompt();
    drawBuffer();
}

void Form::clearForm() {
    Point ul(promptLength + 1, 0);
    Point lr(columns + (promptLength + 1), 0);
    Box formBox(ul, lr);
    clearBox(formBox, win);
}

void Form::drawPrompt() {
    Point promptPoint(0, 0);
    drawStringAtPoint(prompt, promptPoint, win);
}

void Form::drawBuffer() {
    int bufferSize = (int)buffer.size();
    std::string tempBuffer;
    int formLength = columns;
    if(bufferSize >= formLength) {
        tempBuffer = buffer.substr(bufferSize - formLength, bufferSize);
    } else {
        tempBuffer = buffer;
    }

    Point bufferPoint(promptLength + 1, 0);
    drawStringAtPoint(tempBuffer, bufferPoint, win); 
}

void Form::injectString(std::string str) {
    for(char ch : str) {
        addCharToBuffer(ch);
    }
}

void Form::addCharToBuffer(char ch) {
    char converted[2];
    sprintf(converted, "%c", ch);
    std::string str = converted;
    buffer = buffer + str;
}

void Form::removeCharFromBuffer() {
    buffer = buffer.substr(0, buffer.size() - 1);
}

std::string Form::edit() {
    // Make cursor visible while typing
    curs_set(1);

    int ch;
    bool exit = false;
    while(!exit) {
        drawForm();
        ch = wgetch(win);
        switch(ch) {
            case 10: // Enter Key (submit)
            case KEY_F(1): // Cancel form input
                exit = true;
                break;
            default: // Delegate to form driver
                handleInput(ch);
                break;
        }
    }

    // Make cursor invisible after typing
    curs_set(0);

    return trimWhitespace(buffer);
}

void Form::handleInput(int ch) {
    switch(ch) {
        case KEY_BACKSPACE:
        case 127: // Backspace Key
            removeCharFromBuffer();
            break;
        default: // Normal character
            addCharToBuffer(ch);
            break;
    }
}

std::string Form::trimWhitespace(std::string str) {
    size_t first = str.find_first_not_of(' ');
	if(std::string::npos == first) {
		return str;
	}

	size_t last = str.find_last_not_of(' ');

	return str.substr(first, (last - first + 1));
}
