
CC = gcc # This should now be C-only compatible 
WEB_CC = emcc
WEB_FLAGS = 
WEB_OUT = public/templates/home.html
SRC = 

run:
	$(CC) "main.cpp" -o a -O0 
	chmod +x a
	./a

final:
	$(CC) "main.cpp" -o b -O3 -Wall -Wno-missing-braces -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 --fast-math -march=tigerlake -mtune=tigerlake -pipe -fno-plt -fexceptions -Wp,-D_FORTIFY_SOURCE=3 -Wformat -Werror=format-security -fstack-clash-protection -fcf-protection -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer
	chmod +x b
	./b

web:
	$(WEB_COMPILE)
	# Nothing has to be moved anymore, flask uses the templates folder directly

launch-web:
	@$(WEB_COMPILE)
	cd public
	python3 main.py
