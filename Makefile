run:
	g++ "main.cpp" -o a -O0 -Wall -Wno-missing-braces -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	chmod +x a
	./a

final:
	g++ "main.cpp" -o b -O3 -Wall -Wno-missing-braces -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 --fast-math -march=tigerlake -mtune=tigerlake -pipe -fno-plt -fexceptions -Wp,-D_FORTIFY_SOURCE=3 -Wformat -Werror=format-security -fstack-clash-protection -fcf-protection -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer
	chmod +x b
	./b

web:
	emcc main.cpp -o public/templates/home.html -L . -I . -lraylib -s ASYNCIFY -s USE_GLFW=3 -DPLATFORM_WEB
	mv public/templates/home.js public/static/home.js
	mv public/templates/home.wasm public/static/home.wasm
