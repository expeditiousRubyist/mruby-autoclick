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
}

void 
mrb_mruby_autoclick_gem_final(mrb_state *mrb)
{
}