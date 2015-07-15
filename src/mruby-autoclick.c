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

static const char *MODULE_NAME = "AutoClick";

static INPUT
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
		mrb, auto_click, "left_drag",
		mrb_autoclick_left_drag, MRB_ARGS_REQ(4)
	);
	mrb_define_module_function(
		mrb, auto_click, "right_drag",
		mrb_autoclick_right_drag, MRB_ARGS_REQ(4)
	);
}

void 
mrb_mruby_autoclick_gem_final(mrb_state *mrb)
{
}