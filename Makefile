SOURCE = boids.c vector.c linkedlist.c
HEADER = list.h vector.h 

# Change this according to the type of system you are using
all: app_linux

app_win32: $(SOURCE) $(HEADER)
	cp SDL32.dll SDL.dll
	gcc -Wall -o app.exe -Iinclude $(SOURCE) SDL.dll

app_win64: $(SOURCE) $(HEADER)
	cp SDL64.dll SDL.dll
	gcc -Wall -o app.exe -Iinclude $(SOURCE) SDL.dll

app_mac: $(SOURCE) $(HEADER)
	gcc -Wall -o app -Iinclude `sdl-config --cflags --libs` $(SOURCE)

app_linux: $(SOURCE)
	gcc -Wall -o app -Iinclude $(SOURCE) $(HEADER) -lSDL2 -lSDL2_image -lm

clean:
	rm -f *.o app.exe app *~
