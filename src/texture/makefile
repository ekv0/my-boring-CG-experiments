main: main.o shader.o image.o point.o rgb.o texture.o
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

image.o: image.cc
	clang++ -c $<

point.o: point.cc point.hh
	clang++ -c $<

rgb.o: rgb.cc rgb.hh
	clang++ -c $<

texture.o: texture.cc texture.hh rgb.hh point.hh
	clang++ -c $<

debug: main.cc shader.cc shader.hh
	clang++ main.cc shader.cc image.cc point.cc rgb.cc -g \
	-lGL \
	-lglfw3 \
	-lglbinding \
	-lX11 \
	-ldl \
	-lpthread \
	-lXrandr \
	-lXi \
	-o main_debug && \
	lldb main_debug

clean:
	rm -rf *.o main main_debug
