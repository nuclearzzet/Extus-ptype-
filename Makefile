extus: ./src/*.c
	gcc -s -ftime-report ./src/*.c -municode -lWs2_32 -lcomctl32 -lgdi32 -o ./bin/extus(ptype).exe
	./bin/extus(ptype).exe
	
run: ./bin/extus(ptype).exe
	./bin/extus(ptype).exe