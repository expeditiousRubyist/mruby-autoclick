/* mruby-autoclick
 * Copyright (C) expeditiousRubyist 2015
 * 
 * Based on the auto_click ruby gem developed by Tom Lam
 * His source code and copyright information can be found at
 * https://github.com/erinata/auto_click
 */

#if !defined(_WIN32) && !defined(_WIN64)
#error This module does not work on non-Windows systems
#endif

#include <windows.h>
#include "mruby.h"
#include "mruby/array.h"

#define DRAG_WAIT_TIME 100

static const char *MODULE_NAME    = "AutoClick";
static const char *VK_MODULE_NAME = "VirtualKey";

static inline INPUT
make_mouse_input(LONG dx, LONG dy, DWORD mouseData, 
                 DWORD dwFlags, DWORD time, ULONG_PTR dwExtraInfo)
{
	INPUT ret;

	ret.type = INPUT_MOUSE;
	ret.mi.dx = dx;
	ret.mi.dy = dy;
	ret.mi.mouseData = mouseData;
	ret.mi.dwFlags = dwFlags;
	ret.mi.time = time;
	ret.mi.dwExtraInfo = dwExtraInfo;

	return ret;
}

static INPUT LEFT_MOUSE_DOWN;
static INPUT LEFT_MOUSE_UP;
static INPUT RIGHT_MOUSE_DOWN;
static INPUT RIGHT_MOUSE_UP;
static INPUT MIDDLE_MOUSE_DOWN;
static INPUT MIDDLE_MOUSE_UP;

static mrb_value
mrb_autoclick_left_click(mrb_state *mrb, mrb_value klass)
{
	static int initialized = 0;
	static INPUT ips[2];
	if (!initialized) {
		ips[0] = LEFT_MOUSE_DOWN;
		ips[1] = LEFT_MOUSE_UP;
		initialized = 1;
	}
	SendInput(2, ips, sizeof(INPUT));
	return mrb_nil_value();
}

static mrb_value
mrb_autoclick_right_click(mrb_state *mrb, mrb_value klass)
{
	static int initialized = 0;
	static INPUT ips[2];
	if (!initialized) {
		ips[0] = RIGHT_MOUSE_DOWN;
		ips[1] = RIGHT_MOUSE_UP;
		initialized = 1;
	}
	SendInput(2, ips, sizeof(INPUT));
	return mrb_nil_value();
}

static mrb_value
mrb_autoclick_middle_click(mrb_state *mrb, mrb_value klass)
{
	static int initialized = 0;
	static INPUT ips[2];
	if (!initialized) {
		ips[0] = MIDDLE_MOUSE_DOWN;
		ips[1] = MIDDLE_MOUSE_UP;
		initialized = 1;
	}
	SendInput(2, ips, sizeof(INPUT));
	return mrb_nil_value();
}

static mrb_value
mrb_autoclick_double_click(mrb_state *mrb, mrb_value klass)
{
	mrb_autoclick_left_click(mrb, klass);
	mrb_autoclick_left_click(mrb, klass);
	return mrb_nil_value();
}

static mrb_value
mrb_autoclick_cursor_position(mrb_state *mrb, mrb_value klass)
{
	POINT pt;
	mrb_value ary;
	mrb_value vals[2];

	GetCursorPos(&pt);
	vals[0] = mrb_fixnum_value(pt.x);
	vals[1] = mrb_fixnum_value(pt.y);
	ary = mrb_ary_new_from_values(mrb, 2, vals);
	return ary;
}

static mrb_value
mrb_autoclick_mouse_move(mrb_state *mrb, mrb_value klass)
{
	mrb_int x, y;
	mrb_get_args(mrb, "ii", &x, &y);
	SetCursorPos(x, y);
	return mrb_nil_value();
}

static mrb_value
mrb_autoclick_mouse_scroll_vertical(mrb_state *mrb, mrb_value klass)
{
	INPUT ip;
	mrb_int direction;

	mrb_get_args(mrb, "i", &direction);
	ip = make_mouse_input(
		0, 0, WHEEL_DELTA * direction,
		MOUSEEVENTF_WHEEL, 0, 0
	);
	SendInput(1, &ip, sizeof(INPUT));
	return mrb_nil_value();
}

/* If we are using Vista or higher */
#if WINVER >= 0x0600
static mrb_value
mrb_autoclick_mouse_scroll_horizontal(mrb_state *mrb, mrb_value klass)
{
	INPUT ip;
	mrb_int direction;

	mrb_get_args(mrb, "i", &direction);
	ip = make_mouse_input(
		0, 0, WHEEL_DELTA * direction,
		MOUSEEVENTF_HWHEEL, 0, 0
	);
	SendInput(1, &ip, sizeof(INPUT));
	return mrb_nil_value();
}
#endif

static mrb_value
mrb_autoclick_left_drag(mrb_state *mrb, mrb_value klass)
{
	mrb_int sx, sy, ex, ey;
	mrb_get_args(mrb, "iiii", &sx, &sy, &ex, &ey);
	SetCursorPos(sx, sy);
	Sleep(DRAG_WAIT_TIME);
	SendInput(1, &LEFT_MOUSE_DOWN, sizeof(INPUT));
	Sleep(DRAG_WAIT_TIME);
	SetCursorPos(ex, ey);
	Sleep(DRAG_WAIT_TIME);
	SendInput(1, &LEFT_MOUSE_UP, sizeof(INPUT));
	Sleep(DRAG_WAIT_TIME);
	return mrb_nil_value();
}

static mrb_value
mrb_autoclick_right_drag(mrb_state *mrb, mrb_value klass)
{
	mrb_int sx, sy, ex, ey;

	mrb_get_args(mrb, "iiii", &sx, &sy, &ex, &ey);
	SetCursorPos(sx, sy);
	Sleep(DRAG_WAIT_TIME);
	SendInput(1, &RIGHT_MOUSE_DOWN, sizeof(INPUT));
	Sleep(DRAG_WAIT_TIME);
	SetCursorPos(ex, ey);
	Sleep(DRAG_WAIT_TIME);
	SendInput(1, &RIGHT_MOUSE_UP, sizeof(INPUT));
	Sleep(DRAG_WAIT_TIME);
	return mrb_nil_value();
}

static mrb_value
mrb_autoclick_key_down(mrb_state* mrb, mrb_value klass)
{
	INPUT ip;
	mrb_sym key_name;
	mrb_int key_code;
	mrb_value vk_module;
	mrb_value tmp;

	mrb_get_args(mrb, "n", &key_name);
	vk_module = mrb_obj_value(mrb_module_get(mrb, VK_MODULE_NAME));
	tmp = mrb_symbol_value(key_name);
	tmp = mrb_funcall(mrb, vk_module, "code_from_name", 1, tmp);
	key_code = mrb_fixnum(tmp);

	ip.type = INPUT_KEYBOARD;
	ip.ki.wVk = key_code;
	ip.ki.wScan = 0;
	ip.ki.dwFlags = 0; /* Key down */
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	SendInput(1, &ip, sizeof(INPUT));
	return mrb_nil_value();
}

static mrb_value
mrb_autoclick_key_up(mrb_state* mrb, mrb_value klass)
{
	INPUT ip;
	mrb_sym key_name;
	mrb_int key_code;
	mrb_value vk_module;
	mrb_value tmp;

	mrb_get_args(mrb, "n", &key_name);
	vk_module = mrb_obj_value(mrb_module_get(mrb, VK_MODULE_NAME));
	tmp = mrb_symbol_value(key_name);
	tmp = mrb_funcall(mrb, vk_module, "code_from_name", 1, tmp);
	key_code = mrb_fixnum(tmp);

	ip.type = INPUT_KEYBOARD;
	ip.ki.wVk = key_code;
	ip.ki.wScan = 0;
	ip.ki.dwFlags = 2; // Key up
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	SendInput(1, &ip, sizeof(INPUT));
	return mrb_nil_value();
}

static mrb_value
mrb_autoclick_key_stroke(mrb_state* mrb, mrb_value klass)
{
	INPUT ips[2];
	mrb_sym key_name;
	mrb_int key_code;
	mrb_value vk_module;
	mrb_value tmp;

	mrb_get_args(mrb, "n", &key_name);
	vk_module = mrb_obj_value(mrb_module_get(mrb, VK_MODULE_NAME));
	tmp = mrb_symbol_value(key_name);
	tmp = mrb_funcall(mrb, vk_module, "code_from_name", 1, tmp);
	key_code = mrb_fixnum(tmp);

	ips[0].type = INPUT_KEYBOARD;
	ips[0].ki.wVk = key_code;
	ips[0].ki.wScan = 0;
	ips[0].ki.dwFlags = 0; /* Key down */
	ips[0].ki.time = 0;
	ips[0].ki.dwExtraInfo = 0;
	ips[1] = ips[0];
	ips[1].ki.dwFlags = 2; /* Key up */
	SendInput(2, ips, sizeof(INPUT));
	return mrb_nil_value();
}

static mrb_value
mrb_autoclick_type(mrb_state *mrb, mrb_value klass)
{
	INPUT ips[2];
	int i;
	int nchars;
	char *input_str;
	mrb_int input_str_len;
	LPWSTR output_str;

	mrb_get_args(mrb, "s", &input_str, &input_str_len);
	if (!input_str_len)
		return mrb_nil_value();

	/* Get number of wchars to use in the buffer */
	nchars = MultiByteToWideChar(
		CP_UTF8, MB_ERR_INVALID_CHARS,
		input_str, input_str_len,
		NULL, 0
	);

	/* Create buffer and convert from UTF-8 to UTF-16 */
	output_str = malloc(nchars * sizeof(WCHAR));
	nchars = MultiByteToWideChar(
		CP_UTF8, MB_ERR_INVALID_CHARS,
		input_str, input_str_len,
		output_str, nchars
	);

	/* Type each character in buffer as unicode (UTF-16) */
	ips[0].type = INPUT_KEYBOARD;
	ips[0].ki.wVk = 0;
	ips[0].ki.dwFlags = KEYEVENTF_UNICODE;
	ips[0].ki.time = 0;
	ips[0].ki.dwExtraInfo = 0;
	ips[1] = ips[0];
	ips[1].ki.dwFlags |= KEYEVENTF_KEYUP;

	for (i=0; i<nchars; ++i) {
		ips[0].ki.wScan = output_str[i];
		ips[1].ki.wScan = output_str[i];
		SendInput(2, ips, sizeof(INPUT));
	}

	free(output_str);
	return mrb_nil_value();
}

/* Function to indicate that a key is currently "down"
 * "down" would be equivalent to someone having their finger down
 * on the key at the current moment.
 */
static mrb_value
mrb_autoclick_is_key_down(mrb_state *mrb, mrb_value klass)
{
	SHORT key_state;
	mrb_sym key_name;
	mrb_int key_code;
	mrb_value vk_module;
	mrb_value tmp;

	mrb_get_args(mrb, "n", &key_name);
	vk_module = mrb_obj_value(mrb_module_get(mrb, VK_MODULE_NAME));
	tmp = mrb_symbol_value(key_name);
	tmp = mrb_funcall(mrb, vk_module, "code_from_name", 1, tmp);
	key_code = mrb_fixnum(tmp);
	key_state = GetKeyState(key_code);

	/* High order bit (sign bit) = key is down */
	if (key_state < 0) return mrb_true_value();
	else return mrb_false_value();
}

/* Function to indicate that a key is currently "toggled"
 * "toggled" would mean that the key is in some way "active,"
 * namely, a caps lock or num lock key being active.
 */
static mrb_value
mrb_autoclick_is_key_toggled(mrb_state *mrb, mrb_value klass)
{
	SHORT key_state;
	mrb_sym key_name;
	mrb_int key_code;
	mrb_value vk_module;
	mrb_value tmp;

	mrb_get_args(mrb, "n", &key_name);
	vk_module = mrb_obj_value(mrb_module_get(mrb, VK_MODULE_NAME));
	tmp = mrb_symbol_value(key_name);
	tmp = mrb_funcall(mrb, vk_module, "code_from_name", 1, tmp);
	key_code = mrb_fixnum(tmp);
	key_state = GetKeyState(key_code);

	/* Low order bit (even/odd bit) = key is toggled */
	if (key_state & 0x1) return mrb_true_value();
	else return mrb_false_value();
}

void
mrb_mruby_autoclick_gem_init(mrb_state *mrb)
{
	struct RClass *auto_click;

	LEFT_MOUSE_DOWN   = make_mouse_input(0,0,0,MOUSEEVENTF_LEFTDOWN,0,0);
	LEFT_MOUSE_UP     = make_mouse_input(0,0,0,MOUSEEVENTF_LEFTUP,0,0);
	RIGHT_MOUSE_DOWN  = make_mouse_input(0,0,0,MOUSEEVENTF_RIGHTDOWN,0,0);
	RIGHT_MOUSE_UP    = make_mouse_input(0,0,0,MOUSEEVENTF_RIGHTUP,0,0);
	MIDDLE_MOUSE_DOWN = make_mouse_input(0,0,0,MOUSEEVENTF_MIDDLEDOWN,0,0);
	MIDDLE_MOUSE_UP   = make_mouse_input(0,0,0,MOUSEEVENTF_MIDDLEUP,0,0);
	
	auto_click = mrb_define_module(mrb, MODULE_NAME);
	mrb_define_module_function(
		mrb, auto_click, "left_click",
		mrb_autoclick_left_click, MRB_ARGS_NONE()
	);
	mrb_define_module_function(
		mrb, auto_click, "right_click",
		mrb_autoclick_right_click, MRB_ARGS_NONE()
	);
	mrb_define_module_function(
		mrb, auto_click, "middle_click",
		mrb_autoclick_middle_click, MRB_ARGS_NONE()
	);
	mrb_define_module_function(
		mrb, auto_click, "double_click",
		mrb_autoclick_double_click, MRB_ARGS_NONE()
	);
	mrb_define_module_function(
		mrb, auto_click, "cursor_position",
		mrb_autoclick_cursor_position, MRB_ARGS_NONE()
	);
	mrb_define_module_function(
		mrb, auto_click, "mouse_move",
		mrb_autoclick_mouse_move, MRB_ARGS_REQ(2)
	);
	mrb_define_module_function(
		mrb, auto_click, "mouse_scroll_vertical",
		mrb_autoclick_mouse_scroll_vertical, MRB_ARGS_REQ(1)
	);
#if WINVER >= 0x0600
	mrb_define_module_function(
		mrb, auto_click, "mouse_scroll_horizontal",
		mrb_autoclick_mouse_scroll_horizontal, MRB_ARGS_REQ(1)
	);
#endif
	mrb_define_module_function(
		mrb, auto_click, "left_drag",
		mrb_autoclick_left_drag, MRB_ARGS_REQ(4)
	);
	mrb_define_module_function(
		mrb, auto_click, "right_drag",
		mrb_autoclick_right_drag, MRB_ARGS_REQ(4)
	);
	mrb_define_module_function(
		mrb, auto_click, "key_down",
		mrb_autoclick_key_down, MRB_ARGS_REQ(1)
	);
	mrb_define_module_function(
		mrb, auto_click, "key_up",
		mrb_autoclick_key_up, MRB_ARGS_REQ(1)
	);
	mrb_define_module_function(
		mrb, auto_click, "key_stroke",
		mrb_autoclick_key_stroke, MRB_ARGS_REQ(1)
	);
	mrb_define_module_function(
		mrb, auto_click, "type",
		mrb_autoclick_type, MRB_ARGS_REQ(1)
	);
	mrb_define_module_function(
		mrb, auto_click, "is_key_down?",
		mrb_autoclick_is_key_down, MRB_ARGS_REQ(1)
	);
	mrb_define_module_function(
		mrb, auto_click, "is_key_toggled?",
		mrb_autoclick_is_key_toggled, MRB_ARGS_REQ(1)
	);
}

void 
mrb_mruby_autoclick_gem_final(mrb_state *mrb)
{
}