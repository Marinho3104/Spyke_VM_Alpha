
built_ins_path := ./built_ins/
compiler_path := ./compiler/
entry_point_path := ./test/
parser_path := ./parser/
utils_path := ./utils/

output_name := ./output

all:

	$(MAKE) -C $(entry_point_path)
	$(MAKE) -C $(built_ins_path)
	$(MAKE) -C $(compiler_path)
	$(MAKE) -C $(parser_path)
	$(MAKE) -C $(utils_path)

	$(MAKE) compile
	$(MAKE) clean

compile:

	g++ -o $(output_name) ./*.cpp -g

clean:

	rm -f ./*.cpp
	rm -f ./*.h
	rm -f ./*.o