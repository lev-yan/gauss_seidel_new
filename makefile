

run:
	clear
	gcc gauss_seidel.cpp -o smd -lstdc++
	./smd
	@echo " "
	@ctags -R .

clean:
	rm -rf smd
