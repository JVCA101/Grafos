make

# if not exists, create the results folder
if [ ! -d "results_greedy" ]; then
  mkdir -p results_greedy
fi
if [ ! -d "results_random" ]; then
  mkdir -p results_random
fi
if [ ! -d "results_reactive" ]; then
  mkdir -p results_reactive
fi

# vector with all the input files inside instances folder
files=$(ls instances)

# for each file in the vector, run the program with the file as input
for file in $files
do
  ./execGrupoGabrielJoaoThamiris -g instances/$file > results_greedy/$file&
  ./execGrupoGabrielJoaoThamiris -a instances/$file >> results_random/$file&
  ./execGrupoGabrielJoaoThamiris -r instances/$file >> results_reactive/$file&
done

