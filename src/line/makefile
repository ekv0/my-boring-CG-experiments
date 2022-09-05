main: main.o shader.o line.o
	clang++ $^ -o $@ \
	-lGL \
	-lglfw3 \
	-lglbinding \
	-lX11 \
	-ldl \
	-lpthread \
	-lXrandr \
	-lXi

main.o : main.cc shader.hh
	clang++ -c $<

shader.o: shader.cc shader.hh
	clang++ -c $<

line.o: line.cc line.hh
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
