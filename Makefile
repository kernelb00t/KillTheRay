# run:
# 	g++ "main.cpp" -o a -O0 -Wall -Wno-missing-braces -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
# 	chmod +x a
# 	./a

# final:
# 	g++ "main.cpp" -o b -O3 -Wall -Wno-missing-braces -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 --fast-math -march=tigerlake -mtune=tigerlake -pipe -fno-plt -fexceptions -Wp,-D_FORTIFY_SOURCE=3 -Wformat -Werror=format-security -fstack-clash-protection -fcf-protection -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer
# 	chmod +x b
# 	./b

web:
	echo "J'ai rajouté la fonctionnalité pour compiler et lancer Flask derrière (c'est "run"), compiler seulement ("compile") ou lancer Flask seulement ("flask"). Regarde ./Makefile pour plus de précisions."

run:
	emcc main.cpp -o public/templates/home.html -L . -I . -lraylib -s ASYNCIFY -s USE_GLFW=3 -DPLATFORM_WEB -O3 -ffast-math -pipe -fno-plt -fexceptions -Wp,-D_FORTIFY_SOURCE=3 -Wformat -Werror=format-security -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer
	mv public/templates/home.js public/static/home.js
	mv public/templates/home.wasm public/static/home.wasm
	cd public
	python main.py

compile:
	emcc main.cpp -o public/templates/home.html -L . -I . -lraylib -s ASYNCIFY -s USE_GLFW=3 -DPLATFORM_WEB -O3 -ffast-math -pipe -fno-plt -fexceptions -Wp,-D_FORTIFY_SOURCE=3 -Wformat -Werror=format-security -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer
	mv public/templates/home.js public/static/home.js
	mv public/templates/home.wasm public/static/home.wasm

flask:
	cd public
	python main.py