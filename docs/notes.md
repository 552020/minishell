# Notes:

1. Implement redirections that can be used with and without rl_dump_functions
   ex1: cat test.txt | >> test.txt (appends)
   ex2: cat test.txt | grep "hello" >> test.txt (appends)
   ex3: cat test.txt | grep "hello" >> test.txt | cat test.txt (does not append)
2. implement open file and close file for redirections correctly
   Errors:

3. correct:
   cat infile.txt | grep h
   This is computer
   hello world

4. wrong:
   cat infile.txt | grep h
   stuck at lexing first round

5. cat infile.txt >> outfile.txt | cat outfile.txt (this sometimes appends sometimes not in shell)
   our version does not append and I dont know if it should

6. echo 123 > infile.txt >> infile >> infile.txt

7. cat << eof | cat > outfile --- not working