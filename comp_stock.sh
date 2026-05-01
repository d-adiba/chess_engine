#!/bin/bash

make && ./drie > drie_output.txt
sort drie_output.txt > drie_output_sorted.txt
xsel -ob > stockfish_output.txt
sort stockfish_output.txt > stockfish_output_sorted.txt
rm drie_output.txt stockfish_output.txt
diff drie_output_sorted.txt stockfish_output_sorted.txt
echo "Diff complete. Check above for any differences."