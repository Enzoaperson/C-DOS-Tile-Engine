CC=wcl386
DOS4G=-bt=dos -l=dos4g
EXENAME=plat.exe

main: ./src/MAIN.C
	mkdir -p ./build
	cd ./build && $(CC) $(DOS4G) -i="..src/creatures" ../src/creatures/*.C ../src/*.C  -fe="$(EXENAME)"
	cp -r ./src/data ./build
run: ./src/MAIN.C	
	mkdir -p ./build
	cd ./build && $(CC) $(DOS4G) -i="../src/creatures" ../src/creatures/*.C ../src/*.C -fe="$(EXENAME)"
	cp -r ./src/data ./build
	dosbox ./build/$(EXENAME) -exit 

love:
	@echo not war
