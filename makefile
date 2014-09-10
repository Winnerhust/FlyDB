
CC=g++
GTEST_CPPFLAGS=-I/usr/include -pthread 
GTEST_LDFLAGS=-L/usr/lib64 -lgtest -pthread

CPPFLAGS=-Isrc/ -Dprotected=public -Dprivate=public -Wall #-fno-stack-protector #无需栈保护
all: testcase 

testcase:
	$(CC) $(GTEST_CPPFLAGS) $(CPPFLAGS) -g -c src/schema.cpp
	$(CC) $(GTEST_CPPFLAGS) $(CPPFLAGS) -g -c src/record.cpp
	$(CC) $(GTEST_CPPFLAGS) $(CPPFLAGS) -g -c src/page.cpp
	$(CC) $(GTEST_CPPFLAGS) $(CPPFLAGS) -g -c src/file.cpp
	$(CC) $(GTEST_CPPFLAGS) $(CPPFLAGS) -g -c src/dbfile.cpp
	$(CC) $(GTEST_CPPFLAGS) $(CPPFLAGS) -g -c src/stringutil.cpp
	$(CC) $(GTEST_CPPFLAGS) $(CPPFLAGS) -g -c test/schema_test.cpp
	$(CC) $(GTEST_CPPFLAGS) $(CPPFLAGS) -g -c test/record_test.cpp
	$(CC) $(GTEST_CPPFLAGS) $(CPPFLAGS) -g -c test/page_test.cpp
	$(CC) $(GTEST_CPPFLAGS) $(CPPFLAGS) -g -c test/file_test.cpp
	$(CC) $(GTEST_CPPFLAGS) $(CPPFLAGS) -g -c test/dbfile_test.cpp
	$(CC) $(GTEST_LDFLAGS) -g -o testcase schema_test.o schema.o stringutil.o record.o record_test.o page.o page_test.o file.o file_test.o dbfile.o dbfile_test.o
clean:	
	rm -f *.o testcase



	
#	g++  $(gtest-config --cppflags --cxxflags) -o $(TARGET).o -c test_main.cpp
#	g++ $(gtest-config --ldflags --libs) -o $(TARGET) $(TARGET).o
