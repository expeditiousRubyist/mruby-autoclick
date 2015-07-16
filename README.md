# mruby-autoclick

mruby-autoclick is a port of the auto_click ruby gem to mruby. It is used for simulating mouse clicks, cursor movement, and keystrokes on Windows computers. Non-Windows platforms are not supported, as mruby-autoclick -- like its predecessor -- relies on functions in User32.DLL. Modification to support Unix platforms, however, would be much-welcomed.

# Current Supported Usage:

Mouse clicking can be performed using the following function calls:

```
  AutoClick.left_click
  AutoClick.right_click
  AutoClick.middle_click
  AutoClick.double_click
```
Cursor movement can be performed using the following function calls:

```
  AutoClick.mouse_move x, y
  AutoClick.left_drag sx, sy, ex, ey # s = start, e = end
  AutoClick.right_drag sx, sy, ex, ey

  # One can also obtain cursor information with
  pos = AutoClick.cursor_position
```

The keyboard can be controlled through these function calls
```
  # To type a capital 'A'
  AutoClick.key_down :leftshift # Hold down key
  AutoClick.key_stroke :a       # Press key
  AutoClick.key_up :left_shift  # Release key

  # Or in one command
  AutoClick.type "A" # Note: must be UTF-8 encoded
```

More features to be supported soon!

# Licensing and Copyright:

mruby-autoclick is available under the MIT License, as its predecessor auto_click was. The original source code by Tom Lam can be found at https://github.com/erinata/auto_click
