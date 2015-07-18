# mruby-autoclick

mruby-autoclick is an MRuby gem for automating mouse clicks, cursor movement, and keystrokes on the Windows platform, based roughly on the API provided in the auto_click gem for Ruby, written by erinata (source: https://github.com/erinata/auto_click). It has been modified in a few ways - namely, the majority of the code is written in C, for both performance reasons, and because MRuby does not ship with a means of calling functions in Windows DLLs. A few of the functions have been changed as well.

# Available functions

The following functions are available under the `AutoClick` module

```
# Functions for controlling mouse clicks.
left_mouse_down
left_mouse_up
left_click
double_click

right_mouse_down
right_mouse_up
right_click

middle_mouse_down
middle_mouse_up
middle_click

# Functions for controlling mouse movement.
# (x, y) coordinates have the origin at the top-left corner of the screen.
# For drag functions, s = starting coordinate, e = ending coordinate.
mouse_move(x, y)
left_drag(sx, sy, ex, ey)
right_drag(sx, sy, ex, ey)

# Functions for controlling the mouse wheel.
# Note that for both functions, direction is an integer, with positive values
# representing a "scroll up/left" and negative values representing a "scroll
# down/right."
mouse_scroll_vertical(direction)
mouse_scroll_horizontal(direction) # For Windows >= Vista

# Functions for controlling the keyboard.
# Keyname values are available in a hash table in the VirtualKey module.
# Note that for some keys, a US keyboard is assumed. These keys are labeled
# explicitly in the virtual_key.rb file as "US Only."
key_down(keyname)
key_up(keyname)
key_stroke(keyname)
type(str) # Note: str must be encoded as UTF-8

# Functions for querying keyboard/mouse state
is_key_down?(keyname)
is_key_toggled?(keyname)
cursor_position
```

More features to be supported soon!

# Licensing and Copyright:

mruby-autoclick is available under the MIT License, as its predecessor auto_click was. The original source code by erinata can be found at https://github.com/erinata/auto_click
