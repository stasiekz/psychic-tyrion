markowgen: main.o argparser.o reader.o textgen.o statgen.o storage.o serializer.o
	$(CC) -o markowgen $^

	
argparser.o: argparser.h

reader.o: reader.h storage.h argparser.h statgen.h

textgen.o: textgen.h storage.h argparser.h statgen.h

statgen.o: statgen.h argparser.h storage.h

storage.o: storage.h

serializer.o: serializer.h argparser.h storage.h

.PHONY: clean

clean:
	-rm *.o 


