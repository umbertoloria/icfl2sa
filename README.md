# ICFL2SA

Alcune cose state implementate ma ho deciso di non pubblicarle qui, tipo tutte quelle scritto nel .gitignore.

Ad esempio i risultati ottenuti con Valgrid e le analisi ottenibili da chiunque tramite comando.

È importante creare un file *input_file.txt* dove andrà inserita la stringa da immettere come input.

una volta inserita bisogna compilare ed eseguire. Non ho usato ottimizzazioni di compilazione ma se volete farlo, potete modificare il comando liberamente.

```
g++ *.cpp -o main
./main
```

Per esegire il **testing** invece:

```
valgrind --leak-check=full--show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./main
```