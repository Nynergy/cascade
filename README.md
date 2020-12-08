# cascade

A simple and customizable todo-list for the terminal.

------------------------------------------------------------------------------

![Showcase of cascade](showcase.png)

Seen above is an image of cascade in use. You can find it in the top left
corner of the screenshot.

# What is cascade and why does it exist?

_cascade_ is a lightweight todo-list application built on top of ncurses,
using my personal [vexes library](https://github.com/Nynergy/vexes) as a base
for much of the classes and screen rendering. It exists mostly because of this
library, since I wanted to test it out in a production context, but also
because I've been wanting to make a terminal application for a long time and I
also wanted a todo-list app. So here's my attempt at killing three birds with
one (thousand) keystroke(s).

In the past, I've been a user of [calcurse](https://calcurse.org) for managing
my todo-list, as well as appointments, but I've found it to be kind of
cumbersome, and a bit too feature-rich for what I want. Users of calcurse may
see quite a lot of inspiration from that app in cascade, and that observation
is not incorrect. But I hope you'll come to agree that cascade, while it isn't
as feature-rich as calcurse, is just as simple, if not simpler, and just as
appealing an option for your everyday todo-list needs.

# What exactly does it do?

It reads a file containing formatted todo-list information and renders that
information to the terminal screen in a dynamic, customizable way. It can
keep track of a global master list, or you can have it open and read a list by
specifying a path on the command line, as seen in the section below on usage.

Segment your list into sections, add or delete items and sections, edit names,
swap positions, change section colors, the features present in cascade even
allow the user to use it outside of todo-list purposes. At the end of the day,
it's really just a program that dynamically renders user-created panels to the
screen, so do whatever you want with it.

# How can I compile and run it?

Once you've cloned the repo, to compile cascade, simply run `make` in the base
directory. This should create a binary executable called `cascade`, which you
can copy to your `/usr/bin` by running `sudo make install` or by manually
copying it to wherever you like. After that, as long as it is visible in your
PATH, you can just call `cascade`.

Running `cascade` without any other command line arguments will open cascade
using the master list. This list is defined in the configuration file, which
you can learn more about in the section about configuration.

You can also specify a .todo file to open with cascade by giving the path to
it like so: `cascade example.todo`

Keep in mind that this file must exist beforehand and be properly formatted.
Format specifications are discussed in the section below.

To see a help menu, simply run `cascade -h` or `cascade --help` or even supply
an optional help topic, such as 'keybindings' or 'config', like so:
`cascade -h keybindings`

# What are these .todo files it uses?

The extension is just an arbitrary one that made sense for the program. What
matters about the list files is the format. If the file is not properly
formatted, then it may not be parsed and rendered correctly by cascade.

To see a list of specifications, run `cascade -h list` after installing it.
In short, a proper .todo file should look like the following, taken from the
`example.todo` file found at the base level of the repo:

```
[General] : 4
Clean House
Take Out Garbage
Schedule Car Maintenance

[School] : 2
CS471 Homework 14 (Due 12/3)
CS432 Final Project (Due 12/5)

[Work] : 3
Finish Registration Controls
Team Meeting Tuesday @ 9:00 AM
```

Lists are broken up into _sections_, and each section is comprised of _items_.
Sections in the format are delimited by an empty line, and begin with a line
declaring the title of the section, as well as its color code. This number is
translated into an ncurses color to render the section header with.

Each section declaration line is followed by a series of items in that section
without any blank lines in between. As soon as a blank line is encountered,
cascade will assume a new section has started, so be wary of that.

The format is like a demented .ini file, and I kind of love that about it, I
don't know why. Perhaps I shouldn't be allowed to come up with format
specifications.

# How can I configure it?

cascade will look for `~/.cascade/cascade.conf` as its configuration file, and
if it doesn't find it, it will create a default one. If you'd like to change
the default location of the config, go into the source code and change it
yourself, you're more than welcome to.

Once it's found that file, it will parse it and load the configuration
settings into the program. To see a list of configurable options, just run
`cascade -h config` on the command line.

__NOTE:__ MasterList must be defined if you are not supplying a file via
command line, so make sure that is set in the config file.

# How do I use it?

There are two modes in cascade: NORMAL, and MOVE. The keybindings are split
into two sections, one for each mode, found below:

### NORMAL MODE

Key | Action
----|-------
<kbd>q</kbd> | quit cascade
<kbd>j</kbd> and <kbd>k</kbd> | change item focus up and down
<kbd>J</kbd> and <kbd>K</kbd> | change section focus up and down
<kbd>g</kbd> and <kbd>G</kbd> | jump to top or bottom of section
<kbd>e</kbd> and <kbd>E</kbd> | edit item name/section title
<kbd>n</kbd> and <kbd>N</kbd> | add new item/section
<kbd>d</kbd> and <kbd>D</kbd> | delete focused item/section
<kbd>m</kbd> | enter MOVE mode
<kbd>c</kbd> | cycle focused section color
<kbd>s</kbd> | save any unsaved changes

### MOVE MODE

Key | Action
----|-------
<kbd>q</kbd> | quit cascade
<kbd>j</kbd> and <kbd>k</kbd> | move focused item up and down
<kbd>J</kbd> and <kbd>K</kbd> | move focused section up and down
<kbd>m</kbd> | exit MOVE mode
<kbd>s</kbd> | save any unsaved changes

To see a list of keybindings on the command line, just run
`cascade -h keybindings`

# Now what?

Use it. Or don't. Fork it. Change it. Send me issues. Do whatever you want,
send me some nice screenshots of you using it, I don't know.

Use it to get some work done, unlike me, who spent his free time making a tiny
TUI todo-list app instead of actual school work.
