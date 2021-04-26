LIBPATH := ./lib
BINNAMES = main $(wildcard $(LIBPATH)/*.so)

all: main $(addprefix $(LIBPATH)/lib, $(addsuffix .so, sum subtraction multiplication division))

main: main.c plugin_interface.h
	gcc main.c plugin_interface.h -o main -ldl

test: main.c plugin_interface.h
	gcc main.c plugin_interface.h -g -O0 -o main -ldl

$(LIBPATH)/lib%.so: $(LIBPATH)/%.o 
	gcc -shared $< -o $@

$(LIBPATH)/%.o: $(LIBPATH)/%.c plugin_interface.h
	gcc -c $< -o $@



clean:
	rm $(BINNAMES)
