# mruby-autoclick
# Copyright (C) expeditiousRubyist 2015
# 
# This module is for converting English names of keys to integer
# values recognized by the User32.DLL functions. Values are borrowed from here:
# https://github.com/erinata/auto_click/blob/master/lib/auto_click/virtual_key.rb
# 
# A hash table is used in place of a massive case statement, for performance
# reasons. 
module VirtualKey

  # NOTICE: Some of these keycodes are only valid on US Keyboards
  # Therefore, all commands which are dependent on these keycodes
  # may be incorrect for alternative keyboard layouts. It is highly
  # recommended that users of other keyboard layouts look up the
  # alternative keycodes and edit this hash table.

  @@keyboard_values = {
    back:           0x08, backspace:        0x08,
    tab:            0x09,
    clear:          0x0C,
    return:         0x0D, enter:            0x0D,
    shift:          0x10,
    ctrl:           0x11, control:          0x11,
    alt:            0x12, alternate:        0x12,
    pause:          0x13,
    cap:            0x14, capslock:         0x14, caplock:  0x14,
    esc:            0x1B, escape:           0x1B,
    space:          0x20, spacebar:         0x20,
    pageup:         0x21,
    pagedown:       0x22,
    end:            0x23,
    home:           0x24,
    left:           0x25, leftarrow:        0x25, leftkey:  0x25,
    up:             0x26, uparrow:          0x26, upkey:    0x26,
    right:          0x27, rightarrow:       0x27, rightkey: 0x27,
    down:           0x28, downarrow:        0x28, downkey:  0x28,
    select:         0x29,
    print:          0x2A,
    exe:            0x2B, execute:          0x2B,
    printscr:       0x2C, printscreen:      0x2C,
    ins:            0x2D, insert:           0x2D,
    del:            0x2E, delete:           0x2E,
    help:           0x2F,
    # Top row numbers, not numpad
    num0:           0x30, number0:          0x30,
    num1:           0x31, number1:          0x31,
    num2:           0x32, number2:          0x32,
    num3:           0x33, number3:          0x33,
    num4:           0x34, number4:          0x34,
    num5:           0x35, number5:          0x35,
    num6:           0x36, number6:          0x36,
    num7:           0x37, number7:          0x37,
    num8:           0x38, number8:          0x38,
    num9:           0x39, number9:          0x39,
    a:              0x41,
    b:              0x42,
    c:              0x43,
    d:              0x44,
    e:              0x45,
    f:              0x46,
    g:              0x47,
    h:              0x48,
    i:              0x49,
    j:              0x4A,
    k:              0x4B,
    l:              0x4C,
    m:              0x4D,
    n:              0x4E,
    o:              0x4F,
    p:              0x50,
    q:              0x51,
    r:              0x52,
    s:              0x53,
    t:              0x54,
    u:              0x55,
    v:              0x56,
    w:              0x57,
    x:              0x58,
    y:              0x59,
    z:              0x5A,
    win:            0x5B, windows:          0x5B, leftwin:  0x5B,
    # Note: the Windows key is sometimes referred to as the super key
    super:          0x5B, superkey:         0x5B,
    rightwin:       0x5C,
    app:            0x5D, application:      0x5D,
    sleep:          0x5F,
    numpad0:        0x60, numberpad0:       0x60,
    numpad1:        0x61, numberpad1:       0x61,
    numpad2:        0x62, numberpad2:       0x62,
    numpad3:        0x63, numberpad3:       0x63,
    numpad4:        0x64, numberpad4:       0x64,
    numpad5:        0x65, numberpad5:       0x65,
    numpad6:        0x66, numberpad6:       0x66,
    numpad7:        0x67, numberpad7:       0x67,
    numpad8:        0x68, numberpad8:       0x68,
    numpad9:        0x69, numberpad9:       0x69,
    # Numpad math operators
    multiply:       0x6A, multiplication:   0x6A,
    add:            0x6B, addition:         0x6B,
    separator:      0x6C,
    subtract:       0x6D, subtraction:      0x6D,
    decimal:        0x6E,
    divide:         0x6F, division:         0x6F,
    f1:             0x70,
    f2:             0x71,
    f3:             0x72,
    f4:             0x73,
    f5:             0x74,
    f6:             0x75,
    f7:             0x76,
    f8:             0x77,
    f9:             0x78,
    f10:            0x79,
    f11:            0x7A,
    f12:            0x7B,
    f13:            0x7B,
    f14:            0x7C,
    f15:            0x7D,
    f16:            0x7F,
    f17:            0x80,
    f18:            0x81,
    f19:            0x82,
    f20:            0x83,
    f21:            0x84,
    f22:            0x85,
    f23:            0x86,
    f24:            0x87,
    numlock:        0x90, numberlock:       0x90,
    scroll:         0x91, scrolllock:       0x91,
    leftshift:      0xA0, 
    rightshift:     0xA1,
    leftctrl:       0xA2, leftcontrol:      0xA2,
    rightctrl:      0xA3, rightcontrol:     0xA3,
    menu:           0xA4, leftmenu:         0xA4,
    rightmenu:      0xA5,
    browserback:    0xA6,
    browserforward: 0xA7,
    browserrefresh: 0xA8,
    browserstop:    0xA9,
    browsersearch:  0xAA,
    browserfav:     0xAB, browserfavorites: 0xAB,
    browserstart:   0xAC, browserhome:      0xAC,
    mute:           0xAD, volumemute:       0xAD, volmute:  0xAD,
    voldown:        0xAE, volumedown:       0xAE,
    volup:          0xAF, volumeup:         0xAF,
    nexttrack:      0xB0,
    previoustrack:  0xB1,
    mediastop:      0xB2,
    mediaplay:      0xB3, mediapause:       0xB3,
    mail:           0xB4,
    mediaselect:    0xB5,
    app1:           0xB6, application1:     0xB6,
    app2:           0xB7, application2:     0xB7,
    # Note: some of these are marked "US ONLY"
    # These may map to different keys on non-US keyboards
    # For more information, see this:
    # https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731%28v=vs.85%29.aspx
    semicolon:      0xBA, colon:            0xBA,                 # US ONLY
    equal:          0xBB, plus:             0xBB,
    comma:          0xBC, lessthan:         0xBC,
    hyphen:         0xBD, underscore:       0xBD, dash:     0xBD,
    period:         0xBE, greaterthan:      0xBE, dot:      0xBE,
    forwardslash:   0xBF, questionmark:     0xBF, slash:    0xBF, # US ONLY
    backtick:       0xC0, leftquote:        0xC0, tilde:    0xC0, # US ONLY
    leftbracket:    0xDB, leftbrace:        0xDB,                 # US ONLY
    backslash:      0xDC, pipe:             0xDC, bar:      0xDC, # US ONLY
    rightbracket:   0xDD, rightbrace:       0xDD,                 # US ONLY
    doublequote:    0xDE, rightquote:       0xDE, quote:    0xDE, # US ONLY
    play:           0xFA,
    zoom:           0xFB
  }

  def self.code_from_name name
    @@keyboard_values[name.downcase] or 0x00
  end
end