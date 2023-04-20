CC = g++
CFLAGS = -c
# CFLAGS = -c -Wall#$(CFLAGS)是编译器的选项，包括了一些警告和调试选项
# LFLAGS = -Wall#-Wall是g++编译器选项之一，它表示开启所有警告。

SOURCES = main.cpp
#如果SOURCES包含main.cpp和vec3.cpp，则$(SOURCES:.cpp=.o)将生成main.o和vec3.o
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = main

all: $(SOURCES) $(EXECUTABLE)
	
#$@默认是语句的目标，比如这个是$(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ -fopenmp

#$<是规则的依赖项中的第一个文件，即源文件的名称main.cpp
#.cpp.o是一个后缀规则，它指定了将.cpp源文件编译为.o目标文件的方式
.cpp.o: 
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	del $(OBJECTS) $(EXECUTABLE)
