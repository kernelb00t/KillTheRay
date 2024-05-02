#!/bin/env python3
# Better make
from os import listdir, path, stat, system, makedirs
from sys import argv
from json import load, dump

# Default should be linux (or else it's prefixed with WIN_ for windows for example)
VERBOSE = True # 'verbose' simply prints the commands before running them
SRC_PATH = "src"
CC = "gcc"
LIBS_PATH = ""
LFLAGS = f"{LIBS_PATH} -I {SRC_PATH} -lraylib -lGL -lm -lpthread -ldl -lrt -lX11"
CFLAGS = "-g -Wall -Wno-missing-braces"
BIN = "killtheray"
RUN = f"chmod +x {BIN}; ./{BIN}"

WEB_CC = "emcc"
WEB_LIBS_PATH = "-L libs"
WEB_LFLAGS = f"{WEB_LIBS_PATH} -I {SRC_PATH} -lraylib -s ASYNCIFY -s USE_GLFW=3 -DPLATFORM_WEB"
WEB_BIN = "public/home.html"
WEB_RUN = "python main.py"

WIN_CC = "gcc"
WIN_LIBS_PATH = ""
WIN_CFLAGS = f"-I {SRC_PATH}"
WIN_LFLAGS = "-lraylib -lgdi32 -lwinmm"
WIN_BIN = "killtheray.exe"
WIN_RUN = f".\\{WIN_BIN}"

TIMESTAMPS = "config/bm.json"
OBJ_PATH = "temp"

def rls(path_, filter = lambda x: True):
	for target in listdir(path_):
		target = f"{path_}/{target}"
		if path.isdir(target):
			for ret in rls(target, filter):
				yield ret
		else:
			if filter(target):
				yield target
def last_ch(path: str):
	return stat(path).st_mtime
def load_opt():
	global opt
	try:
		with open(TIMESTAMPS, "r") as f:
			opt = load(f)
	except FileNotFoundError:
		print("[bm] Failed to open options.")
		opt = {}
	makedirs(OBJ_PATH, exist_ok=True)
def save_opt():
	global opt
	with open(TIMESTAMPS, "w") as f:
		dump(opt, f)
def yeet(code: int):
	save_opt()
	quit(code)
def fixmissing_files_opt():
	for path_ in rls(
		SRC_PATH,
		lambda p:
			p[-2:] == ".c"
		and p not in opt
	):
		# Iter through all source .c files and check if there are any missing in the options
		if path_ not in opt:
			opt[path_] = last_ch(path_)
def compile_all(check_ch_time = True, cflags = ""):
	for path_ in rls(
		SRC_PATH,
		lambda p:
			p[-2:] == ".c"
	):
		# Iter through all source .c files
		if (not check_ch_time) or opt[path_] < last_ch(path_):
			compile_to_obj(path_, cflags)
def compile_no_obj_all(cflags=""):
	""" cflags are additional flags to the setting CFLAGS """
	j = " ".join(rls(SRC_PATH, lambda x: x[-2:] == ".c"))
	if not j:
		raise Exception("Wtf ???")
	if code := shell(f"{CC} {j} -o {BIN} {cflags} {LFLAGS}"):
		yeet(code)
def compile_to_obj(p: str, cflags=""):
	""" cflags are additional flags to the setting CFLAGS """
	out_path = f"{OBJ_PATH}/{path.basename(p)}"[:-2] + '.o'
	if code := shell(f"{CC} -c {p} -o {out_path} {CFLAGS} {cflags}"):
		yeet(code)
	else:
		opt[p] = last_ch(p)
def link_all(cflags = ""):
	""" cflags are additional flags to the setting CFLAGS """
	j = " ".join(rls(OBJ_PATH, lambda x: x[-2:] == ".o"))
	if not j:
		raise Exception("Wtf ???")
	if code := shell(f"{CC} {j} -o {BIN} {cflags} {LFLAGS}"):
		yeet(code)
def debugging(tool: str):
	match tool:
		case "valgrind" | "mem" | "v":
			if code := shell(f"valgrind --leak-check=full --show-leak-kinds=all --verbose --track-origins=yes -s -- {BIN} examples/test.da"):
				yeet(code)
		# add more tools here
		case _:
			if code := shell(f"gdb {BIN}"):
				yeet(code)
def run():
	if code := shell(RUN):
		yeet(code)

if VERBOSE:
	def shell(cmd: str) -> int:
		print(cmd)
		return system(cmd)
else:
	shell = system

load_opt()
fixmissing_files_opt()

# Default compilation script (with no arguments)
if len(argv) == 1:
	compile_all(cflags="-O0") # Avoid any optimization for a simple test run
	link_all()
	run()
	yeet(0)

# Check for first argument
match argv[1]:
	case "web":
		# Switch global variables to WEB_* ones
		LFLAGS = WEB_LFLAGS
		BIN = WEB_BIN
		CC = WEB_CC
		LIBS_PATH = WEB_LIBS_PATH
		RUN = WEB_RUN
		# Do the thing
		compile_no_obj_all()
		run()
		yeet(0) # yipeeee
	case "win":
		# Switch global variables to WIN_* ones
		LFLAGS = WIN_LFLAGS
		BIN = WIN_BIN
		CC = WIN_CC
		LIBS_PATH = WIN_LIBS_PATH
		RUN = WIN_RUN
		CFLAGS = WIN_CFLAGS
		# Do the thing
		compile_all(cflags="-O3")
		link_all(cflags="-O3")
		run()
		yeet(0)
	case "all":
		# Simply compiles all of the objects
		compile_all(check_ch_time=False, cflags="-O0")
		yeet(0)
	case "production":
		# TODO: Check for additional flags (eg. 'bm.py production platform')
		compile_all(check_ch_time=False, cflags="-O3")
		link_all()
		yeet(0)
	case "dbg" | "gdb" | "debug" | "d":
		link_all(cflags="-O0")
		run()
		debugging("" if len(argv) == 2 else argv[2])
		yeet(0)
	case _:
		print("Syntax: bm [arg]")
		print("\targ can either be 'web', 'all', 'production', or 'debug'")

