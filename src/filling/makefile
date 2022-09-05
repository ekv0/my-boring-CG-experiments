main: main.o shader.o point.o line.o fill.o
	clang++ $^ -o $@ \
	-lGL \
	-lglfw3 \
	-lglbinding \
	-lX11 \
	-ldl \
	-lpthread \
	-lXrandr \
	-lXi

main.o : main.cc shader.hh point.hh
	clang++ -c $<

shader.o: shader.cc shader.hh
	clang++ -c $<

point.o: point.cc point.hh
	clang++ -c $<

line.o: line.cc line.hh
	clang++ -c $<

fill.o: fill.cc fill.hh line.hh point.hh
	clang++ -c $<

debug: main.cc shader.cc shader.hh
	clang++ main.cc shader.cc -g \
	-lGL \
	-lglfw3 \
	-lglbinding \
	-lX11 \
	-ldl \
	-lpthread \
	-lXrandr \
	-lXi \
	-o main_debug
	lldb main_debug

clean:
	rm -rf *.o main main_debug
